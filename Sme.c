/*
 * $XTermId: Sme.c,v 1.5 2025/01/19 21:25:00 tom Exp $
 * $Xorg: Sme.c,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 */

/*
Copyright 2022  Thomas E. Dickey
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
#include <X11/XawPlus/SmeP.h>
#include <X11/XawPlus/Cardinals.h>
#include <X11/XawPlus/Simple.h>

#define offset(field) XtOffsetOf(SmeRec, sme.field)
static XtResource resources[] = {
  {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer),
     offset(callbacks), XtRCallback, (XtPointer)NULL},
  {XtNinternational, XtCInternational, XtRBoolean, sizeof(Boolean),
     offset(international), XtRImmediate, (XtPointer) FALSE},
  {XtNhighlightColor, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(highlightColor), XtRString, "grey90"},
  {XtNshadowColor, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(shadowColor), XtRString, "grey40"}
};
#undef offset

/*
 * Semi Public function definitions.
 */

static void Unhighlight(Widget w);
static void Highlight(Widget w);
static void Notify(Widget w);
static void ClassPartInitialize(WidgetClass class);
static void ClassInit(void);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static XtGeometryResult QueryGeometry(Widget w, XtWidgetGeometry *intended, XtWidgetGeometry *return_val);

#define SUPERCLASS (&rectObjClassRec)

SmeClassRec smeClassRec = {
  {
    /* superclass         */    (WidgetClass) SUPERCLASS,
    /* class_name         */    "Sme",
    /* size               */    sizeof(SmeRec),
    /* class_initialize   */	ClassInit,
    /* class_part_initialize*/	ClassPartInitialize,
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
    /* destroy            */    NULL,
    /* resize             */    NULL,
    /* expose             */    NULL,
    /* set_values         */    NULL,
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
    /* Simple Menu Entry Fields */

    /* highlight */             Highlight,
    /* unhighlight */           Unhighlight,
    /* notify */		Notify,
    /* extension */             NULL
  }
};

WidgetClass smeObjectClass = (WidgetClass) &smeClassRec;

/************************************************************
 *
 * Semi-Public Functions.
 *
 ************************************************************/

/*	Function Name: ClassPartInitialize
 *	Description: handles inheritance of class functions.
 *	Arguments: class - the widget classs of this widget.
 *	Returns: none.
 */

static void ClassPartInitialize(WidgetClass class)
{
    SmeObjectClass m_ent, superC;

    m_ent = (SmeObjectClass) class;
    superC = (SmeObjectClass) m_ent->rect_class.superclass;

/*
 * We don't need to check for null super since we'll get to TextSink
 * eventually.
 */

    if (m_ent->sme_class.highlight == XtInheritHighlight)
	m_ent->sme_class.highlight = superC->sme_class.highlight;

    if (m_ent->sme_class.unhighlight == XtInheritUnhighlight)
	m_ent->sme_class.unhighlight = superC->sme_class.unhighlight;

    if (m_ent->sme_class.notify == XtInheritNotify)
	m_ent->sme_class.notify = superC->sme_class.notify;
}

/* Class initialize method: Initialize object class:
 * 			    Install required converters
 */

static void ClassInit(void)
{
   /* Arguments for the color converter */

   static XtConvertArgRec colConvertArg[] = {
     {XtWidgetBaseOffset, (XtPointer)XtOffsetOf(WidgetRec, core.screen), sizeof(Screen *)},
     {XtWidgetBaseOffset, (XtPointer)XtOffsetOf(WidgetRec, core.colormap), sizeof(Colormap)}
   };

   XawInitializeWidgetSet();
   XtSetTypeConverter(XtRString, XtRPixel, XtCvtStringToPixel,
	colConvertArg, XtNumber(colConvertArg), XtCacheByDisplay, NULL);
}


/*      Function Name: Initialize
 *      Description: Initializes the simple menu widget
 *      Arguments: request - the widget requested by the argument list.
 *                 new     - the new widget with both resource and non
 *                           resource values.
 *      Returns: none.
 *
 * MENU ENTRIES CANNOT HAVE BORDERS.
 */

static void Initialize(
Widget request GCC_UNUSED,
Widget new,
ArgList args GCC_UNUSED,
Cardinal *num_args GCC_UNUSED)
{
    SmeObject entry = (SmeObject) new;

    entry->rectangle.border_width = 0;
}

/*	Function Name: Highlight
 *	Description: The default highlight proceedure for menu entries.
 *	Arguments: w - the menu entry.
 *	Returns: none.
 */

static void Highlight(Widget w GCC_UNUSED)
{
/* This space intentionally left blank. */
}

/*	Function Name: Unhighlight
 *	Description: The default unhighlight proceedure for menu entries.
 *	Arguments: w - the menu entry.
 *	Returns: none.
 */

static void Unhighlight(Widget w GCC_UNUSED)
{
/* This space intentionally left blank. */
}

/*	Function Name: Notify
 *	Description: calls the callback proceedures for this entry.
 *	Arguments: w - the menu entry.
 *	Returns: none.
 */

static void Notify(Widget w)
{
    XtCallCallbacks(w, XtNcallback, (XtPointer)NULL);
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
