/*
 * $XTermId: Simple.c,v 1.9 2024/04/28 23:51:58 tom Exp $
 * $Xorg: Simple.c,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 */

/**************************************************************************

Copyright 2022,2024  Thomas E. Dickey
Copyright 1987, 1988, 1994, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

This file contains modifications for XawPlus, Roland Krause 2002
Note that alle the Add3dExt code from XawPlus 2.x is moved into this widget.

***************************************************************************/

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/SimpleP.h>
#include <X11/Xmu/Drawing.h>

#define offset(field) XtOffsetOf(SimpleRec, simple.field)
#define coffset(field) XtOffsetOf(WidgetRec, core.field)

static XtResource resources[] =
{
    {XtNcursor, XtCCursor, XtRCursor, sizeof(Cursor),
     offset(cursor), XtRImmediate, (XtPointer) None},
    {XtNinsensitiveBorder, XtCInsensitive, XtRPixmap, sizeof(Pixmap),
     offset(insensitive_border), XtRImmediate, (XtPointer) NULL},
    {XtNpointerColor, XtCForeground, XtRPixel, sizeof(Pixel),
     offset(pointer_fg), XtRString, XtDefaultForeground},
    {XtNpointerColorBackground, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(pointer_bg), XtRString, XtDefaultBackground},
    {XtNcursorName, XtCCursor, XtRString, sizeof(String),
     offset(cursor_name), XtRString, NULL},
    {XtNinternational, XtCInternational, XtRBoolean, sizeof(Boolean),
     offset(international), XtRImmediate, (XtPointer) FALSE},
    {XtNhighlightColor, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(highlightColor), XtRString, "grey90"},
    {XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
     coffset(background_pixel), XtRString, "grey75"},
    {XtNshadowColor, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(shadowColor), XtRString, "grey40"},
    {XtNbuttonBorderWidth, XtCWidth, XtRDimension, sizeof(Dimension),
     offset(borderWidth), XtRImmediate, (XtPointer) 2}
#undef offset
#undef coffset
};
/* *INDENT-OFF* */
static void ClassPartInitialize(WidgetClass class);
static void ClassInitialize(void);
static void Destroy(Widget w);
static void Initialize(Widget old, Widget new, ArgList args, Cardinal *num_args);
static void Realize(Widget w, Mask *valueMask, XSetWindowAttributes *attributes);
static void ConvertCursor(Widget w);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static Boolean ChangeSensitive(Widget w);

SimpleClassRec simpleClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &widgetClassRec,
    /* class_name		*/	"Simple",
    /* widget_size		*/	sizeof(SimpleRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	ClassPartInitialize,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	XtExposeCompressSeries,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	NULL,
    /* expose			*/	NULL,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* simple fields */
    /* change_sensitive		*/	ChangeSensitive
  }
};
/* *INDENT-ON* */

WidgetClass simpleWidgetClass = (WidgetClass) & simpleClassRec;

/***********************************************************************
 *
 * Private functions
 *
 ***********************************************************************/

/*	Function Name: ConvertCursor
 *	Description:   Converts a name to a new cursor.
 *	Arguments:     w - the simple widget.
 *	Returns:       none.
 */

static void
ConvertCursor(Widget w)
{
    SimpleWidget simple = (SimpleWidget) w;
    XrmValue from, to;
    Cursor cursor;

    if (simple->simple.cursor_name == NULL)
	return;

    from.addr = (XPointer) DeConst(simple->simple.cursor_name);
    from.size = (unsigned) strlen((char *) from.addr) + 1;

    to.size = sizeof(Cursor);
    to.addr = (XPointer) & cursor;

    if (XtConvertAndStore(w, XtRString, &from, XtRColorCursor, &to)) {
	if (cursor != None)
	    simple->simple.cursor = cursor;
    } else {
	XtAppErrorMsg(XtWidgetToApplicationContext(w),
		      "convertFailed", "ConvertCursor", "XawError",
		      "Simple: ConvertCursor failed.",
		      (String *) NULL, (Cardinal *) NULL);
    }
}

/* 	Function Name:  DrawRectBorder
 *	Description:	Draw the border for the rectangle functions
 *	Arguments:	Dpy - Display
 *			Win - Window
 *			upperGC - GC for highlighting
 * 			lowerGC - GC for shading
 *			borderWith - Width of the rectangle border
 *	Returns:	None
 */

static void
DrawRectBorder(
		  Display *Dpy,	/* Display to paint to */
		  Drawable Win,	/* Window to paint in */
		  GC upperGC,	/* GC for upper and left border */
		  GC lowerGC,	/* GC for lower and right border */
		  int borderWidth,	/* Width of the border */
		  int x,
		  int y,
		  int width,
		  int height)
{
    int xMin, xMax, yMin, yMax, i;

    /* Calculate points for the upper horizontal border an draw it */

    xMin = x;
    xMax = x + width - 2;
    yMin = y;
    for (i = 0; i < borderWidth; i++) {
	XDrawLine(Dpy, Win, upperGC, xMin, yMin, xMax, yMin);
	xMin++;
	xMax--;
	yMin++;
    }
    /* Calculate points for the left vertical border an draw it */

    xMin = x;
    yMin = y + 1;
    yMax = y + height - 2;
    for (i = 0; i < borderWidth; i++) {
	XDrawLine(Dpy, Win, upperGC, xMin, yMin, xMin, yMax);
	xMin++;
	yMin++;
	yMax--;
    }
    /* Calculate points for the horizontal lower border an draw it */

    xMin = x + borderWidth;
    xMax = x + width - borderWidth;
    yMax = y + height - borderWidth;
    for (i = 0; i < borderWidth; i++) {
	XDrawLine(Dpy, Win, lowerGC, xMin, yMax, xMax, yMax);
	xMin--;
	xMax++;
	yMax++;
    }
    /* Calculate points for the right vertical border an draw it */

    xMax = x + width - borderWidth;
    yMin = y + borderWidth;
    yMax = y + height - borderWidth;
    for (i = 0; i < borderWidth; i++) {
	XDrawLine(Dpy, Win, lowerGC, xMax, yMin, xMax, yMax);
	xMax++;
	yMin--;
	yMax++;
    }
}

/***********************************************************************
 *
 * Methods of this Widget
 *
 ***********************************************************************/

/***********************************************************************
 *
 * Class initialize method:	Initialize widget class:
 * 				Install required converters
 *
 ***********************************************************************/

static void
ClassInitialize(void)
{
    /* Arguments for the color converter */

    static XtConvertArgRec colConvertArg[] =
    {
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.screen),
	 sizeof(Screen *)},
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec,
	 core.colormap), sizeof(Colormap)}
    };

    static XtConvertArgRec convertArg[] =
    {
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.screen),
	 sizeof(Screen *)},
	{XtResourceString, (XtPointer) XtNpointerColor, sizeof(Pixel)},
	{XtResourceString, (XtPointer) XtNpointerColorBackground, sizeof(Pixel)},
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec,
	 core.colormap), sizeof(Colormap)}
    };

    XawInitializeWidgetSet();
    XtSetTypeConverter(XtRString, XtRColorCursor, XmuCvtStringToColorCursor,
		       convertArg, XtNumber(convertArg), XtCacheByDisplay,
		       (XtDestructor) NULL);
    XtSetTypeConverter(XtRString, XtRPixel, XtCvtStringToPixel,
		       colConvertArg, XtNumber(colConvertArg),
		       XtCacheByDisplay, (XtDestructor) NULL);
}

/***********************************************************************
 *
 * Class part initialize method
 *
 ***********************************************************************/

static void
ClassPartInitialize(WidgetClass class)
{
    SimpleWidgetClass c = (SimpleWidgetClass) class;
    SimpleWidgetClass super = (SimpleWidgetClass) c->core_class.superclass;

    if (c->simple_class.change_sensitive == NULL) {
	char buf[BUFSIZ];
	char *bufp;
	int len;
	char *fmt = "%s Widget: The Simple Widget class method 'change_sensitive' is undefined.\n"
	"A function must be defined or inherited.";

	if ((size_t) (len = (int) (strlen(fmt) +
	    strlen(c->core_class.class_name))) < sizeof buf)
	    bufp = buf;
	else
	    bufp = XtMalloc((Cardinal) (len + 1));

	if (bufp == NULL) {
	    bufp = buf;
	    strcpy(bufp, &fmt[11]);
	} else
	    (void) sprintf(bufp, fmt, c->core_class.class_name);

	XtWarning(buf);
	if (bufp != buf)
	    XtFree(bufp);
	c->simple_class.change_sensitive = ChangeSensitive;
    }

    if (c->simple_class.change_sensitive == XtInheritChangeSensitive)
	c->simple_class.change_sensitive = super->simple_class.change_sensitive;
}

/***********************************************************************
 *
 * Initialize method:	Initialize widget instance:
 * 			Get GCs for the drawing functions.
 *
 ***********************************************************************/

static void
Initialize(
	      Widget old GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    XGCValues values;
    SimpleWidget w = (SimpleWidget) new;

    /* The border width has to be greater than 0 */

    if (w->simple.borderWidth < 1)
	w->simple.borderWidth = 1;

    /* Get the required GCs */

    values.foreground = w->core.background_pixel;
    w->simple.backgrGC = XtGetGC(new, GCForeground, &values);

    values.foreground = w->simple.highlightColor;
    w->simple.highlightGC = XtGetGC(new, GCForeground, &values);

    values.foreground = w->simple.shadowColor;
    w->simple.shadowGC = XtGetGC(new, GCForeground, &values);
}

/***********************************************************************
 *
 * Realize method
 *
 ***********************************************************************/

static void
Realize(
	   Widget w,
	   Mask * valueMask,
	   XSetWindowAttributes * attributes)
{
    Pixmap border_pixmap = CopyFromParent;
    if (!XtIsSensitive(w)) {
	/* change border to gray; have to remember the old one,
	 * so XtDestroyWidget deletes the proper one */
	if (((SimpleWidget) w)->simple.insensitive_border == None)
	    ((SimpleWidget) w)->simple.insensitive_border =
		XmuCreateStippledPixmap(XtScreen(w),
					w->core.border_pixel,
					w->core.background_pixel,
					w->core.depth);
	border_pixmap = w->core.border_pixmap;
	attributes->border_pixmap =
	    w->core.border_pixmap = ((SimpleWidget) w)->simple.insensitive_border;

	*valueMask |= CWBorderPixmap;
	*valueMask &= (Mask) ~ CWBorderPixel;
    }

    ConvertCursor(w);

    if ((attributes->cursor = ((SimpleWidget) w)->simple.cursor) != None)
	*valueMask |= CWCursor;

    XtCreateWindow(w, (unsigned int) InputOutput, (Visual *) CopyFromParent,
		   *valueMask, attributes);

    if (!XtIsSensitive(w))
	w->core.border_pixmap = border_pixmap;
}

/***********************************************************************
 *
 * Destroy method:	Free all GCs
 *
 ***********************************************************************/

static void
Destroy(Widget w)
{
    SimpleWidget sw = (SimpleWidget) w;

    XtReleaseGC(w, sw->simple.shadowGC);
    XtReleaseGC(w, sw->simple.backgrGC);
    XtReleaseGC(w, sw->simple.highlightGC);
}

/***********************************************************************
 *
 * SetValues method:	Handle changes of resources
 *
 ***********************************************************************/

static Boolean
SetValues(
	     Widget current,
	     Widget request GCC_UNUSED,
	     Widget new,
	     ArgList args GCC_UNUSED,
	     Cardinal *num_args GCC_UNUSED)
{
    SimpleWidget s_old = (SimpleWidget) current;
    SimpleWidget s_new = (SimpleWidget) new;
    Boolean new_cursor = FALSE;
    Boolean redraw = FALSE;

    /* this disables user changes after creation */

    s_new->simple.international = s_old->simple.international;

    if (XtIsSensitive(current) != XtIsSensitive(new))
	(*((SimpleWidgetClass) XtClass(new))->
	 simple_class.change_sensitive) (new);

    if (s_old->simple.cursor != s_new->simple.cursor) {
	new_cursor = TRUE;
    }
    /*
     * We are not handling the string cursor_name correctly here.
     */
    if ((s_old->simple.pointer_fg != s_new->simple.pointer_fg) ||
	(s_old->simple.pointer_bg != s_new->simple.pointer_bg) ||
	(s_old->simple.cursor_name != s_new->simple.cursor_name)) {
	ConvertCursor(new);
	new_cursor = TRUE;
    }

    if (new_cursor && XtIsRealized(new))
	XDefineCursor(XtDisplay(new), XtWindow(new), s_new->simple.cursor);

    /* Check, if any color has changed. If so, free the old color cell and
     * store the new color in the GC.
     */
    if (s_old->simple.highlightColor != s_new->simple.highlightColor) {
	XSetForeground(XtDisplay(new), s_new->simple.highlightGC,
		       s_new->simple.highlightColor);
	redraw = TRUE;
    }
    if (s_old->core.background_pixel != s_new->core.background_pixel) {
	XSetForeground(XtDisplay(new), s_new->simple.backgrGC,
		       s_new->core.background_pixel);
	redraw = TRUE;
    }
    if (s_old->simple.shadowColor != s_new->simple.shadowColor) {
	XSetForeground(XtDisplay(new), s_new->simple.shadowGC,
		       s_new->simple.shadowColor);
	redraw = TRUE;
    }
    /* Check, if the border width has changed */

    if (s_old->simple.borderWidth != s_new->simple.borderWidth)
	redraw = TRUE;

    return (redraw);
}

/*****************************************************************************
 *
 * Exported class methods
 *
 *****************************************************************************/

static Boolean
ChangeSensitive(Widget w)
{
    if (XtIsRealized(w)) {
	if (XtIsSensitive(w))
	    if (w->core.border_pixmap != XtUnspecifiedPixmap)
		XSetWindowBorderPixmap(XtDisplay(w), XtWindow(w),
				       w->core.border_pixmap);
	    else
		XSetWindowBorder(XtDisplay(w), XtWindow(w),
				 w->core.border_pixel);
	else {
	    if (((SimpleWidget) w)->simple.insensitive_border == None)
		((SimpleWidget) w)->simple.insensitive_border =
		    XmuCreateStippledPixmap(XtScreen(w),
					    w->core.border_pixel,
					    w->core.background_pixel,
					    w->core.depth);
	    XSetWindowBorderPixmap(XtDisplay(w), XtWindow(w),
				   ((SimpleWidget) w)->
				   simple.insensitive_border);
	}
    }
    return False;
}

/* Draw a flat rectangle for buttons -----------------------------------
 */
void
XawFlatRectangle(
		    Widget w,
		    int x,
		    int y,
		    unsigned int width,
		    unsigned int height)
{
    SimpleWidget sw = (SimpleWidget) w;
    unsigned int xMin, xWidth, yMin, yHeight, i;

    xMin = (unsigned) x;
    yMin = (unsigned) y;
    xWidth = width - 1;
    yHeight = height - 1;
    for (i = 0; i < (int) sw->simple.borderWidth; i++) {
	XDrawRectangle(XtDisplay(w), sw->core.window, sw->simple.backgrGC,
		       (int) xMin, (int) yMin, xWidth, yHeight);
	xMin++;
	yMin++;
	xWidth -= 2;
	yHeight -= 2;
    }
}

/* Draw a 3D styled rectangle for released buttons --------------------
 */
void
XawRaisedRectangle(
		      Widget w,
		      int x,
		      int y,
		      unsigned int width,
		      unsigned int height)
{
    SimpleWidget sw = (SimpleWidget) w;

    DrawRectBorder(XtDisplay(w), sw->core.window,
		   sw->simple.highlightGC, sw->simple.shadowGC,
		   (int) sw->simple.borderWidth, x, y, (int) width, (int) height);
}

/* Draw a 3D styled rectangle for pressed buttons ----------------------
 */
void
XawSunkenRectangle(
		      Widget w,
		      int x,
		      int y,
		      unsigned int width,
		      unsigned int height)
{
    SimpleWidget sw = (SimpleWidget) w;

    DrawRectBorder(XtDisplay(w), sw->core.window,
		   sw->simple.shadowGC, sw->simple.highlightGC,
		   (int) sw->simple.borderWidth, x, y, (int) width, (int) height);
}

/* Draw a 3D styled raised button --------------------------------------
 */
void
XawRaisedButton(
		   Widget w,
		   int x,
		   int y,
		   unsigned int width,
		   unsigned int height)
{
    SimpleWidget sw = (SimpleWidget) w;

    XFillRectangle(XtDisplay(w), sw->core.window, sw->simple.backgrGC,
		   x, y, width, height);
    DrawRectBorder(XtDisplay(w), sw->core.window,
		   sw->simple.highlightGC, sw->simple.shadowGC,
		   (int) sw->simple.borderWidth, x, y, (int) width, (int) height);
}

/* Draw a 3D styled sunken button --------------------------------------
 */
void
XawSunkenButton(
		   Widget w,
		   int x,
		   int y,
		   unsigned int width,
		   unsigned int height)
{
    SimpleWidget sw = (SimpleWidget) w;

    XFillRectangle(XtDisplay(w), sw->core.window, sw->simple.backgrGC,
		   x, y, width, height);
    DrawRectBorder(XtDisplay(w), sw->core.window,
		   sw->simple.shadowGC, sw->simple.highlightGC,
		   (int) sw->simple.borderWidth, x, y, (int) width, (int) height);
}

/* Draw a flat button --------------------------------------------------
 */
void
XawFlatButton(
		 Widget w,
		 int x,
		 int y,
		 unsigned int width,
		 unsigned int height)
{
    SimpleWidget sw = (SimpleWidget) w;

    XFillRectangle(XtDisplay(w), sw->core.window, sw->simple.backgrGC,
		   x, y, width, height);
}
