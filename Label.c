/*
 * $XTermId: Label.c,v 1.10 2024/04/29 00:02:03 tom Exp $
 * $Xorg: Label.c,v 1.5 2001/02/09 02:03:43 xorgcvs Exp $
 */

/*************************************************************************

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

****************************************************************************/

/*
 * Label.c - Label widget
 *
 */

#include "private.h"

#include "UTF8.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/LabelP.h>
#include <X11/XawPlus/StrToPmap.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/Drawing.h>

#ifdef X_NOT_STDC_ENV
int abs();
#endif

#define streq(a,b) (strcmp( (a), (b) ) == 0)
#define MULTI_LINE_LABEL 32767

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/
/* *INDENT-OFF* */
/* Private Data */

#define offset(field) XtOffsetOf(LabelRec, field)
static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	offset(label.foreground), XtRString, XtDefaultForeground},
    {XtNfont,  XtCFont, XtRFontStruct, sizeof(XFontStruct *),
	offset(label.font),XtRString, XtDefaultFont},
    {XtNfontSet,  XtCFontSet, XtRFontSet, sizeof(XFontSet ),
        offset(label.fontset),XtRString, XtDefaultFontSet},
    {XtNlabel,  XtCLabel, XtRString, sizeof(String),
	offset(label.label), XtRString, NULL},
    {XtNencoding, XtCEncoding, XtRUnsignedChar, sizeof(unsigned char),
	offset(label.encoding), XtRImmediate, (XtPointer)XawTextEncoding8bit},
    {XtNjustify, XtCJustify, XtRJustify, sizeof(XtJustify),
	offset(label.justify), XtRImmediate, (XtPointer)XtJustifyCenter},
    {XtNinternalWidth, XtCWidth, XtRDimension,  sizeof(Dimension),
	offset(label.internal_width), XtRImmediate, (XtPointer)4},
    {XtNinternalHeight, XtCHeight, XtRDimension, sizeof(Dimension),
	offset(label.internal_height), XtRImmediate, (XtPointer)2},

    {XtNresize, XtCResize, XtRBoolean, sizeof(Boolean),
	offset(label.resize), XtRImmediate, (XtPointer)True},

    {XtNtruncLabel, XtCTruncate, XtRBoolean, sizeof(Boolean),
	offset(label.truncLabel), XtRImmediate, (XtPointer)True},
    {XtNtruncLeft, XtCTruncate, XtRBoolean, sizeof(Boolean),
	offset(label.truncLeftSide), XtRImmediate, (XtPointer)False},

    {XtNbitmap, XtCPixmap, XtRBitmap, sizeof(Pixmap),
	offset(label.pixmap), XtRImmediate, (XtPointer)None},
    {XtNclipMask, XtCPixmap, XtRClipMask, sizeof(Pixmap),
	offset(label.clipMask), XtRImmediate, (XtPointer)None},

    {XtNleftBitmap, XtCLeftBitmap, XtRBitmap, sizeof(Pixmap),
       offset(label.left_bitmap), XtRImmediate, (XtPointer)None},
    {XtNleftClipMask, XtCLeftBitmap, XtRClipMask, sizeof(Pixmap),
       offset(label.left_clipMask), XtRImmediate, (XtPointer)None},
};
#undef offset

/* Prototypes of label methods */

static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Resize(Widget w);
static void Redisplay(Widget w, XEvent *event, Region region);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static void ClassInitialize(void);
static void Destroy(Widget w);
static XtGeometryResult QueryGeometry(Widget w, XtWidgetGeometry *intended, XtWidgetGeometry *preferred);

#define superclass	(&simpleClassRec)
#define ownClassName	"Label"

LabelClassRec labelClassRec = {
  {
/* core_class fields */

    /* superclass	  	*/	(WidgetClass) superclass,
    /* class_name	  	*/	ownClassName,
    /* widget_size	  	*/	sizeof(LabelRec),
    /* class_initialize   	*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize		  	*/	XtInheritRealize,
    /* actions		  	*/	NULL,
    /* num_actions	  	*/	0,
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* compress_motion	  	*/	TRUE,
    /* compress_exposure  	*/	XtExposeCompressSeries,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest	  	*/	FALSE,
    /* destroy		  	*/	Destroy,
    /* resize		  	*/	Resize,
    /* expose		  	*/	Redisplay,
    /* set_values	  	*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus	 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* tm_table		   	*/	NULL,
    /* query_geometry		*/	QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
/* Simple class fields initialization */
  {
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
/* Label class fields initialization */
  {
    /* ignore 			*/	0
  }
};
/* *INDENT-ON* */

WidgetClass labelWidgetClass = (WidgetClass) & labelClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

/* Calculate width and height of the label for 8 bit characters
 */
static void
SetWidthAndHeight8Bit(
			 LabelWidget lw)
{
    XFontStruct *fs = lw->label.font;
    int width;
    char *nl, *label;

    lw->label.label_height = (Dimension) (fs->max_bounds.ascent + fs->max_bounds.descent);
    if (lw->label.label == NULL) {
	lw->label.label_len = 0;
	lw->label.label_width = 0;
    } else if ((nl = index(lw->label.label, '\n')) != NULL) {
	/* 8 bit multiline label */

	lw->label.label_len = MULTI_LINE_LABEL;
	lw->label.label_width = 0;
	for (label = lw->label.label; nl != NULL; nl = index(label, '\n')) {
	    width = XTextWidth(fs, label, (int) (nl - label));
	    if (width > (int) lw->label.label_width)
		lw->label.label_width = (Dimension) width;
	    label = nl + 1;
	    if (*label)
		lw->label.label_height +=
		    (Dimension) (fs->max_bounds.ascent + fs->max_bounds.descent);
	}
	if (*label) {
	    width = XTextWidth(fs, label, (int) strlen(label));
	    if (width > (int) lw->label.label_width)
		lw->label.label_width = (Dimension) width;
	}
    } else {			/* 8 bit single line label */
	lw->label.label_len = (Dimension) strlen(lw->label.label);
	lw->label.label_width = (Dimension) XTextWidth(fs, lw->label.label,
						       (int) lw->label.label_len);
    }
}

/* Calculate width and height of the label for 16 bit characters
 */
static void
SetWidthAndHeight2Byte(
			  LabelWidget lw)
{
    XFontStruct *fs = lw->label.font;
    int width;
    char16 *wnl, *label;

    lw->label.label_height = (Dimension) (fs->max_bounds.ascent + fs->max_bounds.descent);
    if (lw->label.label == NULL) {
	lw->label.label_len = 0;
	lw->label.label_width = 0;
    } else if ((wnl = str16chr((char16 *) lw->label.label, '\n')) != NULL) {
	/* 16 bit character multiline label */

	lw->label.label_len = MULTI_LINE_LABEL;
	lw->label.label_width = 0;
	for (label = (char16 *) lw->label.label; wnl != NULL;
	     wnl = str16chr(label, '\n')) {
	    width = XTextWidth16(fs, (XChar2b *) label, (int) (wnl - label));
	    if (width > (int) lw->label.label_width)
		lw->label.label_width = (Dimension) width;
	    label = wnl + 1;
	    if (*label)
		lw->label.label_height +=
		    (Dimension) (fs->max_bounds.ascent + fs->max_bounds.descent);
	}
	if (*label) {
	    width = XTextWidth16(fs, (XChar2b *) label, (int) str16len(label));
	    if (width > (int) lw->label.label_width)
		lw->label.label_width = (Dimension) width;
	}
    } else {			/* 16 bit character single line label */
	lw->label.label_len = (Dimension) str16len((char16 *) lw->label.label);
	lw->label.label_width = (Dimension) XTextWidth16(fs, (XChar2b *) lw->label.label,
							 (int) lw->label.label_len);
    }
}

/* Calculate width and height of the label for a multibyte string (UTF-8)
 */
static void
SetWidthAndHeightMulti(
			  LabelWidget lw)
{
    XFontSet fset = lw->label.fontset;
    XFontSetExtents *ext = XExtentsOfFontSet(fset);
    char *label, *nl;
    int width;

    lw->label.label_height = ext->max_logical_extent.height;
    if (lw->label.label == NULL) {
	lw->label.label_len = 0;
	lw->label.label_width = 0;
    } else if ((nl = index(lw->label.label, '\n')) != NULL) {
	/* Multibyte multiline label */

	lw->label.label_len = MULTI_LINE_LABEL;
	lw->label.label_width = 0;
	for (label = lw->label.label; nl != NULL; nl = index(label, '\n')) {
	    width = XmbTextEscapement(fset, label, (int) (nl - label));
	    if (width > (int) lw->label.label_width)
		lw->label.label_width = (Dimension) width;
	    label = nl + 1;
	    if (*label)
		lw->label.label_height += ext->max_logical_extent.height;
	}
	if (*label) {
	    width = XmbTextEscapement(fset, label, (int) strlen(label));
	    if (width > (int) lw->label.label_width)
		lw->label.label_width = (Dimension) width;
	}
    } else {			/* Multibyte single line label */
	lw->label.label_len = (Dimension) strlen(lw->label.label);
	lw->label.label_width = (Dimension) XmbTextEscapement(fset,
							      lw->label.label,
							      (int) lw->label.label_len);
    }
}

/*
 * Calculate width and height of displayed text in pixels
 */

static void
SetTextWidthAndHeight(
			 LabelWidget lw)
{
    Window root;
    int x, y;
    unsigned int width, height, bw, depth;

    if (lw->label.pixmap != None)	/* bitmap defined ? */
    {
	if (XGetGeometry(XtDisplay(lw), lw->label.pixmap, &root, &x, &y,
			 &width, &height, &bw, &depth)) {
	    lw->label.label_height = (Dimension) height;
	    lw->label.label_width = (Dimension) width;
	    lw->label.label_depth = depth;
	    return;
	}
    }
    /* Calculate the required width and height of the label depended
     * on the character set. Possible character sets are
     * ISO 8859, UNICODE or BMP (Basic Multilingual Plane defined in ISO 10646-1)
     * or UTF8 (Multibyte code, also defined in ISO 10646-1)
     */
    if (lw->simple.international == True)
	SetWidthAndHeightMulti(lw);
    else {
	if (lw->label.encoding)
	    SetWidthAndHeight2Byte(lw);
	else
	    SetWidthAndHeight8Bit(lw);
    }
}

/* Create required GCs
 */
static void
GetnormalGC(
	       LabelWidget lw)
{
    XGCValues values;

    values.foreground = lw->label.foreground;
    values.background = lw->core.background_pixel;
    values.font = lw->label.font->fid;
    values.graphics_exposures = False;

    /* Since Xmb/wcDrawString eats the font, I must use XtAllocateGC. */

    if (lw->simple.international == True)
	lw->label.normal_GC = XtAllocateGC((Widget) lw, 0,
					   GCForeground | GCBackground | GCGraphicsExposures,
					   &values, GCFont, 0);
    else
	lw->label.normal_GC = XtGetGC((Widget) lw,
				      GCForeground | GCBackground | GCFont | GCGraphicsExposures,
				      &values);
}

static void
GetgrayGCs(
	      LabelWidget lw)
{
    XGCValues values;

    values.foreground = lw->simple.shadowColor;
    values.background = lw->core.background_pixel;
    values.font = lw->label.font->fid;
    values.graphics_exposures = False;

    /* Since Xmb/wcDrawString eats the font, I must use XtAllocateGC. */

    if (lw->simple.international == True) {
	lw->label.gray_GC = XtAllocateGC((Widget) lw, 0,
					 (unsigned) GCForeground |
					 GCBackground | GCFont | GCGraphicsExposures,
					 &values, GCFont, 0);

	values.foreground = lw->simple.highlightColor;
	lw->label.lgray_GC = XtAllocateGC((Widget) lw, 0,
					  (unsigned) GCForeground |
					  GCBackground | GCFont | GCGraphicsExposures,
					  &values, GCFont, 0);
    } else {
	lw->label.gray_GC = XtGetGC((Widget) lw,
				    (unsigned) GCForeground | GCBackground |
				    GCFont | GCGraphicsExposures,
				    &values);

	values.foreground = lw->simple.highlightColor;
	lw->label.lgray_GC = XtGetGC((Widget) lw,
				     (unsigned) GCForeground | GCBackground
				     | GCFont | GCGraphicsExposures,
				     &values);
    }
}

static void
compute_bitmap_offsets(
			  LabelWidget clw,
			  LabelWidget lw)
{
    /*
     * label will be displayed at (internal_width, internal_height + lbm_y)
     */
    if (lw->label.lbm_height != 0) {
	lw->label.lbm_y = (((int) clw->core.height) -
			   ((int) lw->label.internal_height * 2) -
			   ((int) lw->label.lbm_height)) / 2;
	lw->label.lbm_x = (int) lw->label.internal_width;
    } else {
	lw->label.lbm_y = 0;
	lw->label.lbm_x = 0;
    }
}

/* Check if bitmaps or pixmaps in use and determine width, height and depth
 */
static void
set_bitmap_info(
		   LabelWidget clw GCC_UNUSED,
		   LabelWidget lw)
{
    Window root;
    int x, y;
    unsigned int bw;

    if (!(lw->label.left_bitmap &&
	  XGetGeometry(XtDisplay(lw), lw->label.left_bitmap, &root, &x, &y,
		       &lw->label.lbm_width, &lw->label.lbm_height, &bw, &lw->label.label_depth))) {
	lw->label.lbm_width = lw->label.lbm_height = 0;
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
    static XtConvertArgRec xpmCvtArg[] =
    {
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(LabelRec, core.screen),
	 sizeof(Screen *)},
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(LabelRec,
	 core.colormap), sizeof(Colormap)},
    };

    XawInitializeWidgetSet();
    XtAddConverter(XtRString, XtRJustify, XmuCvtStringToJustify, NULL, 0);

    XtAddConverter(XtRString, XtRBitmap, XawCvtStringToPixmap,
		   xpmCvtArg, XtNumber(xpmCvtArg));
    XtAddConverter(XtRString, XtRClipMask, XawCvtStringToClipMask,
		   xpmCvtArg, XtNumber(xpmCvtArg));
}

/***********************************************************************
 *
 * Initialize method:	Initialize widget instance:
 * 			Get GCs for drawing bitmaps, pixmaps or text.
 *			Determine required size of this label and call
 *			resize method of the underlying core widget.
 *
 ***********************************************************************/

static void
Initialize(
	      Widget request GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    LabelWidget lw = (LabelWidget) new;

    if (lw->label.label == NULL) {
	lw->label.label = XtNewString(lw->core.name);
	lw->label.encoding = FALSE;
    } else {
	if (lw->label.encoding)	/* Wide character encoding */
	    lw->label.label = (char *) UTF8toUCS2(lw->label.label);

	/* 8 bit character or multibyte character encoding */

	else
	    lw->label.label = XtNewString(lw->label.label);
    }
    GetnormalGC(lw);
    GetgrayGCs(lw);

    SetTextWidthAndHeight(lw);
    lw->label.label_full_len = NULL;

    if (lw->core.height == 0)
	lw->core.height = (Dimension) (lw->label.label_height + 2 * lw->label.internal_height);

    set_bitmap_info(lw, lw);	/* need core.height */

    if (lw->core.width == 0)	/* need label.lbm_width */
	lw->core.width = (lw->label.label_width + (2 * lw->label.internal_width)
			  + LEFT_OFFSET(lw));

    lw->label.label_x = lw->label.label_y = 0;
    (*XtClass(new)->core_class.resize) (new);
}

/***********************************************************************
 *
 * Redisplay method:	Redisplay the label
 *
 ***********************************************************************/

static void
_DrawLabel16(
		LabelWidget lw,
		char16 * labelText,
		int y,
		int len)
{
    Display *disp = XtDisplay((Widget) lw);
    Window win = XtWindow((Widget) lw);

    if (XtIsSensitive((Widget) lw))
	XDrawString16(disp, win, lw->label.normal_GC, lw->label.label_x, y,
		      (XChar2b *) labelText, len);
    else {
	XDrawString16(disp, win, lw->label.lgray_GC, lw->label.label_x + 1,
		      y + 1,
		      (XChar2b *) labelText, len);
	XDrawString16(disp, win, lw->label.gray_GC, lw->label.label_x, y,
		      (XChar2b *) labelText, len);
    }
}

static void
_DrawLabelString(
		    LabelWidget lw,
		    char *labelText,
		    int y,
		    int len)
{
    Display *disp = XtDisplay((Widget) lw);
    Window win = XtWindow((Widget) lw);

    if (lw->simple.international == True)	/* Multibyte string */
    {
	if (XtIsSensitive((Widget) lw))
	    XmbDrawString(disp, win, lw->label.fontset, lw->label.normal_GC, lw->label.label_x,
			  y, labelText, len);
	else {
	    XmbDrawString(disp, win, lw->label.fontset, lw->label.lgray_GC, lw->label.label_x,
			  y, labelText, len);
	    XmbDrawString(disp, win, lw->label.fontset, lw->label.gray_GC,
			  lw->label.label_x + 1,
			  y + 1, labelText, len);
	}
    } else {			/* 8 Bit character string */
	if (XtIsSensitive((Widget) lw))
	    XDrawString(disp, win, lw->label.normal_GC, lw->label.label_x,
			y, labelText, len);
	else {
	    XDrawString(disp, win, lw->label.lgray_GC, lw->label.label_x +
			1, y + 1, labelText, len);
	    XDrawString(disp, win, lw->label.gray_GC, lw->label.label_x, y,
			labelText, len);
	}
    }
}

/* ------------------------------------------------------------------ */

static void
Redisplay(
	     Widget w,
	     XEvent *event,
	     Region region)
{
    LabelWidget lw = (LabelWidget) w;
    GC gc = lw->label.normal_GC;
    int len = lw->label.label_len;
    char *label = lw->label.label;
    Position y = lw->label.label_y + lw->label.font->max_bounds.ascent;
    XFontSetExtents *ext;

    if (lw->label.pixmap == None) {
	/* Display left bitmap or pixmap */

	if (lw->label.left_bitmap && lw->label.lbm_width != 0) {
	    /* We only draw the left bitmap/pixmap if it is in the region to redisplay. */

	    if ((event == NULL) || (XRectInRegion(region, lw->label.lbm_x, lw->label.lbm_y,
						  lw->label.lbm_width,
				    lw->label.lbm_height) != RectangleOut)) {
		if (lw->label.label_depth == 1)
		    XCopyPlane(XtDisplay(w), lw->label.left_bitmap,
			       XtWindow(w), gc,
			       0, 0, lw->label.lbm_width, lw->label.lbm_height,
			       (int) lw->label.lbm_x,
			       (int) lw->label.internal_height + lw->label.lbm_y,
			       (unsigned long) 1L);
		else {
		    if (lw->label.left_clipMask != None) {
			XSetClipMask(XtDisplay(w), gc, lw->label.left_clipMask);
			XSetClipOrigin(XtDisplay(w), gc, lw->label.lbm_x,
				       lw->label.internal_height + lw->label.lbm_y);
		    }
		    XCopyArea(XtDisplay(w), lw->label.left_bitmap,
			      XtWindow(w), gc,
			      0, 0, lw->label.lbm_width, lw->label.lbm_height,
			      (int) lw->label.lbm_x,
			      (int) lw->label.internal_height + lw->label.lbm_y);
		    XSetClipMask(XtDisplay(w), gc, None);
		}
	    }
	}
	/* Draw label text if it is in the region to redisplay */

	if ((event == NULL) || (XRectInRegion(region, lw->label.label_x, lw->label.label_y,
					      lw->label.label_width,
				lw->label.label_height) != RectangleOut)) {
	    if (lw->label.encoding)	/* Label string with UCS2 code/UNICODE */
	    {
		char16 *wnl, *wlabel = (char16 *) label;

		if (len == MULTI_LINE_LABEL) {
		    while ((wnl = str16chr(wlabel, '\n')) != NULL) {
			len = (int) (wnl - wlabel);
			_DrawLabel16(lw, wlabel, (int) y, len);
			y += (Position) (lw->label.font->max_bounds.ascent + lw->label.font->max_bounds.descent);
			wlabel = wnl + 1;
		    }
		    len = str16len(wlabel);
		}
		if (len)
		    _DrawLabel16(lw, wlabel, (int) y, len);
	    } else {		/* Label string with 8 bit code or multibyte code */
		/* Special handling of multibyte code: Get infos about the font set */

		if (lw->simple.international == True) {
		    ext = XExtentsOfFontSet(lw->label.fontset);
		    y = (Position) (lw->label.label_y + abs(ext->max_ink_extent.y));
		    if (len == MULTI_LINE_LABEL) {
			char *nl;
			while ((nl = index(label, '\n')) != NULL) {
			    len = (int) (nl - label);
			    _DrawLabelString(lw, label, (int) y, len);
			    y += (Position) ext->max_ink_extent.height;
			    label = nl + 1;
			}
		    }
		} else {
		    if (len == MULTI_LINE_LABEL) {
			char *nl;
			while ((nl = index(label, '\n')) != NULL) {
			    len = (int) (nl - label);
			    _DrawLabelString(lw, label, (int) y, len);
			    y += (Position)
					     (lw->label.font->max_bounds.ascent
					     + lw->label.font->max_bounds.descent);
			    label = nl + 1;
			}
		    }
		}
		len = (int) strlen(label);
		if (len)
		    _DrawLabelString(lw, label, (int) y, len);
	    }
	}
    } else {			/* Label with only a bitmap or pixmap */
	if (lw->label.label_depth == 1)
	    XCopyPlane(XtDisplay(w), lw->label.pixmap, XtWindow(w), gc,
		       0, 0, lw->label.label_width, lw->label.label_height,
		       lw->label.label_x, lw->label.label_y, 1L);
	else {
	    if (lw->label.clipMask != None) {
		XSetClipMask(XtDisplay(w), gc, lw->label.clipMask);
		XSetClipOrigin(XtDisplay(w), gc, lw->label.label_x, lw->label.label_y);

	    }
	    XCopyArea(XtDisplay(w), lw->label.pixmap, XtWindow(w), gc,
		      0, 0, lw->label.label_width, lw->label.label_height,
		      lw->label.label_x, lw->label.label_y);
	    XSetClipMask(XtDisplay(w), gc, None);
	}
    }
}

/***********************************************************************
 *
 * TruncateLabelString: Truncate the label string, if it is too
 *			long for the widgets window.
 *
 ***********************************************************************/

/* DoTruncate8(): Truncate the 8 bit coded label string.
 * This function does not free the memory used by the label string.
 */
static void
DoTruncate8(
	       LabelWidget lw,
	       int usableWidth)
{
    int newlen, fullLen, currentWidth;
    static char *dots = "..";
    char *p;

    fullLen = (int) strlen(lw->label.label_full_len);

    /* Handling for a fixed width font: Calculate the string width in pixel */

    if (lw->label.font->max_bounds.width == lw->label.font->min_bounds.width)
	newlen = usableWidth / lw->label.font->max_bounds.width;
    else {
	/* Handling for a proportional font:
	 * Calculate character by character how long the string have to be.
	 */
	currentWidth = XTextWidth(lw->label.font, dots, 2);
	newlen = 0;

	if (lw->label.truncLeftSide) {	/* Truncate from the left side */
	    p = &lw->label.label_full_len[fullLen];
	    while ((currentWidth < usableWidth) && (newlen < fullLen)) {
		newlen++;
		p--;
		currentWidth += XTextWidth(lw->label.font, p, 1);
	    }
	    newlen++;		/* for the dots, otherwise we have to decrement newlen */
	} else {		/* Truncate from the right side */
	    p = lw->label.label_full_len;
	    while ((currentWidth < usableWidth) && (newlen < fullLen)) {
		newlen++;
		currentWidth += XTextWidth(lw->label.font, p, 1);
		p++;
	    }
	    newlen++;		/* ... for the dots */
	}
    }
    /* Check if our truncate mechanism works with the
     * given label width. Otherwise we only copy two dots.
     */
    if (newlen > 3) {
	lw->label.label = XtMalloc((Cardinal) (newlen + 1));
	if (lw->label.truncLeftSide) {
	    strcpy(lw->label.label, dots);
	    strcat(lw->label.label, &lw->label.label_full_len[fullLen -
							      newlen + 2]);
	} else {
	    strncpy(lw->label.label, lw->label.label_full_len, (size_t)
		    (newlen - 2));
	    lw->label.label[newlen - 2] = '\0';
	    strcat(lw->label.label, dots);
	}
	lw->label.label_len = (Dimension) newlen;
    } else {			/* The label is too small for the truncate mechanism */
	lw->label.label = XtMalloc(3);
	strcpy(lw->label.label, dots);
    }
}

/* DoTruncate16(): Truncate the label string in UNICODE/UCS2 code.
 * This function does not free the memory used by the label string.
 */
static void
DoTruncate16(
		LabelWidget lw,
		int usableWidth)
{
    int newlen, fullLen, currentWidth;
    static char *dots = "..";
    char16 *p, *wLabel, *dots16;

    dots16 = UTF8toUCS2(dots);
    fullLen = str16len((char16 *) lw->label.label_full_len);

    /* Handling for a fixed width font: Calculate the string width in pixel */

    if (lw->label.font->max_bounds.width == lw->label.font->min_bounds.width)
	newlen = usableWidth / lw->label.font->max_bounds.width;
    else {
	/* Handling for a proportional font:
	 * Calculate character by character how long the string have to be.
	 */
	currentWidth = XTextWidth16(lw->label.font, (XChar2b *) dots16, 2);
	newlen = 0;

	if (lw->label.truncLeftSide) {	/* Truncate from the left side */
	    wLabel = (char16 *) lw->label.label_full_len;
	    p = &wLabel[fullLen];
	    while ((currentWidth < usableWidth) && (newlen < fullLen)) {
		newlen++;
		p--;
		currentWidth += XTextWidth16(lw->label.font, (XChar2b *) p, 1);
	    }
	    newlen++;		/* for the dots, otherwise we have to decrement newlen */
	} else {		/* Truncate from the right side */
	    p = (char16 *) lw->label.label_full_len;
	    while ((currentWidth < usableWidth) && (newlen < fullLen)) {
		newlen++;
		currentWidth += XTextWidth16(lw->label.font, (XChar2b *) p, 1);
		p++;
	    }
	    newlen++;		/* ... for the dots */
	}
    }
    /* Check if our truncate mechanism works with the
     * given label width. Otherwise we only copy two dots.
     */
    if (newlen > 3) {
	lw->label.label = XtMalloc((Cardinal) (sizeof(char16) * (size_t)
					       (newlen + 1)));
	if (lw->label.truncLeftSide) {
	    str16cpy((char16 *) lw->label.label, dots16);
	    wLabel = (char16 *) lw->label.label_full_len;
	    str16cat((char16 *) lw->label.label, &wLabel[fullLen - newlen + 2]);
	} else {
	    wLabel = (char16 *) lw->label.label;
	    str16ncpy(wLabel, (char16 *) lw->label.label_full_len, (size_t)
		      (newlen - 2));
	    str16cat(wLabel, dots16);
	}
	lw->label.label_len = (Dimension) newlen;
    } else {			/* The label is too small for the truncate mechanism */
	lw->label.label = XtMalloc(3 * sizeof(char16));
	str16cpy((char16 *) lw->label.label, dots16);
    }
    XtFree((char *) dots16);
}

/* DoTruncateMulti(): Truncate the label string in multibyte code (UTF8).
 * This function does not free the memory used by the label string.
 */
static void
DoTruncateMulti(
		   LabelWidget lw,
		   int usableWidth)
{
    int newlen, noOfChars, fullLen, currentWidth, charLen;
    static char *dots = "..";	/* This is also a correct multibyte code */
    char *p;

    fullLen = (int) strlen(lw->label.label_full_len);

    /* Handling for all fonts:
     * Calculate character by character how long the string have to be.
     */
    newlen = noOfChars = 2;
    currentWidth = XmbTextEscapement(lw->label.fontset, dots, noOfChars);

    /* In international environments we always truncate from the right side */

    p = lw->label.label_full_len;
    while ((currentWidth < usableWidth) && (newlen < fullLen)) {
	charLen = mblen(p, strlen(p));
	currentWidth += XmbTextEscapement(lw->label.fontset, p, charLen);
	noOfChars++;
	newlen += charLen;
	p += charLen;
    }
    /* Skip back one character because the last step was one step too much */

    newlen -= charLen;
    noOfChars--;

    /* Check if our truncate mechanism works with the
     * given label width. Otherwise we only copy two dots.
     */
    if (noOfChars > 3) {
	lw->label.label = XtMalloc((Cardinal) (newlen + 1));
	strncpy(lw->label.label, lw->label.label_full_len, (size_t) (newlen
								     - 2));
	lw->label.label[newlen - 2] = '\0';
	strcat(lw->label.label, dots);
	lw->label.label_len = (Dimension) newlen;
    } else {			/* The label is too small for the truncate mechanism */
	lw->label.label = XtMalloc(3);
	strcpy(lw->label.label, dots);
    }
}

/* Free the resources allocated by the
 * truncate mechanism and reset the label length
 */
static void
ResetTruncateMode(LabelWidget lw)
{
    XtFree(lw->label.label);
    lw->label.label = lw->label.label_full_len;
    lw->label.label_full_len = NULL;
    if (lw->label.encoding)
	lw->label.label_len = (Dimension) str16len((char16 *) lw->label.label);
    else
	lw->label.label_len = (Dimension) strlen(lw->label.label);
}

/* Check if the label string have to be truncated to display
 * it in the label. We don't do this, if we have a bitmap or
 * a multiline label.
 * TruncateLabelString() will modify the label and the label_len
 * entries in the widget record.
 */
void
TruncateLabelString(
		       LabelWidget lw,
		       Dimension currentWidth)
{
    int usableWidth;

    if ((lw->label.pixmap == None) && (lw->label.label_len !=
	MULTI_LINE_LABEL) &&
	(lw->label.label != NULL)) {
	/* Label string will be displayed and is not multi line:
	 * Check if the string fits into the widget.
	 */
	usableWidth = currentWidth - 2 * lw->label.internal_width;
	if (lw->label.label_width > usableWidth) {
	    /* Widget size is to small to display the current label string.
	     * The label string will be truncated until it fits into the
	     * widget.
	     */
	    if (lw->label.label_full_len == NULL)
		lw->label.label_full_len = lw->label.label;
	    else
		XtFree(lw->label.label);

	    if (lw->label.encoding)
		DoTruncate16(lw, usableWidth);
	    else {
		if (lw->simple.international)
		    DoTruncateMulti(lw, usableWidth);
		else
		    DoTruncate8(lw, usableWidth);
	    }
	} else {		/* After resize the label string fits into the widget */
	    if (lw->label.label_full_len != NULL)
		ResetTruncateMode(lw);
	}
    }
}

/***********************************************************************
 *
 * Resize method:	Resize or reposition the label
 *
 ***********************************************************************/

/* Determine the new position of the label string
 * and the left bitmap or a pixmap after resize.
 */
static void
Reposition(
	      register LabelWidget lw,
	      Dimension width,
	      Dimension height)
{
    Position newPos;
    Position leftedge = (Position) (lw->label.internal_width + LEFT_OFFSET(lw));

    switch (lw->label.justify) {
    case XtJustifyLeft:
	newPos = leftedge;
	break;

    case XtJustifyRight:
	newPos = (Position) (width - (lw->label.label_width + lw->label.internal_width));
	break;

    case XtJustifyCenter:
    default:
	newPos = (Position) (width - lw->label.label_width /* + LEFT_OFFSET(lw) */
	    ) / 2;
	break;
    }
    if (newPos < (Position) leftedge)
	newPos = leftedge;
    lw->label.label_x = newPos;
    lw->label.label_y = (Position) ((int) (height - lw->label.label_height)
				    / 2);
}

static void
Resize(Widget w)
{
    LabelWidget lw = (LabelWidget) w;

    /* We only make a resize, if we are a Label widget.
     * If Label is only the superclass, we do nothing
     * because the derived widget class has its own
     * resize method.
     */
    if (strcmp(ownClassName, XtClass(w)->core_class.class_name) == 0) {
	Reposition(lw, w->core.width, w->core.height);
	if (lw->label.truncLabel)
	    TruncateLabelString(lw, (w->core.width - LEFT_OFFSET(lw)));
	compute_bitmap_offsets(lw, lw);
    }
}

/***********************************************************************
 *
 * Set values method:	Set specified arguments into widget
 *
 ***********************************************************************/

#define PIXMAP	 0
#define WIDTH	 1
#define HEIGHT	 2
#define NUM_CHECKS 3

static Boolean
SetValues(
	     Widget current,
	     Widget request,
	     Widget new,
	     ArgList args,
	     Cardinal *num_args)
{
    LabelWidget curlw = (LabelWidget) current;
    LabelWidget reqlw = (LabelWidget) request;
    LabelWidget newlw = (LabelWidget) new;
    int i;
    Boolean was_resized = False, redisplay = False, checks[NUM_CHECKS];

    for (i = 0; i < NUM_CHECKS; i++)
	checks[i] = FALSE;

    for (i = 0; (Cardinal) i < *num_args; i++) {
	if (streq(XtNbitmap, args[i].name) || streq(XtNpixmap, args[i].name) ||
	    streq(XtNclipMask, args[i].name))
	    checks[PIXMAP] = TRUE;
	if (streq(XtNwidth, args[i].name))
	    checks[WIDTH] = TRUE;
	if (streq(XtNheight, args[i].name))
	    checks[HEIGHT] = TRUE;
    }
    if (newlw->label.label == NULL)
	newlw->label.label = DeConst(newlw->core.name);

    /*
     * resize on bitmap change or change of font style
     */
    if ((curlw->label.left_bitmap != newlw->label.left_bitmap) ||
	(curlw->label.left_clipMask != newlw->label.left_clipMask) ||
	(curlw->label.encoding != newlw->label.encoding) ||
	(curlw->simple.international && (curlw->label.fontset != newlw->label.fontset)))
	was_resized = True;

    if (curlw->label.label != newlw->label.label) {
	if (curlw->label.label != curlw->core.name)
	    XtFree((char *) curlw->label.label);
	if (newlw->label.label != newlw->core.name) {
	    if (newlw->label.encoding)
		newlw->label.label = (char *) UTF8toUCS2(newlw->label.label);
	    else
		newlw->label.label = XtNewString(newlw->label.label);
	}
	if (newlw->label.label_full_len)
	    XtFree(newlw->label.label_full_len);
	newlw->label.label_full_len = NULL;
	was_resized = True;
    }

    if (was_resized || (curlw->label.font != newlw->label.font) ||
	(curlw->label.justify != newlw->label.justify) || checks[PIXMAP]) {
	if (newlw->label.label_full_len)
	    ResetTruncateMode(newlw);
	SetTextWidthAndHeight(newlw);
	was_resized = True;
    }
    /* recalculate the window size if something has changed. */

    if (newlw->label.resize && was_resized) {
	if ((curlw->core.height == reqlw->core.height) && !checks[HEIGHT])
	    newlw->core.height = (Dimension) (newlw->label.label_height + 2
					      * newlw->label.internal_height);

	set_bitmap_info(curlw, newlw);

	if ((curlw->core.width == reqlw->core.width) && !checks[WIDTH])
	    newlw->core.width = (Dimension) (newlw->label.label_width +
					     LEFT_OFFSET(newlw) +
					     2 * newlw->label.internal_width);
    }

    if (curlw->label.foreground != newlw->label.foreground
	|| curlw->core.background_pixel != newlw->core.background_pixel
	|| curlw->simple.shadowColor != newlw->simple.shadowColor
	|| curlw->simple.highlightColor != newlw->simple.highlightColor
	|| curlw->label.font->fid != newlw->label.font->fid) {

	XtReleaseGC(new, curlw->label.normal_GC);
	XtReleaseGC(new, curlw->label.gray_GC);
	XtReleaseGC(new, curlw->label.lgray_GC);
	GetnormalGC(newlw);
	GetgrayGCs(newlw);
	redisplay = True;
    }
    /* Truncate machanism activated or deactivated ? */

    if (curlw->label.truncLabel != newlw->label.truncLabel) {
	/* Truncate mode deactivated: If label is currently truncated,
	 * release truncated string and reset the original label.
	 */
	if (newlw->label.truncLabel == False) {
	    if (newlw->label.label_full_len) {
		ResetTruncateMode(newlw);
		redisplay = True;
	    }
	}
	/* Truncate machanism activated: No resize but resize handling required */

	else
	    was_resized = True;
    }

    if ((curlw->label.truncLeftSide != newlw->label.truncLeftSide) &&
	(newlw->label.label_full_len != NULL)) {
	/* Mode of truncate mechanism changed and label string is truncated:
	 * Recalculation of the string is required.
	 */
	ResetTruncateMode(newlw);
	was_resized = True;
    }

    if ((curlw->label.internal_width != newlw->label.internal_width) ||
	(curlw->label.internal_height != newlw->label.internal_height) || was_resized) {

	/* Resize() will be called if geometry changes succeed */

	if (strcmp(ownClassName, XtClass(new)->core_class.class_name) == 0) {
	    Reposition(newlw, curlw->core.width, curlw->core.height);
	    compute_bitmap_offsets(curlw, newlw);
	    if (newlw->label.truncLabel)
		TruncateLabelString(newlw, (Dimension) (curlw->core.width -
							LEFT_OFFSET(newlw)));
	}
    }

    return was_resized || redisplay ||
	XtIsSensitive(current) != XtIsSensitive(new);
}

/***********************************************************************
 *
 * Destroy method:	Release all used resources
 *
 ***********************************************************************/

static void
Destroy(Widget w)
{
    LabelWidget lw = (LabelWidget) w;

    XtFree(lw->label.label);
    if (lw->label.label_full_len != NULL)
	XtFree(lw->label.label_full_len);
    XtReleaseGC(w, lw->label.normal_GC);
    XtReleaseGC(w, lw->label.gray_GC);
    XtReleaseGC(w, lw->label.lgray_GC);
}

/***********************************************************************
 *
 * QueryGeometry()
 *
 ***********************************************************************/

static XtGeometryResult
QueryGeometry(
		 Widget w,
		 XtWidgetGeometry * intended,
		 XtWidgetGeometry * preferred)
{
    register LabelWidget lw = (LabelWidget) w;

    preferred->request_mode = CWWidth | CWHeight;
    preferred->width = (lw->label.label_width + (2 *
			lw->label.internal_width) +
			LEFT_OFFSET(lw));
    preferred->height = (Dimension) (lw->label.label_height + 2 * lw->label.internal_height);
    if (((intended->request_mode & (CWWidth | CWHeight))
	 == (CWWidth | CWHeight)) &&
	intended->width == preferred->width &&
	intended->height == preferred->height)
	return XtGeometryYes;
    else if (preferred->width == w->core.width &&
	     preferred->height == w->core.height)
	return XtGeometryNo;
    else
	return XtGeometryAlmost;
}
