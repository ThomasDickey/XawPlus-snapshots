/*
 * $XTermId: SmeBSB.c,v 1.10 2024/04/28 23:49:28 tom Exp $
 * $Xorg: SmeBSB.c,v 1.5 2001/02/09 02:03:45 xorgcvs Exp $
 */

/*
Copyright 2022,2024  Thomas E. Dickey
Copyright 1989, 1994, 1998  The Open Group

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
 */

/*
 * SmeBSB.c - Source code file for BSB Menu Entry object.
 *
 * Date:    September 26, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 *
 * This file contains modifications for XawPlus, Roland Krause 2002
 */

#include "private.h"

#include "UTF8.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/SimpleMenu.h>
#include <X11/XawPlus/SmeBSBP.h>
#include <X11/XawPlus/Cardinals.h>
#include <X11/XawPlus/StrToPmap.h>
#include <X11/Xmu/Drawing.h>

#ifdef X_NOT_STDC_ENV
extern int abs();
#endif

#define ONE_HUNDRED 100

#define offset(field) XtOffsetOf(SmeBSBRec, sme_bsb.field)

static XtResource resources[] =
{
    {XtNlabel, XtCLabel, XtRString, sizeof(String),
     offset(label), XtRString, NULL},
    {XtNvertSpace, XtCVertSpace, XtRInt, sizeof(int),
     offset(vert_space), XtRImmediate, (XtPointer) 25},
    {XtNleftBitmap, XtCLeftBitmap, XtRBitmap, sizeof(Pixmap),
     offset(left_bitmap), XtRImmediate, (XtPointer) None},
    {XtNleftClipMask, XtCLeftBitmap, XtRClipMask, sizeof(Pixmap),
     offset(left_clipmask), XtRImmediate, (XtPointer) None},
    {XtNjustify, XtCJustify, XtRJustify, sizeof(XtJustify),
     offset(justify), XtRImmediate, (XtPointer) XtJustifyLeft},
    {XtNrightBitmap, XtCRightBitmap, XtRBitmap, sizeof(Pixmap),
     offset(right_bitmap), XtRImmediate, (XtPointer) None},
    {XtNrightClipMask, XtCRightBitmap, XtRClipMask, sizeof(Pixmap),
     offset(right_clipmask), XtRImmediate, (XtPointer) None},
    {XtNleftMargin, XtCHorizontalMargins, XtRDimension, sizeof(Dimension),
     offset(left_margin), XtRImmediate, (XtPointer) 4},
    {XtNrightMargin, XtCHorizontalMargins, XtRDimension, sizeof(Dimension),
     offset(right_margin), XtRImmediate, (XtPointer) 4},
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
     offset(foreground), XtRString, XtDefaultForeground},
    {XtNfont, XtCFont, XtRFontStruct, sizeof(XFontStruct *),
     offset(font), XtRString, XtDefaultFont},
    {XtNfontSet, XtCFontSet, XtRFontSet, sizeof(XFontSet),
     offset(fontset), XtRString, XtDefaultFontSet},
    {XtNencoding, XtCEncoding, XtRUnsignedChar, sizeof(unsigned char),
     offset(encoding), XtRImmediate, (XtPointer) XawTextEncoding8bit},
};
#undef offset

/*
 * Semi Public function definitions.
 */

static void Redisplay(Widget w, XEvent *event, Region region);
static void Destroy(Widget w);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void FlipColors(Widget w);
static void ClassInitialize(void);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList
			 args, Cardinal *num_args);
static XtGeometryResult QueryGeometry(Widget w, XtWidgetGeometry * intended,
				      XtWidgetGeometry * return_val);

/*
 * Private Function Definitions.
 */

static void GetDefaultSize(Widget w, Dimension *width, Dimension *height);
static void DrawBitmaps(Widget w, GC gc);
static void GetBitmapInfo(Widget w, Boolean is_left);
static void CreateGCs(Widget w);
static void DestroyGCs(Widget w);
/* *INDENT-OFF* */
#define superclass (&smeClassRec)
SmeBSBClassRec smeBSBClassRec = {
  {
    /* superclass         */    (WidgetClass) superclass,
    /* class_name         */    "SmeBSB",
    /* size               */    sizeof(SmeBSBRec),
    /* class_initializer  */	ClassInitialize,
    /* class_part_initialize*/	NULL,
    /* Class init'ed      */	FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */	NULL,
    /* realize            */    NULL,
    /* actions            */    NULL,
    /* num_actions        */    ZERO,
    /* resources          */    resources,
    /* resource_count     */	XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    FALSE,
    /* compress_exposure  */    XtExposeNoCompress,
    /* compress_enterleave*/ 	FALSE,
    /* visible_interest   */    FALSE,
    /* destroy            */    Destroy,
    /* resize             */    NULL,
    /* expose             */    Redisplay,
    /* set_values         */    SetValues,
    /* set_values_hook    */	NULL,
    /* set_values_almost  */	XtInheritSetValuesAlmost,
    /* get_values_hook    */	NULL,
    /* accept_focus       */    NULL,
    /* intrinsics version */	XtVersion,
    /* callback offsets   */    NULL,
    /* tm_table		  */    NULL,
    /* query_geometry	  */    QueryGeometry,
    /* display_accelerator*/    NULL,
    /* extension	  */    NULL
  },{
    /* SimpleMenuClass Fields */
    /* highlight          */	FlipColors,
    /* unhighlight        */	FlipColors,
    /* notify             */	XtInheritNotify,
    /* extension	  */	NULL
  }, {
    /* BSBClass Fields */
    /* extension	  */    NULL
  }
};
/* *INDENT-ON* */

WidgetClass smeBSBObjectClass = (WidgetClass) & smeBSBClassRec;

/************************************************************
 *
 * Semi-Public Functions.
 *
 ************************************************************/

/*	Function Name: ClassInitialize
 *	Description: Initializes the SmeBSBObject class.
 *	Arguments: none.
 *	Returns: none.
 */

static void
ClassInitialize(void)
{
    static XtConvertArgRec screenConvertArg[] =
    {
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.screen),
	 sizeof(Screen *)},
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec,
	 core.colormap), sizeof(Colormap)},
    };

    XawInitializeWidgetSet();
    XtAddConverter(XtRString, XtRJustify, XmuCvtStringToJustify, NULL, 0);

    XtAddConverter(XtRString, XtRPixmap, XawCvtStringToPixmap,
		   screenConvertArg, XtNumber(screenConvertArg));
    XtAddConverter(XtRString, XtRClipMask, XawCvtStringToClipMask,
		   screenConvertArg, XtNumber(screenConvertArg));
}

/*      Function Name: Initialize
 *      Description: Initializes the simple menu widget
 *      Arguments: request - the widget requested by the argument list.
 *                 new     - the new widget with both resource and non
 *                           resource values.
 *      Returns: none.
 */

static void
Initialize(
	      Widget request GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    SmeBSBObject entry = (SmeBSBObject) new;

    if (entry->sme_bsb.label == NULL) {
	entry->sme_bsb.label = XtName(new);
	entry->sme_bsb.encoding = FALSE;
    } else {
	if (entry->sme_bsb.encoding)	/* Wide character encoding */
	    entry->sme_bsb.label = (char *) UTF8toUCS2(entry->sme_bsb.label);

	/* 8 bit character or multibyte character encoding */

	else
	    entry->sme_bsb.label = XtNewString(entry->sme_bsb.label);
    }
    CreateGCs(new);
    GetBitmapInfo(new, TRUE);	/* Left Bitmap Info */
    GetBitmapInfo(new, FALSE);	/* Right Bitmap Info */
    GetDefaultSize(new, &(entry->rectangle.width), &(entry->rectangle.height));
}

/*      Function Name: Destroy
 *      Description: Called at destroy time, cleans up.
 *      Arguments: w - the simple menu widget.
 *      Returns: none.
 */

static void
Destroy(Widget w)
{
    SmeBSBObject entry = (SmeBSBObject) w;

    DestroyGCs(w);
    if (entry->sme_bsb.label != XtName(w))
	XtFree(DeConst(entry->sme_bsb.label));
}

/*      Function Name: Redisplay
 *      Description: Redisplays the contents of the widget.
 *      Arguments: w - the simple menu widget.
 *                 event - the X event that caused this redisplay.
 *                 region - the region the needs to be repainted.
 *      Returns: none.
 */

static void
DrawString16(
		Widget w,
		GC gc,
		int x,
		int y,
		const char *label,
		int len)
{
    SmeBSBObject entry = (SmeBSBObject) w;

    if (XtIsSensitive(w) && XtIsSensitive(XtParent(w))) {
	/* Draw the string in normal or revert style */

	XDrawString16(XtDisplayOfObject(w), XtWindowOfObject(w), gc, x, y,
		      (const XChar2b *) label, len);
    } else {
	/* Draw the string in the insensitive style */

	XDrawString16(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_bsb.lgray_gc,
		      x + 1, y + 1, (const XChar2b *) label, len);
	XDrawString16(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_bsb.gray_gc,
		      x, y, (const XChar2b *) label, len);
    }
}

/* ------------------------------------------------------------------------- */

static void
DrawStringInt(
		 Widget w,
		 GC gc,
		 int x,
		 int y,
		 const char *label,
		 int len)
{
    SmeBSBObject entry = (SmeBSBObject) w;

    if (XtIsSensitive(w) && XtIsSensitive(XtParent(w))) {
	/* Draw the multi byte string in normal or revert style */

	XmbDrawString(XtDisplayOfObject(w), XtWindowOfObject(w),
		      entry->sme_bsb.fontset, gc, x, y, label, len);
    } else {
	/* Draw the multi byte string in the insensitive style */

	XmbDrawString(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_bsb.fontset,
		      entry->sme_bsb.lgray_gc, x + 1, y + 1, label, len);
	XmbDrawString(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_bsb.fontset,
		      entry->sme_bsb.gray_gc, x, y, label, len);
    }
}

/* ------------------------------------------------------------------------- */

static void
DrawString8(
	       Widget w,
	       GC gc,
	       int x,
	       int y,
	       const char *label,
	       int len)
{
    SmeBSBObject entry = (SmeBSBObject) w;

    if (XtIsSensitive(w) && XtIsSensitive(XtParent(w))) {
	/* Draw the string in normal or revert style */

	XDrawString(XtDisplayOfObject(w), XtWindowOfObject(w), gc, x, y,
		    label, len);
    } else {
	/* Draw the string in the insensitive style */

	XDrawString(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_bsb.lgray_gc,
		    x + 1, y + 1, label, len);
	XDrawString(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_bsb.gray_gc,
		    x, y, label, len);
    }
}

/* ------------------------------------------------------------------------- */

static int
CalcTextWidth(
		 SmeBSBObject entry,
		 const char *label,
		 int len)
{
    int width;

    if (entry->sme_bsb.encoding)
	width = XTextWidth16(entry->sme_bsb.font, (const XChar2b *) label, len);
    else {
	if (entry->sme.international)
	    width = XmbTextEscapement(entry->sme_bsb.fontset, label, len);
	else
	    width = XTextWidth(entry->sme_bsb.font, label, len);
    }
    return width;
}

/* ------------------------------------------------------------------------- */

static void
Redisplay(
	     Widget w,
	     XEvent *event GCC_UNUSED,
	     Region region GCC_UNUSED)
{
    GC gc;
    SmeBSBObject entry = (SmeBSBObject) w;
    int font_ascent, font_descent, x_loc, y_loc;

    int fontset_ascent, fontset_descent;
    XFontSetExtents *ext = XExtentsOfFontSet(entry->sme_bsb.fontset);

    font_ascent = font_descent = fontset_ascent = fontset_descent = 0;
    entry->sme_bsb.set_values_area_cleared = FALSE;

    if (entry->sme.international == True) {
	fontset_ascent = abs(ext->max_ink_extent.y);
	fontset_descent = ext->max_ink_extent.height - fontset_ascent;
    } else {			/*else, compute size from font like R5 */
	font_ascent = entry->sme_bsb.font->max_bounds.ascent;
	font_descent = entry->sme_bsb.font->max_bounds.descent;
    }
    y_loc = entry->rectangle.y;
    x_loc = entry->rectangle.x;

    if (XtIsSensitive(w) && XtIsSensitive(XtParent(w))) {
	if (w == XawSimpleMenuGetActiveEntry(XtParent(w))) {
	    XFillRectangle(XtDisplayOfObject(w), XtWindowOfObject(w),
			   entry->sme_bsb.norm_gc, x_loc, y_loc,
			   (unsigned int) entry->rectangle.width,
			   (unsigned int) entry->rectangle.height);
	    gc = entry->sme_bsb.rev_gc;
	} else
	    gc = entry->sme_bsb.norm_gc;
    } else
	gc = entry->sme_bsb.norm_gc;

    if (entry->sme_bsb.label != NULL) {
	const char *label = entry->sme_bsb.label;
	int len, width, t_width;

	/* Determine the length of the label string */

	if (entry->sme_bsb.encoding)
	    len = str16len((const char16 *) label);
	else
	    len = (int) strlen(label);

	/* Calculate the position of the string */

	switch (entry->sme_bsb.justify) {
	case XtJustifyCenter:

	    t_width = CalcTextWidth(entry, label, len);
	    width = entry->rectangle.width - (entry->sme_bsb.left_margin +
					      entry->sme_bsb.right_margin);
	    x_loc = entry->sme_bsb.left_margin + (width - t_width) / 2;
	    break;

	case XtJustifyRight:

	    t_width = CalcTextWidth(entry, label, len);
	    x_loc = entry->rectangle.width - (entry->sme_bsb.right_margin + t_width);
	    break;

	case XtJustifyLeft:
	    /*FALLTHROUGH */
	default:		/* Everything else is XtJustifyLeft */

	    x_loc = entry->sme_bsb.left_margin;
	    break;
	}
	/* Now we draw the string at its position */

	if (entry->sme_bsb.encoding)	/* Label string with UCS2 code/UNICODE */
	{
	    y_loc += ((int) entry->rectangle.height -
		      (font_ascent + font_descent)) / 2 + font_ascent;
	    DrawString16(w, gc, x_loc, y_loc, label, len);
	} else {		/* Label string with multibyte code */
	    if (entry->sme.international) {
		y_loc += ((int) entry->rectangle.height -
			  (fontset_ascent + fontset_descent)) / 2 + fontset_ascent;
		DrawStringInt(w, gc, x_loc, y_loc, label, len);
	    } else {		/* Label string with 8 bit code */
		y_loc += ((int) entry->rectangle.height -
			  (font_ascent + font_descent)) / 2 + font_ascent;
		DrawString8(w, gc, x_loc, y_loc, label, len);
	    }
	}
    }
    DrawBitmaps(w, gc);
}

/*      Function Name: SetValues
 *      Description: Relayout the menu when one of the resources is changed.
 *      Arguments: current - current state of the widget.
 *                 request - what was requested.
 *                 new - what the widget will become.
 *      Returns: none
 */

static Boolean
SetValues(
	     Widget current,
	     Widget request GCC_UNUSED,
	     Widget new,
	     ArgList args GCC_UNUSED,
	     Cardinal *num_args GCC_UNUSED)
{
    SmeBSBObject entry = (SmeBSBObject) new;
    SmeBSBObject old_entry = (SmeBSBObject) current;
    Boolean ret_val = FALSE, labelModified = FALSE;

    /* Handle changes of encoding */

    if (old_entry->sme_bsb.encoding != entry->sme_bsb.encoding) {
	/* If the encoding changes and the label not, the
	 * label string has the wrong format and we have to
	 * modify it. If both changes, the encoding and the label,
	 * in the next step a label in the necessary format is created.
	 */
	if (old_entry->sme_bsb.label == entry->sme_bsb.label) {
	    if (entry->sme_bsb.encoding)
		entry->sme_bsb.label = (char *) UTF8toUCS2(entry->sme_bsb.label);
	    else
		entry->sme_bsb.label = XtName(new);
	    if (old_entry->sme_bsb.label != XtName(new))
		XtFree(DeConst(old_entry->sme_bsb.label));
	}
	ret_val = labelModified = TRUE;
    }
    /* Handle changes of the label string */

    if (!labelModified && (old_entry->sme_bsb.label != entry->sme_bsb.label)) {
	/* Wide character encoding */

	if (entry->sme_bsb.encoding) {
	    entry->sme_bsb.label = (char *) UTF8toUCS2(entry->sme_bsb.label);
	    XtFree(DeConst(old_entry->sme_bsb.label));
	}
	/* 8 bit character or multibyte character encoding */

	else if (entry->sme_bsb.label != XtName(new)) {
	    entry->sme_bsb.label = XtNewString(entry->sme_bsb.label);
	    XtFree(DeConst(old_entry->sme_bsb.label));
	}
	ret_val = TRUE;
    }

    if (entry->rectangle.sensitive != old_entry->rectangle.sensitive)
	ret_val = TRUE;

    if ((entry->sme_bsb.left_bitmap != old_entry->sme_bsb.left_bitmap) ||
	(entry->sme_bsb.left_clipmask != old_entry->sme_bsb.left_clipmask)) {
	GetBitmapInfo(new, TRUE);
	ret_val = TRUE;
    }

    if ((entry->sme_bsb.right_bitmap != old_entry->sme_bsb.right_bitmap) ||
	(entry->sme_bsb.right_clipmask != old_entry->sme_bsb.right_clipmask)) {
	GetBitmapInfo(new, FALSE);
	ret_val = TRUE;
    }

    if (((old_entry->sme_bsb.font != entry->sme_bsb.font) &&
	 (old_entry->sme.international == False)) ||
	(old_entry->sme_bsb.foreground != entry->sme_bsb.foreground) ||
	(old_entry->sme.highlightColor != entry->sme.highlightColor) ||
	(old_entry->sme.shadowColor != entry->sme.shadowColor)) {
	DestroyGCs(current);
	CreateGCs(new);
	ret_val = TRUE;
    }

    if ((old_entry->sme_bsb.fontset != entry->sme_bsb.fontset) &&
	(old_entry->sme.international == True))
	/* DONT changes the GCs, because the fontset is not in them. */
	ret_val = TRUE;

    if (ret_val) {
	GetDefaultSize(new, &(entry->rectangle.width), &(entry->rectangle.height));
	entry->sme_bsb.set_values_area_cleared = TRUE;
    }
    return (ret_val);
}

/*	Function Name: QueryGeometry.
 *	Description: Returns the preferred geometry for this widget.
 *	Arguments: w - the menu entry object.
 *                 itended, return_val - the intended and return geometry info.
 *	Returns: A Geometry Result.
 *
 * See the Intrinsics manual for details on what this function is for.
 *
 * I just return the height and width of the label plus the margins.
 */

static XtGeometryResult
QueryGeometry(
		 Widget w,
		 XtWidgetGeometry * intended,
		 XtWidgetGeometry * return_val)
{
    SmeBSBObject entry = (SmeBSBObject) w;
    Dimension width, height;
    XtGeometryResult ret_val = XtGeometryYes;
    XtGeometryMask mode = intended->request_mode;

    GetDefaultSize(w, &width, &height);

    if (((mode & CWWidth) && (intended->width != width)) ||
	!(mode & CWWidth)) {
	return_val->request_mode |= CWWidth;
	return_val->width = width;
	ret_val = XtGeometryAlmost;
    }

    if (((mode & CWHeight) && (intended->height != height)) ||
	!(mode & CWHeight)) {
	return_val->request_mode |= CWHeight;
	return_val->height = height;
	ret_val = XtGeometryAlmost;
    }

    if (ret_val == XtGeometryAlmost) {
	mode = return_val->request_mode;

	if (((mode & CWWidth) && (width == entry->rectangle.width)) &&
	    ((mode & CWHeight) && (height == entry->rectangle.height)))
	    return (XtGeometryNo);
    }

    return (ret_val);
}

/*      Function Name: FlipColors
 *      Description: Invert the colors of the current entry.
 *      Arguments: w - the bsb menu entry widget.
 *      Returns: none.
 */

static void
FlipColors(Widget w)
{
    SmeBSBObject entry = (SmeBSBObject) w;

    if (entry->sme_bsb.set_values_area_cleared)
	return;

    XFillRectangle(XtDisplayOfObject(w), XtWindowOfObject(w),
		   entry->sme_bsb.invert_gc,
		   (int) entry->rectangle.x, (int) entry->rectangle.y,
		   (unsigned int) entry->rectangle.width,
		   (unsigned int) entry->rectangle.height);
}

/************************************************************
 *
 * Private Functions.
 *
 ************************************************************/

/*	Function Name: GetDefaultSize
 *	Description: Calculates the Default (preferred) size of
 *                   this menu entry.
 *	Arguments: w - the menu entry widget.
 *                 width, height - default sizes (RETURNED).
 *	Returns: none.
 */

static void
GetDefaultSize(
		  Widget w,
		  Dimension *width,
		  Dimension *height)
{
    SmeBSBObject entry = (SmeBSBObject) w;

    /* Initialize width and height */

    *width = entry->sme_bsb.left_margin + entry->sme_bsb.right_margin;
    *height = (Dimension) (entry->sme_bsb.font->max_bounds.ascent +
			   entry->sme_bsb.font->max_bounds.descent);

    /* Now calculate the final values adding the string width and
     * the font height to the current width and height.
     */
    if (entry->sme_bsb.encoding)
	*width += (Dimension) XTextWidth16(entry->sme_bsb.font, (const
								 XChar2b *) entry->sme_bsb.label,
					   str16len((const char16 *) entry->sme_bsb.label));
    else {
	if (entry->sme.international) {
	    XFontSetExtents *ext = XExtentsOfFontSet(entry->sme_bsb.fontset);

	    *width += (Dimension) XmbTextEscapement(entry->sme_bsb.fontset, entry->sme_bsb.label,
						    (int) strlen(entry->sme_bsb.label));
	    *height = ext->max_ink_extent.height;
	} else
	    *width += (Dimension) XTextWidth(entry->sme_bsb.font, entry->sme_bsb.label,
					     (int) strlen(entry->sme_bsb.label));
    }
    /* Let us see, if the left or the right bitmap is higher than the
     * font height and if so, use this value instead of the font height.
     */
    if (entry->sme_bsb.right_bitmap_height > *height)
	*height = entry->sme_bsb.right_bitmap_height;

    if (entry->sme_bsb.left_bitmap_height > *height)
	*height = entry->sme_bsb.left_bitmap_height;

    *height = (Dimension) (((int) *height * (ONE_HUNDRED +
			   entry->sme_bsb.vert_space)) / ONE_HUNDRED);
}

/*      Function Name: DrawBitmaps
 *      Description: Draws left and right bitmaps or Pixmaps
 *      Arguments: w - the simple menu widget.
 *                 gc - graphics context to use for drawing.
 *      Returns: none
 */

static void
DrawBitmaps(Widget w, GC gc)
{
    int x_loc, y_loc;
    SmeBSBObject entry = (SmeBSBObject) w;

    if ((entry->sme_bsb.left_bitmap == None) &&
	(entry->sme_bsb.right_bitmap == None))
	return;

    /* Draw Left Bitmap or Pixmap */

    if (entry->sme_bsb.left_bitmap != None) {
	x_loc = (int) (entry->sme_bsb.left_margin -
	    entry->sme_bsb.left_bitmap_width) / 2;
	y_loc = entry->rectangle.y + (int) (entry->rectangle.height -
					    entry->sme_bsb.left_bitmap_height)
	    / 2;

	if (entry->sme_bsb.left_bitmap_depth == 1) {
	    XCopyPlane(XtDisplayOfObject(w), entry->sme_bsb.left_bitmap,
		       XtWindowOfObject(w), gc, 0, 0,
		       entry->sme_bsb.left_bitmap_width,
		       entry->sme_bsb.left_bitmap_height, x_loc, y_loc, 1L);
	} else {
	    if (entry->sme_bsb.left_clipmask != None) {
		XSetClipMask(XtDisplayOfObject(w), gc, entry->sme_bsb.left_clipmask);
		XSetClipOrigin(XtDisplayOfObject(w), gc, x_loc, y_loc);
	    }
	    XCopyArea(XtDisplayOfObject(w), entry->sme_bsb.left_bitmap,
		      XtWindowOfObject(w),
		      gc, 0, 0, entry->sme_bsb.left_bitmap_width,
		      entry->sme_bsb.left_bitmap_height, x_loc, y_loc);
	    XSetClipMask(XtDisplayOfObject(w), gc, None);
	}
    }
    /* Draw Right Bitmap or Pixmap */

    if (entry->sme_bsb.right_bitmap != None) {
	x_loc = entry->rectangle.width - (int) (entry->sme_bsb.right_margin +
						entry->sme_bsb.right_bitmap_width)
	    / 2;
	y_loc = entry->rectangle.y + (int) (entry->rectangle.height -
					    entry->sme_bsb.right_bitmap_height)
	    / 2;

	if (entry->sme_bsb.right_bitmap_depth == 1) {
	    XCopyPlane(XtDisplayOfObject(w), entry->sme_bsb.right_bitmap,
		       XtWindowOfObject(w), gc, 0, 0,
		       entry->sme_bsb.right_bitmap_width,
		       entry->sme_bsb.right_bitmap_height, x_loc, y_loc, 1L);
	} else {
	    if (entry->sme_bsb.right_clipmask != None) {
		XSetClipMask(XtDisplayOfObject(w), gc, entry->sme_bsb.right_clipmask);
		XSetClipOrigin(XtDisplayOfObject(w), gc, x_loc, y_loc);
	    }
	    XCopyArea(XtDisplayOfObject(w), entry->sme_bsb.right_bitmap,
		      XtWindowOfObject(w),
		      gc, 0, 0, entry->sme_bsb.right_bitmap_width,
		      entry->sme_bsb.right_bitmap_height, x_loc, y_loc);
	    XSetClipMask(XtDisplayOfObject(w), gc, None);
	}
    }
}

/*      Function Name: GetBitmapInfo
 *      Description: Gets the bitmap information from either of the bitmaps.
 *      Arguments: w - the bsb menu entry widget.
 *                 is_left - TRUE if we are testing left bitmap,
 *                           FALSE if we are testing the right bitmap.
 *      Returns: none
 */

static void
GetBitmapInfo(Widget w, Boolean is_left)
{
    SmeBSBObject entry = (SmeBSBObject) w;
    unsigned int depth, bw;
    Window root;
    int x, y;
    unsigned int width, height;
    char buf[BUFSIZ];

    if (is_left) {
	if (entry->sme_bsb.left_bitmap != None) {
	    if (!XGetGeometry(XtDisplayOfObject(w),
			      entry->sme_bsb.left_bitmap, &root,
			      &x, &y, &width, &height, &bw, &depth)) {
		sprintf(buf, "SmeBSB Object: %s \"%s\".",
			"Could not get Left Bitmap geometry information for menu entry ",
			XtName(w));
		XtAppError(XtWidgetToApplicationContext(w), buf);
	    }
	    entry->sme_bsb.left_bitmap_width = (Dimension) width;
	    entry->sme_bsb.left_bitmap_height = (Dimension) height;
	    entry->sme_bsb.left_bitmap_depth = depth;
	} else {		/* No left bitmap */
	    entry->sme_bsb.left_bitmap_width = 0;
	    entry->sme_bsb.left_bitmap_height = 0;
	    entry->sme_bsb.left_bitmap_depth = 1;
	}
    } else if (entry->sme_bsb.right_bitmap != None) {
	if (!XGetGeometry(XtDisplayOfObject(w), entry->sme_bsb.right_bitmap, &root,
			  &x, &y, &width, &height, &bw, &depth)) {
	    sprintf(buf, "SmeBSB Object: %s \"%s\".",
		    "Could not get Right Bitmap geometry information for menu entry ",
		    XtName(w));
	    XtAppError(XtWidgetToApplicationContext(w), buf);
	}
	entry->sme_bsb.right_bitmap_width = (Dimension) width;
	entry->sme_bsb.right_bitmap_height = (Dimension) height;
	entry->sme_bsb.right_bitmap_depth = depth;
    } else {			/* No right bitmap */
	entry->sme_bsb.right_bitmap_width = 0;
	entry->sme_bsb.right_bitmap_height = 0;
	entry->sme_bsb.right_bitmap_depth = 1;
    }
}

/*      Function Name: CreateGCs
 *      Description: Creates all gc's for the simple menu widget.
 *      Arguments: w - the simple menu widget.
 *      Returns: none.
 */

static void
CreateGCs(Widget w)
{
    SmeBSBObject entry = (SmeBSBObject) w;
    XGCValues values;
    XtGCMask mask;

    if (entry->sme.international == True) {
	mask = GCForeground | GCBackground | GCGraphicsExposures;

	/* Get GC with reverse colors */

	values.foreground = XtParent(w)->core.background_pixel;
	values.background = entry->sme_bsb.foreground;
	values.graphics_exposures = FALSE;
	entry->sme_bsb.rev_gc = XtAllocateGC(w, 0, mask, &values, GCFont, 0);

	/* Get GC with normal colors */

	values.foreground = entry->sme_bsb.foreground;
	values.background = XtParent(w)->core.background_pixel;
	entry->sme_bsb.norm_gc = XtAllocateGC(w, 0, mask, &values, GCFont, 0);

	/* Get GCs to display insensitive entries */

	values.foreground = entry->sme.shadowColor;
	entry->sme_bsb.gray_gc = XtAllocateGC(w, 0, mask, &values, GCFont, 0);

	values.foreground = entry->sme.highlightColor;
	entry->sme_bsb.lgray_gc = XtAllocateGC(w, 0, mask, &values, GCFont, 0);
    } else {
	mask = GCForeground | GCBackground | GCGraphicsExposures | GCFont;

	/* Get GC with reverse colors */

	values.foreground = XtParent(w)->core.background_pixel;
	values.background = entry->sme_bsb.foreground;
	values.font = entry->sme_bsb.font->fid;
	values.graphics_exposures = FALSE;
	entry->sme_bsb.rev_gc = XtGetGC(w, mask, &values);

	/* Get GC with normal colors */

	values.foreground = entry->sme_bsb.foreground;
	values.background = XtParent(w)->core.background_pixel;
	entry->sme_bsb.norm_gc = XtGetGC(w, mask, &values);

	/* Get GCs to display insensitive entries */

	values.foreground = entry->sme.shadowColor;
	entry->sme_bsb.gray_gc = XtGetGC(w, mask, &values);

	values.foreground = entry->sme.highlightColor;
	entry->sme_bsb.lgray_gc = XtGetGC(w, mask, &values);
    }
    /* Get GC for flipping colors, this GC is font independent ! */

    values.foreground = entry->sme_bsb.foreground ^ values.background;
    values.background = 0;
    values.function = GXxor;
    mask = GCForeground | GCBackground | GCGraphicsExposures | GCFunction;
    entry->sme_bsb.invert_gc = XtGetGC(w, mask, &values);
}

/*      Function Name: DestroyGCs
 *      Description: Removes all gc's for the simple menu widget.
 *      Arguments: w - the simple menu widget.
 *      Returns: none.
 */

static void
DestroyGCs(Widget w)
{
    SmeBSBObject entry = (SmeBSBObject) w;

    XtReleaseGC(w, entry->sme_bsb.norm_gc);
    XtReleaseGC(w, entry->sme_bsb.gray_gc);
    XtReleaseGC(w, entry->sme_bsb.lgray_gc);
    XtReleaseGC(w, entry->sme_bsb.rev_gc);
    XtReleaseGC(w, entry->sme_bsb.invert_gc);
}
