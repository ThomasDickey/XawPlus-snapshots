/*
 * $XTermId: SmeLine.c,v 1.4 2022/12/13 00:53:17 tom Exp $
 * $Xorg: SmeLine.c,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 */

/*
Copyright 2022  Thomas E. Dickey
Copyright 1989, 1998  The Open Group

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
 *
 * Author:  Chris D. Peterson, MIT X Consortium
 */

/*
 * Sme.c - Source code for the generic menu entry
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

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/SmeLineP.h>
#include <X11/XawPlus/Cardinals.h>

#define offset(field) XtOffsetOf(SmeLineRec, sme_line.field)

static XtResource resources[] = {
  {XtNlineWidth, XtCLineWidth, XtRDimension, sizeof(Dimension),
     offset(line_width), XtRImmediate, (XtPointer) 1}
};
#undef offset

/*
 * Function definitions.
 */

static void Redisplay(Widget w, XEvent * event, Region region);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void DestroyGC(Widget w);
static void CreateGC(Widget w);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static XtGeometryResult QueryGeometry(Widget w, XtWidgetGeometry *intended, XtWidgetGeometry *return_val);


#define SUPERCLASS (&smeClassRec)

SmeLineClassRec smeLineClassRec = {
  {
    /* superclass         */    (WidgetClass) SUPERCLASS,
    /* class_name         */    "SmeLine",
    /* size               */    sizeof(SmeLineRec),
    /* class_initialize   */	XawInitializeWidgetSet,
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
    /* destroy            */    DestroyGC,
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
    /* Menu Entry Fields */

    /* highlight */             XtInheritHighlight,
    /* unhighlight */           XtInheritUnhighlight,
    /* notify */		XtInheritNotify,
    /* extension */             NULL
  },{
    /* Line Menu Entry Fields */
    /* extension */             NULL
  }
};

WidgetClass smeLineObjectClass = (WidgetClass) &smeLineClassRec;

/************************************************************
 *
 * Semi-Public Functions.
 *
 ************************************************************/

/*      Function Name: Initialize
 *      Description: Initializes the simple menu widget
 *      Arguments: request - the widget requested by the argument list.
 *                 new     - the new widget with both resource and non
 *                           resource values.
 *      Returns: none.
 */

static void Initialize(
Widget request GCC_UNUSED,
Widget new,
ArgList args GCC_UNUSED,
Cardinal *num_args GCC_UNUSED)
{
    SmeLineObject entry = (SmeLineObject) new;

    entry->rectangle.height = 2 * entry->sme_line.line_width;
    CreateGC(new);
}

/*	Function Name: CreateGC
 *	Description: Creates the GC for the line entry widget.
 *	Arguments: w - the Line entry widget.
 *	Returns: none
 *
 *      We can only share the GC if there is no stipple, because
 *      we need to change the stipple origin when drawing.
 */

static void CreateGC(Widget w)
{
    SmeLineObject entry = (SmeLineObject) w;
    XGCValues values;
    XtGCMask mask = GCForeground | GCGraphicsExposures | GCLineWidth ;

    values.foreground = entry->sme.highlightColor;
    values.graphics_exposures = FALSE;
    values.line_width = entry->sme_line.line_width;
    entry->sme_line.highGC = XtGetGC(w, mask, &values);

    values.foreground = entry->sme.shadowColor;
    entry->sme_line.shadowGC = XtGetGC(w, mask, &values);
}

/*	Function Name: DestroyGC
 *	Description: Destroys the GC when we are done with it.
 *	Arguments: w - the Line entry widget.
 *	Returns: none
 */

static void DestroyGC(Widget w)
{
    SmeLineObject entry = (SmeLineObject) w;

    XtReleaseGC(w, entry->sme_line.highGC);
    XtReleaseGC(w, entry->sme_line.shadowGC);
}

/*	Function Name: Redisplay
 *	Description: Paints the Line
 *	Arguments: w - the menu entry.
 *                 event, region - NOT USED.
 *	Returns: none
 */

static void Redisplay(
Widget w,
XEvent * event GCC_UNUSED,
Region region GCC_UNUSED)
{
    SmeLineObject entry = (SmeLineObject) w;
    int y = entry->rectangle.y;

    XDrawLine(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_line.shadowGC,
	entry->rectangle.x+2, y, (unsigned int) entry->rectangle.width, y);
    y += entry->sme_line.line_width;
    XDrawLine(XtDisplayOfObject(w), XtWindowOfObject(w), entry->sme_line.highGC,
	entry->rectangle.x+2, y, (unsigned int) entry->rectangle.width, y);
}

/*      Function Name: SetValues
 *      Description: Relayout the menu when one of the resources is changed.
 *      Arguments: current - current state of the widget.
 *                 request - what was requested.
 *                 new - what the widget will become.
 *      Returns: none
 */

static Boolean SetValues(
Widget current,
Widget request GCC_UNUSED,
Widget new,
ArgList args GCC_UNUSED,
Cardinal *num_args GCC_UNUSED)
{
    SmeLineObject entry = (SmeLineObject) new;
    SmeLineObject old_entry = (SmeLineObject) current;

    if ((entry->sme_line.line_width != old_entry->sme_line.line_width) ||
	(entry->sme.highlightColor != old_entry->sme.highlightColor) ||
	(entry->sme.shadowColor != old_entry->sme.shadowColor))
    {
	DestroyGC(current);
	CreateGC(new);
	return(TRUE);
    }
    return(FALSE);
}

/*	Function Name: QueryGeometry.
 *	Description: Returns the preferred geometry for this widget.
 *	Arguments: w - the menu entry object.
 *                 itended, return - the intended and return geometry info.
 *	Returns: A Geometry Result.
 *
 * See the Intrinsics manual for details on what this function is for.
 *
 * I just return the height and a width of 1.
 */

static XtGeometryResult QueryGeometry(
Widget w,
XtWidgetGeometry *intended,
XtWidgetGeometry *return_val)
{
    SmeObject entry = (SmeObject) w;
    Dimension width;
    XtGeometryResult ret_val = XtGeometryYes;
    XtGeometryMask mode = intended->request_mode;

    width = 1;			/* we can be really small. */

    if ( ((mode & CWWidth) && (intended->width != width)) ||
	 !(mode & CWWidth) ) {
	return_val->request_mode |= CWWidth;
	return_val->width = width;
	mode = return_val->request_mode;

	if ( (mode & CWWidth) && (width == entry->rectangle.width) )
	    return(XtGeometryNo);
	return(XtGeometryAlmost);
    }
    return(ret_val);
}
