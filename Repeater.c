/*
 * $XTermId: Repeater.c,v 1.4 2022/12/13 00:53:17 tom Exp $
 * $Xorg: Repeater.c,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 *
Copyright 2022  Thomas E. Dickey
Copyright 1990, 1994, 1998  The Open Group

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
 * Author:  Jim Fulton, MIT X Consortium
 *
 * This widget is used for press-and-hold style buttons.
 *
 * This file contains modifications for XawPlus, Roland Krause 2002
 */

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>		/* for XtN and XtC defines */
#include <X11/XawPlus/XawInit.h>	/* for XawInitializeWidgetSet() */
#include <X11/XawPlus/RepeaterP.h>	/* us */

static void tic(XtPointer client_data, XtIntervalId *id);

#define DO_CALLBACK(rw) \
    XtCallCallbackList ((Widget) rw, rw->command.callbacks, (XtPointer)NULL)


#define ADD_TIMEOUT(rw,delay) \
  XtAppAddTimeOut (XtWidgetToApplicationContext ((Widget) rw), \
		   (unsigned long) delay, tic, (XtPointer) rw)

#define CLEAR_TIMEOUT(rw) \
  if ((rw)->repeater.timer) { \
      XtRemoveTimeOut ((rw)->repeater.timer); \
      (rw)->repeater.timer = 0; \
  }


/*
 * Translations to give user interface of press-notify...-release_or_leave
 */
static char defaultTranslations[] =
  "<EnterWindow>:     highlight() \n\
   <LeaveWindow>:     unhighlight() \n\
   <Btn1Down>:        set() start() \n\
   <Btn1Up>:          stop() unset() ";


/*
 * Actions added by this widget
 */
static void ActionStart(Widget gw, XEvent *event, String *params, Cardinal *num_params);
static void ActionStop(Widget gw, XEvent *event, String *params, Cardinal *num_params);

static XtActionsRec actions[] = {
    { "start", ActionStart },		/* trigger timers */
    { "stop", ActionStop },		/* clear timers */
};


/*
 * New resources added by this widget
 */
static XtResource resources[] = {
#define off(field) XtOffsetOf(RepeaterRec, repeater.field)
    { XtNdecay, XtCDecay, XtRInt, sizeof (int),
	off(decay), XtRImmediate, (XtPointer) REP_DEF_DECAY },
    { XtNinitialDelay, XtCDelay, XtRInt, sizeof (int),
	off(initial_delay), XtRImmediate, (XtPointer) REP_DEF_INITIAL_DELAY },
    { XtNminimumDelay, XtCMinimumDelay, XtRInt, sizeof (int),
	off(minimum_delay), XtRImmediate, (XtPointer) REP_DEF_MINIMUM_DELAY },
    { XtNrepeatDelay, XtCDelay, XtRInt, sizeof (int),
	off(repeat_delay), XtRImmediate, (XtPointer) REP_DEF_REPEAT_DELAY },
    { XtNflash, XtCBoolean, XtRBoolean, sizeof (Boolean),
	off(flash), XtRImmediate, (XtPointer) FALSE },
    { XtNstartCallback, XtCStartCallback, XtRCallback, sizeof (XtPointer),
	off(start_callbacks), XtRImmediate, (XtPointer) NULL },
    { XtNstopCallback, XtCStopCallback, XtRCallback, sizeof (XtPointer),
	off(stop_callbacks), XtRImmediate, (XtPointer) NULL },
#undef off
};


/*
 * Class Methods
 */

static void Initialize(Widget greq, Widget gnew, ArgList args, Cardinal *num_args);
static void Destroy(Widget gw);
static Boolean SetValues(Widget gcur, Widget greq, Widget gnew, ArgList args, Cardinal *num_args);

RepeaterClassRec repeaterClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &commandClassRec,
    /* class_name		*/	"Repeater",
    /* widget_size		*/	sizeof(RepeaterRec),
    /* class_initialize		*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	actions,
    /* num_actions		*/	XtNumber(actions),
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	XtExposeCompressSeries,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	XtInheritResize,
    /* expose			*/	XtInheritExpose,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	defaultTranslations,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* simple fields */
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
  { /* label fields */
    /* ignore			*/	0
  },
  { /* command fields */
    /* ignore			*/	0
  },
  { /* repeater fields */
    /* ignore                   */	0
  }
};

WidgetClass repeaterWidgetClass = (WidgetClass) &repeaterClassRec;


/*****************************************************************************
 *                                                                           *
 *			   repeater utility routines                         *
 *                                                                           *
 *****************************************************************************/

/* clock timeout */
static void tic (
    XtPointer client_data,
    XtIntervalId *id GCC_UNUSED)
{
    RepeaterWidget rw = (RepeaterWidget) client_data;

    rw->repeater.timer = 0;		/* timer is removed */
    if (rw->repeater.flash) {
	XtExposeProc expose;
	expose = repeaterWidgetClass->core_class.superclass->core_class.expose;
	XClearWindow (XtDisplay((Widget) rw), XtWindow((Widget) rw));
	rw->command.set = FALSE;
	(*expose) ((Widget) rw, (XEvent *) NULL, (Region) NULL);
	XClearWindow (XtDisplay((Widget) rw), XtWindow((Widget) rw));
	rw->command.set = TRUE;
	(*expose) ((Widget) rw, (XEvent *) NULL, (Region) NULL);
    }
    DO_CALLBACK (rw);

    rw->repeater.timer = ADD_TIMEOUT (rw, rw->repeater.next_delay);

					/* decrement delay time, but clamp */
    if (rw->repeater.decay) {
	rw->repeater.next_delay -= rw->repeater.decay;
	if (rw->repeater.next_delay < rw->repeater.minimum_delay)
	  rw->repeater.next_delay = rw->repeater.minimum_delay;
    }
}


/*****************************************************************************
 *                                                                           *
 * 			    repeater class methods                           *
 *                                                                           *
 *****************************************************************************/

/* ARGSUSED */
static void Initialize (
    Widget greq GCC_UNUSED,
    Widget gnew,
    ArgList args GCC_UNUSED,
    Cardinal *num_args GCC_UNUSED)
{
    RepeaterWidget new = (RepeaterWidget) gnew;

    if (new->repeater.minimum_delay < 0) new->repeater.minimum_delay = 0;
    new->repeater.timer = (XtIntervalId) 0;
}

static void Destroy (Widget gw)
{
    CLEAR_TIMEOUT ((RepeaterWidget) gw);
}

/* ARGSUSED */
static Boolean SetValues (
    Widget gcur,
    Widget greq GCC_UNUSED,
    Widget gnew,
    ArgList args GCC_UNUSED,
    Cardinal *num_args GCC_UNUSED)
{
    RepeaterWidget cur = (RepeaterWidget) gcur;
    RepeaterWidget new = (RepeaterWidget) gnew;
    Boolean redisplay = FALSE;

    if (cur->repeater.minimum_delay != new->repeater.minimum_delay) {
	if (new->repeater.next_delay < new->repeater.minimum_delay)
	  new->repeater.next_delay = new->repeater.minimum_delay;
    }

    return redisplay;
}

/*****************************************************************************
 *                                                                           *
 * 			     repeater action procs                           *
 *                                                                           *
 *****************************************************************************/

/* ARGSUSED */
static void ActionStart (
    Widget gw,
    XEvent *event GCC_UNUSED,
    String *params GCC_UNUSED,
    Cardinal *num_params GCC_UNUSED)
{
    RepeaterWidget rw = (RepeaterWidget) gw;

    CLEAR_TIMEOUT (rw);
    if (rw->repeater.start_callbacks)
      XtCallCallbackList (gw, rw->repeater.start_callbacks, (XtPointer)NULL);

    DO_CALLBACK (rw);
    rw->repeater.timer = ADD_TIMEOUT (rw, rw->repeater.initial_delay);
    rw->repeater.next_delay = rw->repeater.repeat_delay;
}


/* ARGSUSED */
static void ActionStop (
    Widget gw,
    XEvent *event GCC_UNUSED,
    String *params GCC_UNUSED,
    Cardinal *num_params GCC_UNUSED)
{
    RepeaterWidget rw = (RepeaterWidget) gw;

    CLEAR_TIMEOUT ((RepeaterWidget) gw);
    if (rw->repeater.stop_callbacks)
      XtCallCallbackList (gw, rw->repeater.stop_callbacks, (XtPointer)NULL);
}
