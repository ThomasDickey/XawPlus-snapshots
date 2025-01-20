/*
 * $XTermId: Grip.c,v 1.5 2025/01/19 14:06:01 tom Exp $
 * $Xorg: Grip.c,v 1.4 2001/02/09 02:03:43 xorgcvs Exp $
 */

/*************************************************************************

Copyright 2022,2025  Thomas E. Dickey
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

**************************************************************************/

/*
 * Grip.c - Grip Widget (Used by Paned Widget)
 *
 */

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/GripP.h>

static XtResource resources[] = {
   {XtNwidth, XtCWidth, XtRDimension, sizeof(Dimension),
      XtOffsetOf(GripRec, core.width), XtRImmediate,
      (XtPointer) DEFAULT_GRIP_SIZE},
   {XtNheight, XtCHeight, XtRDimension, sizeof(Dimension),
      XtOffsetOf(GripRec, core.height), XtRImmediate,
      (XtPointer) DEFAULT_GRIP_SIZE},
   {XtNborderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
      XtOffsetOf(GripRec, core.border_width), XtRImmediate, (XtPointer)0},
   {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer),
      XtOffsetOf(GripRec, grip.grip_action), XtRCallback, NULL},
};

/* Function prototypes */

static void GripAction(Widget widget, XEvent *event, String *params, Cardinal *num_params);
static void Redisplay(Widget w, XEvent *event, Region region);

static XtActionsRec actionsList[] =
{
  {"GripAction",      GripAction},
};

#define SuperClass (&simpleClassRec)

GripClassRec gripClassRec = {
   {
/* core class fields */
    /* superclass         */   (WidgetClass) SuperClass,
    /* class name         */   "Grip",
    /* size               */   sizeof(GripRec),
    /* class initialize   */   XawInitializeWidgetSet,
    /* class_part_init    */   NULL,
    /* class_inited       */   FALSE,
    /* initialize         */   NULL,
    /* initialize_hook    */   NULL,
    /* realize            */   XtInheritRealize,
    /* actions            */   actionsList,
    /* num_actions        */   XtNumber(actionsList),
    /* resources          */   resources,
    /* resource_count     */   XtNumber(resources),
    /* xrm_class          */   NULLQUARK,
    /* compress_motion    */   TRUE,
    /* compress_exposure  */   XtExposeCompressSeries,
    /* compress_enterleave*/   TRUE,
    /* visible_interest   */   FALSE,
    /* destroy            */   NULL,
    /* resize             */   NULL,
    /* expose             */   Redisplay,
    /* set_values         */   NULL,
    /* set_values_hook    */   NULL,
    /* set_values_almost  */   XtInheritSetValuesAlmost,
    /* get_values_hook    */   NULL,
    /* accept_focus       */   NULL,
    /* version            */   XtVersion,
    /* callback_private   */   NULL,
    /* tm_table           */   NULL,
    /* query_geometry     */   XtInheritQueryGeometry,
    /* display_accelerator*/   XtInheritDisplayAccelerator,
    /* extension          */   NULL
   },
/* Simple class fields initialization */
   {
    /* change_sensitive   */   XtInheritChangeSensitive
   },
/* Grip class fields initialization */
   {
    /* not used		  */   0
   }
};

WidgetClass gripWidgetClass = (WidgetClass) &gripClassRec;

/***********************************************************************
 *
 * Grip Action:		Call the action callbacks
 *
 ***********************************************************************/

static void GripAction(
    Widget widget,
    XEvent *event,
    String *params,
    Cardinal *num_params)
{
    XawGripCallDataRec call_data;

    call_data.event = event;
    call_data.params = params;
    call_data.num_params = *num_params;

    XtCallCallbacks( widget, XtNcallback, (XtPointer)&call_data );
}

/***********************************************************************
 *
 * Redisplay method:	Redisplay the grip
 *
 ***********************************************************************/

static void Redisplay(
Widget w,
XEvent *event GCC_UNUSED,
Region region GCC_UNUSED)
{
  XawRaisedButton(w, 0, 0, w->core.width, w->core.height);
}
