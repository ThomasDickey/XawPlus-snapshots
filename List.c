/*
 * $XTermId: List.c,v 1.10 2025/01/19 14:28:15 tom Exp $
 * $Xorg: List.c,v 1.4 2001/02/09 02:03:44 xorgcvs Exp $
 */

/*
Copyright 2015-2024,2025  Thomas E. Dickey
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
 * List.c - List widget
 *
 * This is a List widget.  It allows the user to select an item in a list and
 * notifies the application through a callback function.
 *
 *	Created: 	8/13/88
 *	By:		Chris D. Peterson
 *                      MIT X Consortium
 *
 * This file contains modifications for XawPlus, Roland Krause 2002
 */

#include "private.h"

#include <ctype.h>

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/Xmu/Drawing.h>

#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/ListP.h>
/* *INDENT-OFF* */
/* These added so widget knows whether its height, width are user selected.
I also added the freedoms member of the list widget part. */

#define HeightLock  1
#define WidthLock   2
#define LongestLock 4

#define HeightFree( w )  !(((ListWidget)(w))->list.freedoms & HeightLock )
#define WidthFree( w )   !(((ListWidget)(w))->list.freedoms & WidthLock )
#define LongestFree( w ) !(((ListWidget)(w))->list.freedoms & LongestLock )

/*
 * Default Translation table.
 */

static char defaultTranslations[] =
  "<Btn1Down>:   Set()\n\
   <Btn1Up>:     Notify()";

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

#define offset(field) XtOffset(ListWidget, field)

static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	offset(list.foreground), XtRString, XtDefaultForeground},
    {XtNcursor, XtCCursor, XtRCursor, sizeof(Cursor),
       offset(simple.cursor), XtRString, "left_ptr"},
    {XtNfont,  XtCFont, XtRFontStruct, sizeof(XFontStruct *),
	offset(list.font),XtRString, XtDefaultFont},
    {XtNfontSet,  XtCFontSet, XtRFontSet, sizeof(XFontSet ),
	offset(list.fontset),XtRString, XtDefaultFontSet},
    {XtNlist, XtCList, XtRPointer, sizeof(char **),
       offset(list.list), XtRString, NULL},
    {XtNiconList, XtCList, XtRPointer, sizeof(XawIconList *),
       offset(list.iconList), XtRPointer, NULL},
    {XtNiconWidth, XtCWidth, XtRDimension,  sizeof(Dimension),
	offset(list.icon_width), XtRImmediate, (XtPointer)0},
    {XtNiconHeight, XtCHeight, XtRDimension, sizeof(Dimension),
	offset(list.icon_height), XtRImmediate, (XtPointer)0},
    {XtNiconDepth, XtCDepth, XtRDimension, sizeof(Dimension),
	offset(list.icon_depth), XtRImmediate, (XtPointer)1},
    {XtNdefaultColumns, XtCColumns, XtRInt,  sizeof(int),
	offset(list.default_cols), XtRImmediate, (XtPointer)2},
    {XtNlongest, XtCLongest, XtRInt,  sizeof(int),
	offset(list.longest), XtRImmediate, (XtPointer)0},
    {XtNnumberStrings, XtCNumberStrings, XtRInt,  sizeof(int),
	offset(list.nitems), XtRImmediate, (XtPointer)0},
    {XtNpasteBuffer, XtCBoolean, XtRBoolean,  sizeof(Boolean),
	offset(list.paste), XtRImmediate, (XtPointer) False},
    {XtNforceColumns, XtCColumns, XtRBoolean,  sizeof(Boolean),
	offset(list.force_cols), XtRImmediate, (XtPointer) False},
    {XtNverticalList, XtCBoolean, XtRBoolean,  sizeof(Boolean),
	offset(list.vertical_cols), XtRImmediate, (XtPointer) False},
    {XtNinternalWidth, XtCWidth, XtRDimension,  sizeof(Dimension),
	offset(list.internal_width), XtRImmediate, (XtPointer)4},
    {XtNinternalHeight, XtCHeight, XtRDimension, sizeof(Dimension),
	offset(list.internal_height), XtRImmediate, (XtPointer)2},
    {XtNcolumnSpacing, XtCSpacing, XtRDimension,  sizeof(Dimension),
	offset(list.column_space), XtRImmediate, (XtPointer)6},
    {XtNrowSpacing, XtCSpacing, XtRDimension,  sizeof(Dimension),
	offset(list.row_space), XtRImmediate, (XtPointer)2},
    {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer),
        offset(list.callback), XtRCallback, NULL},
};

static void Initialize(Widget junk, Widget new, ArgList args, Cardinal *num_args);
static void ChangeSize(Widget w, Dimension width, Dimension height);
static void Resize(Widget w);
static void Redisplay(Widget w, XEvent *event, Region junk);
static void Destroy(Widget w);
static Boolean Layout(Widget w, Boolean xfree, Boolean yfree, Dimension *width, Dimension *height);
static XtGeometryResult PreferredGeom(Widget w, XtWidgetGeometry *intended, XtWidgetGeometry *requested);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Notify(Widget w, XEvent * event, String * params, Cardinal *num_params);
static void Set(Widget w, XEvent * event, String * params, Cardinal *num_params);
static void Unset(Widget w, XEvent * event, String * params, Cardinal *num_params);

static XtActionsRec actions[] = {
      {"Notify",         Notify},
      {"Set",            Set},
      {"Unset",          Unset},
};

ListClassRec listClassRec = {
  {
/* core_class fields */
    /* superclass	  	*/	(WidgetClass) &simpleClassRec,
    /* class_name	  	*/	"List",
    /* widget_size	  	*/	sizeof(ListRec),
    /* class_initialize   	*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize		  	*/	XtInheritRealize,
    /* actions		  	*/	actions,
    /* num_actions	  	*/	XtNumber(actions),
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
    /* tm_table		   	*/	defaultTranslations,
    /* query_geometry		*/      PreferredGeom,
    /* display_accelerator	*/	NULL,
    /* extension		*/	NULL
  },
/* Simple class fields initialization */
  {
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
/* List class fields initialization */
  {
    /* not used			*/	0
  },
};
/* *INDENT-ON* */

WidgetClass listWidgetClass = (WidgetClass) & listClassRec;
WidgetClass iconListWidgetClass = (WidgetClass) & listClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void
GetGCs(Widget w)
{
    XGCValues values;
    ListWidget lw = (ListWidget) w;

    values.foreground = lw->list.foreground;
    values.background = lw->core.background_pixel;
    values.font = lw->list.font->fid;

    if (lw->simple.international == True) {
	lw->list.normgc = XtAllocateGC(w, 0, GCForeground | GCBackground,
				       &values, GCFont, 0);
	lw->list.iconGc = XtAllocateGC(w, 0, GCForeground | GCBackground,
				       &values, 0, 0);
    } else {
	lw->list.normgc = XtGetGC(w, GCForeground | GCBackground | GCFont, &values);
	lw->list.iconGc = XtGetGC(w, GCForeground | GCBackground, &values);
    }

    values.foreground = lw->core.background_pixel;
    values.background = lw->list.foreground;

    if (lw->simple.international == True)
	lw->list.revgc = XtAllocateGC(w, 0, GCForeground | GCBackground,
				      &values, GCFont, 0);
    else
	lw->list.revgc = XtGetGC(w, GCForeground | GCFont | GCBackground, &values);

    values.tile = XmuCreateStippledPixmap(XtScreen(w),
					  lw->list.foreground,
					  lw->core.background_pixel,
					  lw->core.depth);
    values.fill_style = FillTiled;

    if (lw->simple.international == True)
	lw->list.graygc = XtAllocateGC(w, 0, GCTile | GCFillStyle, &values,
				       GCFont, 0);
    else
	lw->list.graygc = XtGetGC(w, GCFont | GCTile | GCFillStyle, &values);
}

/* CalculatedValues()
 *
 * does routine checks/computations that must be done after data changes
 * but won't hurt if accidently called
 *
 * These calculations were needed in SetValues.  They were in ResetList.
 * ResetList called ChangeSize, which made an XtGeometryRequest.  You
 * MAY NOT change your geometry from within a SetValues. (Xt man,
 * sect. 9.7.2)  So, I factored these changes out. */

static void
CalculatedValues(Widget w)
{
    int i, len;

    ListWidget lw = (ListWidget) w;

    /* If both lists are NULL then the list will just be the name of the widget. */

    if ((lw->list.list == NULL) && (lw->list.iconList == NULL)) {
	lw->list.list = &(lw->core.name);
	lw->list.nitems = 1;
    }

    /* Get the number of items. One list is always defined here ! */

    if (lw->list.iconList) {
	/* If an icon list is available we use it with a
	 * higher priority as the text list.
	 */
	if (lw->list.nitems == 0)
	    for (; lw->list.iconList[lw->list.nitems].string != NULL;
		 lw->list.nitems++) ;
    } else if ((lw->list.list) && (lw->list.nitems == 0))
	for (; lw->list.list[lw->list.nitems] != NULL; lw->list.nitems++) ;

    /* Get column width. */

    if (LongestFree(lw)) {
	lw->list.longest = 0;	/* so it will accumulate real longest below */

	if (lw->list.iconList) {
	    for (i = 0; i < lw->list.nitems; i++) {
		if (lw->simple.international == True)
		    len = XmbTextEscapement(lw->list.fontset,
					    lw->list.iconList[i].string,
					    (int)
							 strlen(lw->list.iconList[i].string));
		else
		    len = XTextWidth(lw->list.font, lw->list.iconList[i].string,
				     (int) strlen(lw->list.iconList[i].string));
		if (len > lw->list.longest)
		    lw->list.longest = len;
	    }
	} else {
	    for (i = 0; i < lw->list.nitems; i++) {
		if (lw->simple.international == True)
		    len = XmbTextEscapement(lw->list.fontset,
					    lw->list.list[i], (int)
									   strlen(lw->list.list[i]));
		else
		    len = XTextWidth(lw->list.font, lw->list.list[i], (int)
				     strlen(lw->list.list[i]));
		if (len > lw->list.longest)
		    lw->list.longest = len;
	    }
	}
    }
    lw->list.col_width = lw->list.longest + lw->list.column_space;
    if (lw->list.iconList)
	lw->list.col_width += lw->list.icon_width;
}

/*	Function Name: ResetList
 *	Description: Resets the new list when important things change.
 *	Arguments: w - the widget.
 *                 changex, changey - allow the height or width to change?
 *
 *	Returns: TRUE if width or height have been changed
 */

static void
ResetList(
	     Widget w,
	     Boolean changex,
	     Boolean changey)
{
    Dimension width = w->core.width;
    Dimension height = w->core.height;

    CalculatedValues(w);

    if (Layout(w, changex, changey, &width, &height))
	ChangeSize(w, width, height);
}

/*	Function Name: ChangeSize.
 *	Description: Laysout the widget.
 *	Arguments: w - the widget to try change the size of.
 *	Returns: none.
 */

static void
ChangeSize(
	      Widget w,
	      Dimension width,
	      Dimension height)
{
    XtWidgetGeometry request, reply;

    request.request_mode = CWWidth | CWHeight;
    request.width = width;
    request.height = height;

    switch (XtMakeGeometryRequest(w, &request, &reply)) {
    case XtGeometryYes:
	break;
    case XtGeometryNo:
	break;
    case XtGeometryAlmost:
	Layout(w, (request.height != reply.height),
	       (request.width != reply.width),
	       &(reply.width), &(reply.height));
	request = reply;
	switch (XtMakeGeometryRequest(w, &request, &reply)) {
	case XtGeometryYes:
	case XtGeometryNo:
	    break;
	case XtGeometryAlmost:
	    request = reply;
	    Layout(w, FALSE, FALSE, &(request.width), &(request.height));
	    request.request_mode = CWWidth | CWHeight;
	    XtMakeGeometryRequest(w, &request, &reply);
	    break;
	case XtGeometryDone:
	default:
	    XtAppWarning(XtWidgetToApplicationContext(w),
			 "List Widget: Unknown geometry return.");
	    break;
	}
	break;
    case XtGeometryDone:
    default:
	XtAppWarning(XtWidgetToApplicationContext(w),
		     "List Widget: Unknown geometry return.");
	break;
    }
}

/*	Function Name: Initialize
 *	Description: Function that initilizes the widget instance.
 *	Arguments: junk - NOT USED.
 *                 new  - the new widget.
 *	Returns: none
 */

static void
Initialize(
	      Widget junk GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    ListWidget lw = (ListWidget) new;

/*
 * Initialize all private resources.
 */

    /* record for posterity if we are free */
    lw->list.freedoms = (lw->core.width != 0) * WidthLock +
	(lw->core.height != 0) * HeightLock +
	(lw->list.longest != 0) * LongestLock;

    GetGCs(new);

    /* Set row height. based on font or fontset */

    if (lw->simple.international == True)
	lw->list.row_height = XExtentsOfFontSet(lw->list.fontset)->max_ink_extent.height;
    else
	lw->list.row_height = lw->list.font->max_bounds.ascent + lw->list.font->max_bounds.descent;

    if (lw->list.iconList && (lw->list.row_height < lw->list.icon_height))
	lw->list.row_height = lw->list.icon_height + lw->list.row_space;
    else
	lw->list.row_height += lw->list.row_space;

    ResetList(new, WidthFree(lw), HeightFree(lw));
    lw->list.highlight = lw->list.is_highlighted = NO_HIGHLIGHT;

}				/* Initialize */

/*	Function Name: CvtToItem
 *	Description: Converts Xcoord to item number of item containing that
 *                   point.
 *	Arguments: w - the list widget.
 *                 xloc, yloc - x location, and y location.
 *	Returns: the item number.
 */

static int
CvtToItem(
	     Widget w,
	     int xloc,
	     int yloc,
	     int *item)
{
    int one, another;
    ListWidget lw = (ListWidget) w;
    int ret_val = OKAY;

    if (lw->list.vertical_cols) {
	one = lw->list.nrows * ((xloc - (int) lw->list.internal_width)
				/ lw->list.col_width);
	another = (yloc - (int) lw->list.internal_height)
	    / lw->list.row_height;
	/* If out of range, return minimum possible value. */
	if (another >= lw->list.nrows) {
	    another = lw->list.nrows - 1;
	    ret_val = OUT_OF_RANGE;
	}
    } else {
	one = (lw->list.ncols * ((yloc - (int) lw->list.internal_height)
				 / lw->list.row_height));
	/* If in right margin handle things right. */
	another = (xloc - (int) lw->list.internal_width) / lw->list.col_width;
	if (another >= lw->list.ncols) {
	    another = lw->list.ncols - 1;
	    ret_val = OUT_OF_RANGE;
	}
    }
    if ((xloc < 0) || (yloc < 0))
	ret_val = OUT_OF_RANGE;
    if (one < 0)
	one = 0;
    if (another < 0)
	another = 0;
    *item = one + another;
    if (*item >= lw->list.nitems)
	return (OUT_OF_RANGE);
    return (ret_val);
}

/*	Function Name: FindCornerItems.
 *	Description: Find the corners of the rectangle in item space.
 *	Arguments: w - the list widget.
 *                 event - the event structure that has the rectangle it it.
 *                 ul_ret, lr_ret - the corners ** RETURNED **.
 *	Returns: none.
 */

static void
FindCornerItems(
		   Widget w,
		   XEvent *event,
		   int *ul_ret,
		   int *lr_ret)
{
    int xloc, yloc;

    xloc = event->xexpose.x;
    yloc = event->xexpose.y;
    CvtToItem(w, xloc, yloc, ul_ret);
    xloc += event->xexpose.width;
    yloc += event->xexpose.height;
    CvtToItem(w, xloc, yloc, lr_ret);
}

/*	Function Name: ItemInRectangle
 *	Description: returns TRUE if the item passed is in the given rectangle.
 *	Arguments: w - the list widget.
 *                 ul, lr - corners of the rectangle in item space.
 *                 item - item to check.
 *	Returns: TRUE if the item passed is in the given rectangle.
 */

static Boolean
ItemInRectangle(
		   Widget w,
		   int ul,
		   int lr,
		   int item)
{
    ListWidget lw = (ListWidget) w;
    int mod_item;
    int things;

    if (item < ul || item > lr)
	return (FALSE);
    if (lw->list.vertical_cols)
	things = lw->list.nrows;
    else
	things = lw->list.ncols;

    mod_item = item % things;
    if ((mod_item >= ul % things) && (mod_item <= lr % things))
	return (TRUE);
    return (FALSE);
}

/*	Function Name: HighlightBackground
 *	Description: paints the color of the background for the given item.
 *	Arguments: w - the widget.
 *                 x, y - ul corner of the area item occupies.
 *                 gc - the gc that is used to paint this rectangle
 *	Returns:
 */

static void
HighlightBackground(
		       Widget w,
		       int x,
		       int y,
		       GC gc)
{
    ListWidget lw = (ListWidget) w;
    int hl_x, hl_y, width;

    hl_x = x - lw->list.column_space / 2;
    width = lw->list.col_width + lw->list.column_space / 2;
    hl_y = y - lw->list.row_space / 2;

    XFillRectangle(XtDisplay(w), XtWindow(w), gc, hl_x, hl_y, (unsigned)
		   width, (unsigned) lw->list.row_height);
}

/*  PaintItemName()
 *
 *  paints the name of the item in the appropriate location.
 *  w - the list widget.
 *  item - the item to draw.
 *
 *  NOTE: no action taken on an unrealized widget. */

static void
PaintItemName(
		 Widget w,
		 int item)
{
    char *str;
    GC gc;
    int x, y, str_x, str_y, h_x, delta;
    ListWidget lw = (ListWidget) w;
    XFontSetExtents *ext = XExtentsOfFontSet(lw->list.fontset);

    if (!XtIsRealized(w))
	return;			/* Just in case... */

    if (lw->list.vertical_cols) {
	x = (lw->list.col_width + lw->list.internal_width) * (item / lw->list.nrows);
	y = lw->list.row_height * (item % lw->list.nrows) + lw->list.internal_height;
    } else {
	x = (lw->list.col_width + lw->list.internal_width) * (item % lw->list.ncols);
	y = lw->list.row_height * (item / lw->list.ncols) + lw->list.internal_height;
    }
    /* Determine the string and the icon position */

    if (lw->list.iconList)
	str_x = x + lw->list.icon_width + lw->list.internal_width;
    else
	str_x = x;
    h_x = x + lw->list.internal_width;

    if (lw->simple.international) {
	delta = (lw->list.row_height - ext->max_ink_extent.height) / 2 - lw->list.internal_height;
	str_y = y + abs(ext->max_ink_extent.y) + delta;
    } else {
	delta = (lw->list.row_height - lw->list.font->max_bounds.ascent -
		 lw->list.font->max_bounds.descent) / 2 - lw->list.internal_height;
	str_y = y + lw->list.font->max_bounds.ascent + delta;
    }
    /* Paint or erase the highlighted background */

    if (item == lw->list.is_highlighted) {
	if (item == lw->list.highlight) {
	    gc = lw->list.revgc;
	    HighlightBackground(w, h_x, y, lw->list.normgc);
	} else {
	    if (XtIsSensitive(w))
		gc = lw->list.normgc;
	    else
		gc = lw->list.graygc;
	    HighlightBackground(w, h_x, y, lw->list.revgc);
	    lw->list.is_highlighted = NO_HIGHLIGHT;
	}
    } else {
	if (item == lw->list.highlight) {
	    gc = lw->list.revgc;
	    HighlightBackground(w, h_x, y, lw->list.normgc);
	    lw->list.is_highlighted = item;
	} else {
	    if (XtIsSensitive(w))
		gc = lw->list.normgc;
	    else
		gc = lw->list.graygc;
	}
    }
    /*  Paint the icon if there is one. The icon will only be painted, if
       width and height are greater than 0 and if an icon is defined for
       this entry. The depth of the icon has to be 1 (normal bitmap)
       or of the same color depth as your display */

    if ((lw->list.iconList) &&
	(lw->list.icon_width > 0) && (lw->list.icon_height > 0)) {
	x += lw->list.column_space / 2;
	if (lw->list.iconList[item].bitmap != XtUnspecifiedPixmap)
	    if (lw->list.icon_depth == 1)
		XCopyPlane(XtDisplay(w), lw->list.iconList[item].bitmap,
			   XtWindow(w),
			   lw->list.normgc, 0, 0, lw->list.icon_width,
			   lw->list.icon_height, x, y, 1L);
	    else if (lw->list.iconList[item].clipMask != XtUnspecifiedPixmap) {
		/* Support of a clip mask for non rectangular or transparent
		 * pixmaps: Manipulate the GC with a clip mask
		 */
		XSetClipMask(XtDisplay(w), lw->list.iconGc,
			     lw->list.iconList[item].clipMask);
		XSetClipOrigin(XtDisplay(w), lw->list.iconGc, x, y);
		XCopyArea(XtDisplay(w), lw->list.iconList[item].bitmap,
			  XtWindow(w),
			  lw->list.iconGc, 0, 0, lw->list.icon_width,
			  lw->list.icon_height, x, y);
	    } else
		XCopyArea(XtDisplay(w), lw->list.iconList[item].bitmap,
			  XtWindow(w),
			  lw->list.normgc, 0, 0, lw->list.icon_width,
			  lw->list.icon_height, x, y);
	else
	    XClearArea(XtDisplay(w), XtWindow(w), x, y,
		       lw->list.icon_width, lw->list.icon_height, FALSE);
    }
    /* List's overall width contains the same number of inter-column
       column_space's as columns.  There should thus be a half
       column_width margin on each side of each column.
       The row case is symmetric. */

    str_x += lw->list.column_space / 2;
    str_y += lw->list.row_space / 2;

    /* Draw the String */

    if (lw->list.iconList)
	str = DeConst(lw->list.iconList[item].string);
    else
	str = DeConst(lw->list.list[item]);

    if (lw->simple.international)
	XmbDrawString(XtDisplay(w), XtWindow(w), lw->list.fontset,
		      gc, str_x, str_y, str, (int) strlen(str));
    else
	XDrawString(XtDisplay(w), XtWindow(w), gc, str_x, str_y, str, (int)
		    strlen(str));
}

/* Redisplay()
 *
 * Repaints the widget window on expose events.
 * w - the list widget.
 * event - the expose event for this repaint.
 * junk - not used, unless three-d patch enabled. */

/* ARGSUSED */
static void
Redisplay(
	     Widget w,
	     XEvent *event,
	     Region junk GCC_UNUSED)
{
    int item;			/* an item to work with. */
    int ul_item, lr_item;	/* corners of items we need to paint. */
    ListWidget lw = (ListWidget) w;

    if (event == NULL) {	/* repaint all. */
	ul_item = 0;
	lr_item = lw->list.nrows * lw->list.ncols - 1;
	XClearWindow(XtDisplay(w), XtWindow(w));
    } else
	FindCornerItems(w, event, &ul_item, &lr_item);

    for (item = ul_item; (item <= lr_item && item < lw->list.nitems); item++)
	if (ItemInRectangle(w, ul_item, lr_item, item))
	    PaintItemName(w, item);
}

/* PreferredGeom()
 *
 * This tells the parent what size we would like to be
 * given certain constraints.
 * w - the widget.
 * intended - what the parent intends to do with us.
 * requested - what we want to happen. */

static XtGeometryResult
PreferredGeom(
		 Widget w,
		 XtWidgetGeometry * intended,
		 XtWidgetGeometry * requested)
{
    Dimension new_width, new_height;
    Boolean change, width_req, height_req;

    width_req = intended->request_mode & CWWidth;
    height_req = intended->request_mode & CWHeight;

    if (width_req)
	new_width = intended->width;
    else
	new_width = w->core.width;

    if (height_req)
	new_height = intended->height;
    else
	new_height = w->core.height;

    requested->request_mode = 0;

/*
 * We only care about our height and width.
 */

    if (!width_req && !height_req)
	return (XtGeometryYes);

    change = Layout(w, !width_req, !height_req, &new_width, &new_height);

    requested->request_mode |= CWWidth;
    requested->width = new_width;
    requested->request_mode |= CWHeight;
    requested->height = new_height;

    if (change)
	return (XtGeometryAlmost);
    return (XtGeometryYes);
}

/* Resize()
 *
 * resizes the widget, by changing the number of rows and columns. */

static void
Resize(Widget w)
{
    Dimension width, height;

    width = w->core.width;
    height = w->core.height;

    if (Layout(w, FALSE, FALSE, &width, &height))
	XtAppWarning(XtWidgetToApplicationContext(w),
		     "List Widget: Size changed when it shouldn't have when resising.");
}

/* Layout()
 *
 * lays out the item in the list.
 * w - the widget.
 * xfree, yfree - TRUE if we are free to resize the widget in
 *                this direction.
 * width, height- the is the current width and height that we are going
 *                we are going to layout the list widget to,
 *                depending on xfree and yfree of course.
 *
 * RETURNS: TRUE if width or height have been changed. */

static Boolean
Layout(
	  Widget w,
	  Boolean xfree,
	  Boolean yfree,
	  Dimension *width,
	  Dimension *height)
{
    ListWidget lw = (ListWidget) w;
    Boolean change = FALSE;

/*
 * If force columns is set then always use number of columns specified
 * by default_cols.
 */

    if (lw->list.force_cols) {
	lw->list.ncols = lw->list.default_cols;
	if (lw->list.ncols <= 0)
	    lw->list.ncols = 1;
	/* 12/3 = 4 and 10/3 = 4, but 9/3 = 3 */
	lw->list.nrows = ((lw->list.nitems - 1) / lw->list.ncols) + 1;
	if (xfree) {		/* If allowed resize width. */

	    /* this counts the same number
	       of inter-column column_space 's as columns.  There should thus be a
	       half column_space margin on each side of each column... */

	    *width = (Dimension) ((lw->list.ncols * lw->list.col_width)
				  + 2 * lw->list.internal_width);
	    change = TRUE;
	}
	if (yfree) {		/* If allowed resize height. */
	    *height = (Dimension) ((lw->list.nrows * lw->list.row_height)
				   + 2 * lw->list.internal_height);
	    change = TRUE;
	}
	return (change);
    }

/*
 * If both width and height are free to change the use default_cols
 * to determine the number columns and set new width and height to
 * just fit the window.
 */

    if (xfree && yfree) {
	lw->list.ncols = lw->list.default_cols;
	if (lw->list.ncols <= 0)
	    lw->list.ncols = 1;
	lw->list.nrows = ((lw->list.nitems - 1) / lw->list.ncols) + 1;
	*width = (Dimension) ((lw->list.ncols * lw->list.col_width)
			      + 2 * lw->list.internal_width);
	*height = (Dimension) ((lw->list.nrows * lw->list.row_height)
			       + 2 * lw->list.internal_height);
	change = TRUE;
    }
/*
 * If the width is fixed then use it to determine the number of columns.
 * If the height is free to move (width still fixed) then resize the height
 * of the widget to fit the current list exactly.
 */
    else if (!xfree) {
	lw->list.ncols = ((int) (*width - 2 * lw->list.internal_width)
			  / (int) lw->list.col_width);
	if (lw->list.ncols <= 0)
	    lw->list.ncols = 1;
	lw->list.nrows = ((lw->list.nitems - 1) / lw->list.ncols) + 1;
	if (yfree) {
	    *height = (Dimension) ((lw->list.nrows * lw->list.row_height)
				   + 2 * lw->list.internal_height);
	    change = TRUE;
	}
    }
/*
 * The last case is xfree and !yfree we use the height to determine
 * the number of rows and then set the width to just fit the resulting
 * number of columns.
 */
    else if (!yfree) {		/* xfree must be TRUE. */
	lw->list.nrows = (int) (*height - 2 * lw->list.internal_height)
	    / (int) lw->list.row_height;
	if (lw->list.nrows <= 0)
	    lw->list.nrows = 1;
	lw->list.ncols = ((lw->list.nitems - 1) / lw->list.nrows) + 1;
	*width = (Dimension) ((lw->list.ncols * lw->list.col_width)
			      + 2 * lw->list.internal_width);
	change = TRUE;
    }
    return (change);
}

/* Notify() - ACTION
 *
 * Notifies the user that a button has been pressed, and
 * calls the callback; if the XtNpasteBuffer resource is true
 * then the name of the item is also put in CUT_BUFFER0.	*/

/* ARGSUSED */
static void
Notify(
	  Widget w,
	  XEvent *event,
	  String *params GCC_UNUSED,
	  Cardinal *num_params GCC_UNUSED)
{
    ListWidget lw = (ListWidget) w;
    int item;
    char *str;
    XawListReturnStruct ret_value;

/*
 * Find item and if out of range then unhighlight and return.
 *
 * If the current item is unhighlighted then the user has aborted the
 * notify, so unhighlight and return.
 */

    if (((CvtToItem(w, event->xbutton.x, event->xbutton.y, &item)) ==
	OUT_OF_RANGE) ||
	(lw->list.highlight != item)) {
	XawListUnhighlight(w);
	return;
    }
    /* if XtNpasteBuffer is set then paste it. */

    str = DeConst(lw->list.iconList
		  ? lw->list.iconList[item].string
		  : lw->list.list[item]);
    if (lw->list.paste)
	XStoreBytes(XtDisplay(w), str, (int) strlen(str));

    /* Call Callback function. */

    ret_value.string = str;
    ret_value.list_index = item;
    XtCallCallbacks(w, XtNcallback, (XtPointer) &ret_value);
}

/* Unset() - ACTION
 *
 * unhighlights the current element. */

/* ARGSUSED */
static void
Unset(
	 Widget w,
	 XEvent *event GCC_UNUSED,
	 String *params GCC_UNUSED,
	 Cardinal *num_params GCC_UNUSED)
{
    XawListUnhighlight(w);
}

/* Set() - ACTION
 *
 * Highlights the current element. */

/* ARGSUSED */
static void
Set(
       Widget w,
       XEvent *event,
       String *params GCC_UNUSED,
       Cardinal *num_params GCC_UNUSED)
{
    int item;
    ListWidget lw = (ListWidget) w;

    if ((CvtToItem(w, event->xbutton.x, event->xbutton.y, &item))
	== OUT_OF_RANGE)
	XawListUnhighlight(w);	/* Unhighlight current item. */
    else if (lw->list.is_highlighted != item)	/* If this item is not */
	XawListHighlight(w, item);	/* highlighted then do it. */
}

/*
 * Set specified arguments into widget
 */

static Boolean
SetValues(
	     Widget current,
	     Widget request,
	     Widget new,
	     ArgList args GCC_UNUSED,
	     Cardinal *num_args GCC_UNUSED)
{
    ListWidget cl = (ListWidget) current;
    ListWidget rl = (ListWidget) request;
    ListWidget nl = (ListWidget) new;
    Boolean redraw = FALSE;
    int text_height;
    XFontSetExtents *ext = XExtentsOfFontSet(nl->list.fontset);

    /* If the request height/width is different, lock it.  Unless its 0. If */
    /* neither new nor 0, leave it as it was.  Not in R5. */

    if (nl->core.width != cl->core.width)
	nl->list.freedoms |= WidthLock;
    if (nl->core.width == 0)
	nl->list.freedoms &= ~WidthLock;

    if (nl->core.height != cl->core.height)
	nl->list.freedoms |= HeightLock;
    if (nl->core.height == 0)
	nl->list.freedoms &= ~HeightLock;

    if (nl->list.longest != cl->list.longest)
	nl->list.freedoms |= LongestLock;
    if (nl->list.longest == 0)
	nl->list.freedoms &= ~LongestLock;

    /* _DONT_ check for fontset here - it's not in GC. */

    if ((cl->list.foreground != nl->list.foreground) ||
	(cl->core.background_pixel != nl->core.background_pixel) ||
	(cl->list.font != nl->list.font)) {
	XGCValues values;
	XGetGCValues(XtDisplay(current), cl->list.graygc, GCTile, &values);
	XmuReleaseStippledPixmap(XtScreen(current), values.tile);
	XtReleaseGC(current, cl->list.graygc);
	XtReleaseGC(current, cl->list.revgc);
	XtReleaseGC(current, cl->list.normgc);
	GetGCs(new);
	redraw = TRUE;
    }
    /* Reset row height. */

    if ((cl->list.font != nl->list.font) && (cl->simple.international == False)) {
	/* Check, if the row height is determined with the
	   font or with the icon and set it */

	text_height = nl->list.font->max_bounds.ascent + nl->list.font->max_bounds.descent;
	if (text_height > nl->list.icon_height)
	    nl->list.row_height = text_height + nl->list.row_space;
	else
	    nl->list.row_height = nl->list.icon_height + nl->list.row_space;
    } else if ((cl->list.fontset != nl->list.fontset) &&
	       (cl->simple.international == True)) {
	if (ext->max_ink_extent.height > nl->list.icon_height)
	    nl->list.row_height = ext->max_ink_extent.height + nl->list.row_space;
	else
	    nl->list.row_height = nl->list.icon_height + nl->list.row_space;
    }
    /* ...If the above two font(set) change checkers above both failed, check
       if row_space was altered.  If one of the above passed, row_height will
       already have been re-calculated. */

    else if (cl->list.row_space != nl->list.row_space)
	nl->list.row_height = cl->list.row_height + nl->list.row_space - cl->list.row_space;

    if ((cl->core.width != nl->core.width) ||
	(cl->core.height != nl->core.height) ||
	(cl->list.iconList != nl->list.iconList) ||
	((cl->list.list != nl->list.list) && (nl->list.iconList == NULL)) ||
	((cl->list.icon_height != nl->list.icon_height) &&
	nl->list.iconList) ||
	((cl->list.icon_width != nl->list.icon_width) && nl->list.iconList) ||
	(cl->list.internal_width != nl->list.internal_width) ||
	(cl->list.internal_height != nl->list.internal_height) ||
	(cl->list.column_space != nl->list.column_space) ||
	(cl->list.row_space != nl->list.row_space) ||
	(cl->list.default_cols != nl->list.default_cols) ||
	((cl->list.force_cols != nl->list.force_cols) &&
	 (rl->list.force_cols != nl->list.ncols)) ||
	(cl->list.vertical_cols != nl->list.vertical_cols) ||
	(cl->list.longest != nl->list.longest) ||
	(cl->list.nitems != nl->list.nitems) ||
	((cl->list.font != nl->list.font) && (nl->simple.international ==
	FALSE)) ||

    /* Equiv. fontsets might have different values, but the same fonts, so the
       next comparison is sloppy but not dangerous.  */

	((cl->list.fontset != nl->list.fontset) && nl->simple.international)) {
	CalculatedValues(new);
	Layout(new, WidthFree(nl), HeightFree(nl), &nl->core.width, &nl->core.height);
	redraw = TRUE;
    }

    if ((cl->list.list != nl->list.list) || (cl->list.iconList != nl->list.iconList))
	nl->list.is_highlighted = nl->list.highlight = NO_HIGHLIGHT;

    if ((cl->core.sensitive != nl->core.sensitive) ||
	(cl->core.ancestor_sensitive != nl->core.ancestor_sensitive)) {
	nl->list.highlight = NO_HIGHLIGHT;
	redraw = TRUE;
    }

    if (!XtIsRealized(current))
	return (FALSE);
    return (redraw);
}

static void
Destroy(
	   Widget w)
{
    ListWidget lw = (ListWidget) w;
    XGCValues values;

    XGetGCValues(XtDisplay(w), lw->list.graygc, GCTile, &values);
    XmuReleaseStippledPixmap(XtScreen(w), values.tile);
    XtReleaseGC(w, lw->list.graygc);
    XtReleaseGC(w, lw->list.revgc);
    XtReleaseGC(w, lw->list.normgc);
    XtReleaseGC(w, lw->list.iconGc);
}

/* Exported Functions */

/*	Function Name: XawListChange.
 *	Description: Changes the list being used and shown.
 *	Arguments: w - the list widget.
 *                 list - the new list.
 *                 nitems - the number of items in the list.
 *                 longest - the length (in Pixels) of the longest element
 *                           in the list.
 *                 resize - if TRUE the the list widget will
 *                          try to resize itself.
 *	Returns: none.
 *      NOTE:      If nitems of longest are <= 0 then they will be calculated.
 *                 If nitems is <= 0 then the list needs to be NULL terminated.
 */

void
XawListChange(
		 Widget w,
		 String *list,
		 int nitems,
		 int longest,
		 Boolean resize_it)
{
    ListWidget lw = (ListWidget) w;

    if (nitems < 0)
	nitems = 0;
    if (longest < 0)
	longest = 0;

    lw->list.list = list;
    lw->list.iconList = NULL;	/* To avoid the use of an iconList */
    lw->list.nitems = nitems;
    lw->list.longest = longest;

    /* If the user passes 0 meaning "calculate it", it must be free */

    if (longest != 0)
	lw->list.freedoms |= LongestLock;
    else
	lw->list.freedoms &= ~LongestLock;

    /* else - still resize if its not locked */

    if (resize_it)
	lw->list.freedoms &= ~WidthLock & ~HeightLock;

    /* Redisplay the list */

    ResetList(w, WidthFree(w), HeightFree(w));
    lw->list.is_highlighted = lw->list.highlight = NO_HIGHLIGHT;
    if (XtIsRealized(w))
	Redisplay(w, (XEvent *) NULL, (Region) NULL);
}

/*	Function Name: XawIconListChange.
 *	Description: Changes the ICONLIST being used and shown.
 *	Arguments: w - 		the list widget.
 *                 iconlist - 	the new list.
 *                 nitems - 	the number of items in the list.
 *                 longest - 	the length (in Pixels) of the longest element
 *                           	in the list.
 *		   icon_w -	width of all icons
 *		   icon_h -	height of all icons
 *		   icon_d -	depth of all icons
 *                 resize - 	if TRUE the the list widget will
 *                          	try to resize itself.
 *	Returns: none.
 *      NOTE:      If nitems of longest are <= 0 then they will be calculated.
 *                 If nitems is <= 0 then the list needs to be NULL terminated.
 */

void
XawIconListChange(
		     Widget w,
		     XawIconList * iconlist,
		     int nitems,
		     int longest,
		     int icon_w,
		     int icon_h,
		     int icon_d,
		     Boolean resize_it)
{
    ListWidget lw = (ListWidget) w;
    int text_height;

    /* Store the new values */

    if (nitems < 0)
	nitems = 0;
    if (longest < 0)
	longest = 0;

    lw->list.iconList = iconlist;
    lw->list.list = NULL;
    lw->list.nitems = nitems;
    lw->list.longest = longest;
    lw->list.icon_width = (Dimension) icon_w;
    lw->list.icon_height = (Dimension) icon_h;
    lw->list.icon_depth = (Dimension) icon_d;

    /* If the user passes 0 meaning "calculate it", it must be free */

    if (longest != 0)
	lw->list.freedoms |= LongestLock;
    else
	lw->list.freedoms &= ~LongestLock;

    /* else - still resize if its not locked */

    if (resize_it)
	lw->list.freedoms &= ~WidthLock & ~HeightLock;

    /* Set row height, if required */

    text_height = lw->list.font->max_bounds.ascent + lw->list.font->max_bounds.descent;
    if (lw->list.icon_height > text_height)
	lw->list.row_height = lw->list.icon_height + lw->list.row_space;

    /* Redisplay the list */

    ResetList(w, WidthFree(w), HeightFree(w));
    lw->list.is_highlighted = lw->list.highlight = NO_HIGHLIGHT;
    if (XtIsRealized(w))
	Redisplay(w, (XEvent *) NULL, (Region) NULL);
}

/*	Function Name: XawListUnhighlight
 *	Description: unlights the current highlighted element.
 *	Arguments: w - the widget.
 *	Returns: none.
 */

void
XawListUnhighlight(
		      Widget w)
{
    ListWidget lw = (ListWidget) w;

    lw->list.highlight = NO_HIGHLIGHT;
    if (lw->list.is_highlighted != NO_HIGHLIGHT)
	PaintItemName(w, lw->list.is_highlighted);	/* unhighlight this one. */
}

/*	Function Name: XawListHighlight
 *	Description: Highlights the given item.
 *	Arguments: w - the list widget.
 *                 item - the item to hightlight.
 *	Returns: none.
 */

void
XawListHighlight(
		    Widget w,
		    int item)
{
    ListWidget lw = (ListWidget) w;

    if (XtIsSensitive(w)) {
	lw->list.highlight = item;
	if (lw->list.is_highlighted != NO_HIGHLIGHT)
	    PaintItemName(w, lw->list.is_highlighted);	/* Unhighlight. */
	PaintItemName(w, item);	/* HIGHLIGHT this one. */
    }
}

/*	Function Name: XawListShowCurrent
 *	Description: returns the currently highlighted object.
 *	Arguments: w - the list widget.
 *	Returns: the info about the currently highlighted object.
 */

XawListReturnStruct *
XawListShowCurrent(Widget w)
{
    ListWidget lw = (ListWidget) w;
    XawListReturnStruct *ret_val;

    ret_val = (XawListReturnStruct *) XtMalloc(sizeof(XawListReturnStruct));

    /* With this change we let it crash outside instead of inside Xaw :o) */

    if (ret_val) {
	ret_val->list_index = lw->list.highlight;
	if (ret_val->list_index == XAW_LIST_NONE)
	    ret_val->string = "";
	else
	    ret_val->string = lw->list.iconList ?
		lw->list.iconList[ret_val->list_index].string
		: lw->list.list[ret_val->list_index];
    }
    return (ret_val);
}
