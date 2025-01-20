/*
 * $XTermId: Toggle.c,v 1.7 2025/01/19 14:28:15 tom Exp $
 * $Xorg: Toggle.c,v 1.4 2001/02/09 02:03:47 xorgcvs Exp $
 */

/*

Copyright 2015-2022,2025  Thomas E. Dickey
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

This file contains modifications for XawPlus, Roland Krause 2002

*/

/*
 * Toggle.c - Toggle button widget
 *
 * Author: Chris D. Peterson
 *         MIT X Consortium
 *         kit@expo.lcs.mit.edu
 *
 * Date:   January 12, 1989
 *
 */

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/Misc.h>
#include <X11/XawPlus/ToggleP.h>

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

/*
 * The order of toggle and notify are important, as the state has
 * to be set when we call the notify proc.
 */

static char defaultTranslations[] =
    "<EnterWindow>:	    highlight()		\n\
     <LeaveWindow>:	    unhighlight()	\n\
     <Btn1Down>,<Btn1Up>:   toggle() notify()";

#define offset(field) XtOffsetOf(ToggleRec, field)

static XtResource resources[] = {
   {XtNstate, XtCState, XtRBoolean, sizeof(Boolean),
      offset(command.set), XtRString, "off"},
   {XtNradioGroup, XtCWidget, XtRWidget, sizeof(Widget),
      offset(toggle.widget), XtRWidget, (XtPointer) NULL },
   {XtNradioData, XtCRadioData, XtRPointer, sizeof(XtPointer),
      offset(toggle.radio_data), XtRPointer, (XtPointer) NULL },
};

#undef offset


static void Toggle(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Notify(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void ToggleSet(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void ToggleDestroy(Widget w, XtPointer junk, XtPointer garbage);
static void ClassInit(void);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);

/* Functions for handling the Radio Group. */

static RadioGroup * GetRadioGroup(Widget w);
static void CreateRadioGroup(Widget w1, Widget w2);
static void AddToRadioGroup(RadioGroup * group, Widget w);
static void TurnOffRadioSiblings(Widget w);
static void RemoveFromRadioGroup(Widget w);

static XtActionsRec actionsList[] =
{
  {"toggle",	        Toggle},
  {"notify",	        Notify},
  {"set",	        ToggleSet},
};

#define SuperClass ((CommandWidgetClass)&commandClassRec)

ToggleClassRec toggleClassRec = {
  {
    (WidgetClass) SuperClass,		/* superclass		  */
    "Toggle",				/* class_name		  */
    sizeof(ToggleRec),			/* size			  */
    ClassInit,				/* class_initialize	  */
    NULL,				/* class_part_initialize  */
    FALSE,				/* class_inited		  */
    Initialize,				/* initialize		  */
    NULL,				/* initialize_hook	  */
    XtInheritRealize,			/* realize		  */
    actionsList,			/* actions		  */
    XtNumber(actionsList),		/* num_actions		  */
    resources,				/* resources		  */
    XtNumber(resources),		/* resource_count	  */
    NULLQUARK,				/* xrm_class		  */
    FALSE,				/* compress_motion	  */
    XtExposeCompressSeries,		/* compress_exposure	  */
    TRUE,				/* compress_enterleave    */
    FALSE,				/* visible_interest	  */
    NULL,         			/* destroy		  */
    XtInheritResize,			/* resize		  */
    XtInheritExpose,			/* expose		  */
    SetValues,				/* set_values		  */
    NULL,				/* set_values_hook	  */
    XtInheritSetValuesAlmost,		/* set_values_almost	  */
    NULL,				/* get_values_hook	  */
    NULL,				/* accept_focus		  */
    XtVersion,				/* version		  */
    NULL,				/* callback_private	  */
    defaultTranslations,		/* tm_table		  */
    XtInheritQueryGeometry,		/* query_geometry	  */
    XtInheritDisplayAccelerator,	/* display_accelerator	  */
    NULL				/* extension		  */
  },  /* CoreClass fields initialization */
  {
    XtInheritChangeSensitive		/* change_sensitive	  */
  },  /* SimpleClass fields initialization */
  {
    0                                     /* field not used    */
  },  /* LabelClass fields initialization */
  {
    0                                     /* field not used    */
  },  /* CommmandClass fields initialization */
  {
      NULL,			        /* Set Procedure. */
      NULL,			        /* Unset Procedure. */
      NULL			        /* extension. */
  }  /* ToggleClass fields initialization */
};

  /* for public consumption */
WidgetClass toggleWidgetClass = (WidgetClass) &toggleClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void
ClassInit(void)
{
  XtActionList actions;
  Cardinal num_actions;
  Cardinal i;
  ToggleWidgetClass class = (ToggleWidgetClass) toggleWidgetClass;
  static XtConvertArgRec parentCvtArgs[] = {
      {XtBaseOffset, (XtPointer)XtOffsetOf(WidgetRec, core.parent),
	   sizeof(Widget)}
  };

  XawInitializeWidgetSet();
  XtSetTypeConverter(XtRString, XtRWidget, XmuNewCvtStringToWidget,
		     parentCvtArgs, XtNumber(parentCvtArgs), XtCacheNone,
		     (XtDestructor)NULL);
/*
 * Find the set and unset actions in the command widget's action table.
 */

  XtGetActionList(commandWidgetClass, &actions, &num_actions);

  for (i = 0 ; i < num_actions ; i++) {
    if (streq(actions[i].string, "set"))
	class->toggle_class.Set = actions[i].proc;
    if (streq(actions[i].string, "reset"))
	class->toggle_class.Reset = actions[i].proc;

    if ( (class->toggle_class.Set != NULL) &&
	 (class->toggle_class.Reset != NULL) ) {
	XtFree((char *) actions);
	return;
    }
  }

/* We should never get here. */
  XtError("Aborting, due to errors resolving bindings in the Toggle widget.");
}

static void Initialize(
 Widget request,
 Widget new,
 ArgList args GCC_UNUSED,
 Cardinal *num_args GCC_UNUSED)
{
    ToggleWidget tw = (ToggleWidget) new;
    ToggleWidget tw_req = (ToggleWidget) request;

    tw->toggle.radio_group = NULL;

    if (tw->toggle.radio_data == NULL)
      tw->toggle.radio_data = (XtPointer) DeConst(new->core.name);

    if (tw->toggle.widget != NULL) {
      if ( GetRadioGroup(tw->toggle.widget) == NULL)
	CreateRadioGroup(new, tw->toggle.widget);
      else
	AddToRadioGroup( GetRadioGroup(tw->toggle.widget), new);
    }
    XtAddCallback(new, XtNdestroyCallback, ToggleDestroy, (XtPointer)NULL);

/*
 * Command widget assumes that the widget is unset, so we only
 * have to handle the case where it needs to be set.
 *
 * If this widget is in a radio group then it may cause another
 * widget to be unset, thus calling the notify proceedure.
 *
 * I want to set the toggle if the user set the state to "On" in
 * the resource group, reguardless of what my ancestors did.
 */

    if (tw_req->command.set)
      ToggleSet(new, (XEvent *)NULL, (String *)NULL, (Cardinal *)NULL);
}

/************************************************************
 *
 *  Action Procedures
 *
 ************************************************************/

/* ARGSUSED */
static void
ToggleSet(
Widget w,
XEvent *event,
String *params GCC_UNUSED,
Cardinal *num_params GCC_UNUSED)
{
    ToggleWidgetClass class = (ToggleWidgetClass) w->core.widget_class;

    TurnOffRadioSiblings(w);
    class->toggle_class.Set(w, event, NULL, NULL);
}

/* ARGSUSED */
static void
Toggle(
Widget w,
XEvent *event,
String *params,
Cardinal *num_params)
{
  ToggleWidget tw = (ToggleWidget)w;
  ToggleWidgetClass class = (ToggleWidgetClass) w->core.widget_class;

  if (tw->command.set)
    class->toggle_class.Reset(w, event, NULL, NULL);
  else
    ToggleSet(w, event, params, num_params);
}

/* ARGSUSED */
static void Notify(
Widget w,
XEvent *event GCC_UNUSED,
String *params GCC_UNUSED,
Cardinal *num_params GCC_UNUSED)
{
  ToggleWidget tw = (ToggleWidget) w;
  long antilint = tw->command.set;

  XtCallCallbacks( w, XtNcallback, (XtPointer) antilint );
}

/************************************************************
 *
 * Set specified arguments into widget
 *
 ***********************************************************/

/* ARGSUSED */
static Boolean
SetValues (
Widget current,
Widget request,
Widget new,
ArgList args GCC_UNUSED,
Cardinal *num_args GCC_UNUSED)
{
    ToggleWidget oldtw = (ToggleWidget) current;
    ToggleWidget tw = (ToggleWidget) new;
    ToggleWidget rtw = (ToggleWidget) request;

    if (oldtw->toggle.widget != tw->toggle.widget)
      XawToggleChangeRadioGroup(new, tw->toggle.widget);

    if (!tw->core.sensitive && oldtw->core.sensitive && rtw->command.set)
	tw->command.set = True;

    if (oldtw->command.set != tw->command.set) {
	tw->command.set = oldtw->command.set;
	Toggle(new, (XEvent *)NULL, (String *)NULL, (Cardinal *)0);
    }
    return(FALSE);
}

/*	Function Name: ToggleDestroy
 *	Description: Destroy Callback for toggle widget.
 *	Arguments: w - the toggle widget that is being destroyed.
 *                 junk, grabage - not used.
 *	Returns: none.
 */

/* ARGSUSED */
static void
ToggleDestroy(Widget w, XtPointer junk GCC_UNUSED, XtPointer garbage GCC_UNUSED)
{
  RemoveFromRadioGroup(w);
}

/************************************************************
 *
 * Below are all the private procedures that handle
 * radio toggle buttons.
 *
 ************************************************************/

/*	Function Name: GetRadioGroup
 *	Description: Gets the radio group associated with a give toggle
 *                   widget.
 *	Arguments: w - the toggle widget who's radio group we are getting.
 *	Returns: the radio group associated with this toggle group.
 */

static RadioGroup *
GetRadioGroup(Widget w)
{
  ToggleWidget tw = (ToggleWidget) w;

  if (tw == NULL) return(NULL);
  return( tw->toggle.radio_group );
}

/*	Function Name: CreateRadioGroup
 *	Description: Creates a radio group. give two widgets.
 *	Arguments: w1, w2 - the toggle widgets to add to the radio group.
 *	Returns: none.
 *
 *      NOTE:  A pointer to the group is added to each widget's radio_group
 *             field.
 */

static void
CreateRadioGroup(Widget w1, Widget w2)
{
  char error_buf[BUFSIZ];
  ToggleWidget tw1 = (ToggleWidget) w1;
  ToggleWidget tw2 = (ToggleWidget) w2;

  if ( (tw1->toggle.radio_group != NULL) || (tw2->toggle.radio_group != NULL) ) {
    (void) sprintf(error_buf, "%s %s", "Toggle Widget Error - Attempting",
	    "to create a new toggle group, when one already exists.");
    XtWarning(error_buf);
  }

  AddToRadioGroup( (RadioGroup *)NULL, w1 );
  AddToRadioGroup( GetRadioGroup(w1), w2 );
}

/*	Function Name: AddToRadioGroup
 *	Description: Adds a toggle to the radio group.
 *	Arguments: group - any element of the radio group the we are adding to.
 *                 w - the new toggle widget to add to the group.
 *	Returns: none.
 */

static void
AddToRadioGroup(RadioGroup * group, Widget w)
{
  ToggleWidget tw = (ToggleWidget) w;
  RadioGroup * local;

  local = (RadioGroup *) XtMalloc( sizeof(RadioGroup) );
  local->widget = w;
  tw->toggle.radio_group = local;

  if (group == NULL) {		/* Creating new group. */
    group = local;
    group->next = NULL;
    group->prev = NULL;
    return;
  }
  local->prev = group;		/* Adding to previous group. */
  if ((local->next = group->next) != NULL)
      local->next->prev = local;
  group->next = local;
}

/*	Function Name: TurnOffRadioSiblings
 *	Description: Deactivates all radio siblings.
 *	Arguments: widget - a toggle widget.
 *	Returns: none.
 */

static void
TurnOffRadioSiblings(Widget w)
{
  RadioGroup * group;
  ToggleWidgetClass class = (ToggleWidgetClass) w->core.widget_class;

  if ( (group = GetRadioGroup(w)) == NULL)  /* Punt if there is no group */
    return;

  /* Go to the top of the group. */

  for ( ; group->prev != NULL ; group = group->prev );

  while ( group != NULL ) {
    ToggleWidget local_tog = (ToggleWidget) group->widget;
    if ( local_tog->command.set ) {
      class->toggle_class.Reset(group->widget, NULL, NULL, NULL);
      Notify(group->widget, (XEvent *)NULL, (String *)NULL, (Cardinal *)NULL);
    }
    group = group->next;
  }
}

/*	Function Name: RemoveFromRadioGroup
 *	Description: Removes a toggle from a RadioGroup.
 *	Arguments: w - the toggle widget to remove.
 *	Returns: none.
 */

static void
RemoveFromRadioGroup(Widget w)
{
  RadioGroup * group = GetRadioGroup(w);
  if (group != NULL) {
    if (group->prev != NULL)
      (group->prev)->next = group->next;
    if (group->next != NULL)
      (group->next)->prev = group->prev;
    XtFree((char *) group);
  }
}

/************************************************************
 *
 * Public Routines
 *
 ************************************************************/

/*	Function Name: XawToggleChangeRadioGroup
 *	Description: Allows a toggle widget to change radio groups.
 *	Arguments: w - The toggle widget to change groups.
 *                 radio_group - any widget in the new group.
 *	Returns: none.
 */

void
XawToggleChangeRadioGroup(Widget w, Widget radio_group)
{
  ToggleWidget tw = (ToggleWidget) w;
  RadioGroup * group;

  RemoveFromRadioGroup(w);

/*
 * If the toggle that we are about to add is set then we will
 * unset all toggles in the new radio group.
 */

  if ( tw->command.set && radio_group != NULL )
    XawToggleUnsetCurrent(radio_group);

  if (radio_group != NULL) {
      if ((group = GetRadioGroup(radio_group)) == NULL)
	  CreateRadioGroup(w, radio_group);
      else AddToRadioGroup(group, w);
  }
}

/*	Function Name: XawToggleGetCurrent
 *	Description: Returns the RadioData associated with the toggle
 *                   widget that is currently active in a toggle group.
 *	Arguments: w - any toggle widget in the toggle group.
 *	Returns: The XtNradioData associated with the toggle widget.
 */

XtPointer
XawToggleGetCurrent(Widget w)
{
  RadioGroup * group;

  if ( (group = GetRadioGroup(w)) == NULL) return(NULL);
  for ( ; group->prev != NULL ; group = group->prev);

  while ( group != NULL ) {
    ToggleWidget local_tog = (ToggleWidget) group->widget;
    if ( local_tog->command.set )
      return( local_tog->toggle.radio_data );
    group = group->next;
  }
  return(NULL);
}

/*	Function Name: XawToggleSetCurrent
 *	Description: Sets the Toggle widget associated with the
 *                   radio_data specified.
 *	Arguments: radio_group - any toggle widget in the toggle group.
 *                 radio_data - radio data of the toggle widget to set.
 *	Returns: none.
 */

void
XawToggleSetCurrent(Widget radio_group, XtPointer radio_data)
{
  RadioGroup * group;
  ToggleWidget local_tog;

/* Special case of no radio group. */

  if ( (group = GetRadioGroup(radio_group)) == NULL) {
    local_tog = (ToggleWidget) radio_group;
    if ( (local_tog->toggle.radio_data == radio_data) )
      if (!local_tog->command.set) {
	ToggleSet((Widget) local_tog, (XEvent *)NULL, (String *)NULL, (Cardinal *)0);
	Notify((Widget) local_tog, (XEvent *)NULL, (String *)NULL, (Cardinal *)NULL);
      }
    return;
  }

/*
 * find top of radio_roup
 */

  for ( ; group->prev != NULL ; group = group->prev);

/*
 * search for matching radio data.
 */

  while ( group != NULL ) {
    local_tog = (ToggleWidget) group->widget;
    if ( (local_tog->toggle.radio_data == radio_data) ) {
      if (!local_tog->command.set) { /* if not already set. */
	ToggleSet((Widget) local_tog, (XEvent *)NULL, (String *)NULL, (Cardinal *)NULL);
	Notify((Widget) local_tog, (XEvent *)NULL, (String *)NULL, (Cardinal *)NULL);
      }
      return;			/* found it, done */
    }
    group = group->next;
  }
}

/*	Function Name: XawToggleUnsetCurrent
 *	Description: Unsets all Toggles in the radio_group specified.
 *	Arguments: radio_group - any toggle widget in the toggle group.
 *	Returns: none.
 */

void
XawToggleUnsetCurrent(Widget radio_group)
{
  ToggleWidgetClass class;
  ToggleWidget local_tog = (ToggleWidget) radio_group;

  /* Special Case no radio group. */

  if (local_tog->command.set) {
    class = (ToggleWidgetClass) local_tog->core.widget_class;
    class->toggle_class.Reset(radio_group, NULL, NULL, NULL);
    Notify(radio_group, (XEvent *)NULL, (String *)NULL, (Cardinal *)NULL);
  }
  if ( GetRadioGroup(radio_group) == NULL) return;
  TurnOffRadioSiblings(radio_group);
}
