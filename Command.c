/*
 * $XTermId: Command.c,v 1.7 2024/04/29 00:04:37 tom Exp $
 * $Xorg: Command.c,v 1.5 2001/02/09 02:03:43 xorgcvs Exp $
 */

/************************************************************************

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

*************************************************************************/

/*
 * Command.c - Command button widget
 */

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Misc.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/Shell.h>
#include <X11/XawPlus/CommandP.h>
#include <X11/Xmu/Xmu.h>

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/
/* *INDENT-OFF* */
/* Private Data */

static char defaultTranslations[] =
    "<EnterWindow>:	highlight()		\n\
     <LeaveWindow>:	reset()			\n\
     <Btn1Down>:	set()			\n\
     <Btn1Up>:		notify() unset()	";

#define offset(field) XtOffsetOf(CommandRec, field)
static XtResource resources[] = {
   {XtNborderWidth, XtCBorderWidth, XtRDimension, sizeof(Dimension),
	offset(core.border_width),  XtRImmediate, (XtPointer)0},
   {XtNhelpText, XtCLabel, XtRString, sizeof(String),
	offset(command.helpText), XtRString, NULL},
   {XtNuseHelp, XtCBoolean, XtRBoolean, sizeof(Boolean),
	offset(command.useHelp), XtRImmediate, (XtPointer)True},
  {XtNhelpBackground, XtCBackground, XtRPixel, sizeof(Pixel),
	offset(command.helpBackground), XtRString, "LightGoldenrodYellow"},
   {XtNcallback, XtCCallback, XtRCallback, sizeof(XtPointer),
	offset(command.callbacks), XtRCallback, (XtPointer)NULL},
   {XtNhighlightThickness, XtCThickness, XtRDimension, sizeof(Dimension),
	offset(command.highlight_thickness), XtRImmediate, (XtPointer)DEFAULT_THICKNESS},
   {XtNhighlightMode, XtCBoolean, XtRBoolean, sizeof(Boolean),
	offset(command.highlightMode), XtRImmediate, (XtPointer)True},
};
#undef offset

static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Redisplay(Widget w, XEvent *event, Region region);
static void Set(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void Reset(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void Notify(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void Unset(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void Highlight(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void Unhighlight(Widget w, XEvent *event, String *params, Cardinal *num_params);
static void Destroy(Widget w);
static void PaintCommandWidget(Widget w, Region region);
static void Resize(Widget w);
static void RepositionLabel(Widget current, Widget new);
static XtGeometryResult QueryGeometry(Widget w, XtWidgetGeometry *intended, XtWidgetGeometry *preferred);

static XtActionsRec actionsList[] = {
  {"set",		Set},
  {"notify",		Notify},
  {"highlight",		Highlight},
  {"reset",		Reset},
  {"unset",		Unset},
  {"unhighlight",	Unhighlight}
};

#define SuperClass ((LabelWidgetClass)&labelClassRec)

CommandClassRec commandClassRec = {
  {
    (WidgetClass) SuperClass,		/* superclass		  */
    "Command",				/* class_name		  */
    sizeof(CommandRec),			/* size			  */
    XawInitializeWidgetSet,		/* class_initialize	  */
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
    Destroy,				/* destroy		  */
    Resize,				/* resize		  */
    Redisplay,				/* expose		  */
    SetValues,				/* set_values		  */
    NULL,				/* set_values_hook	  */
    XtInheritSetValuesAlmost,		/* set_values_almost	  */
    NULL,				/* get_values_hook	  */
    NULL,				/* accept_focus		  */
    XtVersion,				/* version		  */
    NULL,				/* callback_private	  */
    defaultTranslations,		/* tm_table		  */
    QueryGeometry, 			/* query_geometry	  */
    XtInheritDisplayAccelerator,	/* display_accelerator	  */
    NULL				/* extension		  */
  },  /* CoreClass fields initialization */
  {
    XtInheritChangeSensitive		/* change_sensitive	*/
  },  /* SimpleClass fields initialization */
  {
    0					/* field not used	*/
  },  /* LabelClass fields initialization */
  {
    0					/* field not used	*/
  }   /* CommandClass fields initialization */
};
/* *INDENT-ON* */

  /* for public consumption */
WidgetClass commandWidgetClass = (WidgetClass) & commandClassRec;

/***********************************************************************
 *
 * PaintCommandWidget:	Paints the command widget.
 *	Arguments: 	w	- the command widget.
 *                 	region	- region to paint (passed to the superclass).
 *	Returns:	none
 *
 ***********************************************************************/

static void
PaintCommandWidget(
		      Widget w,
		      Region region)
{
    CommandWidget cbw = (CommandWidget) w;
    int offset = (int) cbw->command.highlight_thickness;
    unsigned int width = (unsigned) (w->core.width - 2 * offset);
    unsigned int height = (unsigned) (w->core.height - 2 * offset);

    (*SuperClass->core_class.expose) (w, (XEvent *) NULL, region);
    if (cbw->command.set)
	XawSunkenRectangle(w, offset, offset, width, height);
    else {
	if (cbw->command.highlightMode == FALSE)
	    XawRaisedRectangle(w, offset, offset, width, height);
    }
}

/***********************************************************************
 *
 * CalcWidth,		Calculate the required width and height of
 * CalcHeight:		the Command widget.
 *	Arguments: 	w - the command widget.
 *	Returns:	The required width or height
 *
 *
 ***********************************************************************/

static Dimension
CalcWidth(CommandWidget cbw)
{
    return (LEFT_OFFSET(cbw) + (unsigned) (cbw->label.label_width +
					   2 * (cbw->label.internal_width +
						cbw->command.highlight_thickness +
						cbw->simple.borderWidth)));
}

static Dimension
CalcHeight(CommandWidget cbw)
{
    return (Dimension) (cbw->label.label_height + 2 *
			(cbw->label.internal_height +
			 cbw->command.highlight_thickness + cbw->simple.borderWidth));
}

/***********************************************************************
 *
 * RepositionLabel:	Calculate the correct position of the label
 *			or bitmap and the left bitmap.
 *	Arguments: 	current	- the current command widget
 *			new	- The new widget with its resources
 *
 *	Returns:	none
 *
 ***********************************************************************/

static void
RepositionLabel(
		   Widget current,
		   Widget new)
{
    CommandWidget cbw = (CommandWidget) new;
    Position newPos, leftedge;

    leftedge = (Position) (LEFT_OFFSET(cbw) + cbw->label.internal_width +
			   cbw->command.highlight_thickness + cbw->simple.borderWidth);
    switch (cbw->label.justify) {
    case XtJustifyLeft:
	newPos = leftedge;
	break;

    case XtJustifyRight:
	newPos = (Position) (current->core.width - cbw->label.label_width -
			     cbw->label.internal_width -
			     cbw->command.highlight_thickness - cbw->simple.borderWidth);
	break;

    case XtJustifyCenter:
    default:
	newPos = (Position) (current->core.width - cbw->label.label_width /* + LEFT_OFFSET(cbw) */
	    ) / 2;
	break;
    }
    /* Justify the label on the left side, if there is not enough space
     * in the widget. Don't do that with pixmaps.
     */
    if ((newPos < leftedge) && (cbw->label.pixmap == None))
	newPos = leftedge;
    cbw->label.label_x = newPos;
    cbw->label.label_y = (Position) ((int) (current->core.height -
				     cbw->label.label_height) / 2);

    /*
     * Left bitmap will be displayed at
     * (internal_width + border width, internal_height + border height + lbm_y)
     */
    if (cbw->label.lbm_height != 0) {
	cbw->label.lbm_y = (int) ((current->core.height -
				  cbw->label.lbm_height) / 2 -
				  cbw->label.internal_height);
	cbw->label.lbm_x = (int) (cbw->label.internal_width +
				  cbw->command.highlight_thickness +
				  cbw->simple.borderWidth);
    } else {
	cbw->label.lbm_y = 0;
	cbw->label.lbm_x = 0;
    }
}

/***********************************************************************
 *
 * DestroyHelpWidget:	Destroy the not longer required helper widget
 *
 ***********************************************************************/

static void
DestroyHelpWidget(CommandWidget w)
{
    if (w->command.helpTimer != 0) {
	XtRemoveTimeOut(w->command.helpTimer);
	w->command.helpTimer = 0;
    }
    if (w->command.helpWidget != NULL) {
	XtDestroyWidget(w->command.helpWidget);
	w->command.helpWidget = NULL;
    }
}

/***********************************************************************
 *
 * CreateHelpWidget:	Create the help widget and set the help text
 *
 ***********************************************************************/

static void
CreateHelpWidget(CommandWidget cw)
{
    cw->command.helpWidget = XtVaCreatePopupShell("helpWindow",
						  overrideShellWidgetClass,
						  (Widget) cw,
						  XtNallowShellResize, TRUE, NULL);

    cw->command.helpLabel = XtVaCreateManagedWidget("helpLabel",
						    labelWidgetClass, cw->command.helpWidget,
						    XtNtruncLabel, False,
						    XtNlabel, cw->command.helpText,
						    XtNbackground,
						    cw->command.helpBackground, NULL);
}

/***********************************************************************
 *
 * PopupHelper:		Popup the helper widget after timeout
 *
 ***********************************************************************/

static void
PopupHelper(
	       XtPointer widget,
	       XtIntervalId * timer GCC_UNUSED)
{
    Widget w = (Widget) widget;
    CommandWidget cw = (CommandWidget) widget;
    Display *display = XtDisplay(w);
    Window root, child;
    int XCoord, YCoord, XRel, YRel, XWidth;
    unsigned int mask;

    /* Determine the position of the helper */

    XWidth = XDisplayWidth(display, XDefaultScreen(display));
    XQueryPointer(display, XtWindow(w), &root, &child, &XCoord, &YCoord,
		  &XRel, &YRel, &mask);
    XCoord += 10;
    YCoord += 10;
    XtVaSetValues(cw->command.helpWidget, XtNx, XCoord, XtNy, YCoord, NULL);
    XtPopup(cw->command.helpWidget, XtGrabNone);

    /* Move the helper window to the left, if the right border is
     * outside of the screen
     */
    if ((XCoord + cw->command.helpWidget->core.width) > XWidth) {
	XCoord = XWidth - cw->command.helpWidget->core.width - 2;
	XtMoveWidget(cw->command.helpWidget, (Position) XCoord, (Position) YCoord);
    }
    cw->command.helpTimer = 0;
}

/***********************************************************************
 *
 * Methods of this Widget
 *
 ***********************************************************************/

/***********************************************************************
 *
 * Initialize method:	Initialize widget instance:
 * 			Replace the underlying GC of the label widget
 *			through the own GC an initialize some private
 *			variables. Resize the widget to get space for
 *			the button decorations.
 *
 ***********************************************************************/

static void
Initialize(
	      Widget request GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    Dimension Add, OldWidth;
    CommandWidget cbw = (CommandWidget) new;

    cbw->command.set = FALSE;

    /* Calculate the correct size of the widget */

    Add = (Dimension) (2 * (cbw->simple.borderWidth + cbw->command.highlight_thickness));
    OldWidth = cbw->core.width;
    cbw->core.width += Add;
    cbw->core.height += Add;
    RepositionLabel(new, new);
    if (cbw->label.truncLabel)
	TruncateLabelString((LabelWidget) new, (OldWidth -
								       LEFT_OFFSET((LabelWidget) new)));

    /* Initialize the help widget, if required */

    cbw->command.helpTimer = 0;
    cbw->command.helpWidget = NULL;
    if ((cbw->command.useHelp == TRUE) && (cbw->command.helpText != NULL))
	CreateHelpWidget(cbw);
}

/***********************************************************************
 *
 * Redisplay method:	Redisplay the widget
 *
 ***********************************************************************/

static void
Redisplay(
	     Widget w,
	     XEvent *event GCC_UNUSED,
	     Region region)
{
    PaintCommandWidget(w, region);
}

/***********************************************************************
 *
 * Destroy method:	Release the GC and destroy the helper widget.
 *			Remove the timeout if set.
 *
 ***********************************************************************/

static void
Destroy(Widget w)
{
    CommandWidget cbw = (CommandWidget) w;
    DestroyHelpWidget(cbw);
}

/***********************************************************************
 *
 * Resize method:	Resize the underlying label widget and paint
 *			the own decorations.
 *
 ***********************************************************************/

static void
Resize(Widget w)
{
    CommandWidget cbw = (CommandWidget) w;
    int offset = (int) cbw->command.highlight_thickness;
    unsigned int width = (unsigned) (w->core.width - 2 * offset);
    unsigned int height = (unsigned) (w->core.height - 2 * offset);

    RepositionLabel(w, w);
    if (cbw->label.truncLabel)
	TruncateLabelString((LabelWidget) w, (width - 2 *
					      cbw->simple.borderWidth -
					      LEFT_OFFSET(cbw)));

    if (XtIsRealized(w) && (cbw->command.highlightMode == FALSE))
	XawRaisedRectangle(w, offset, offset, width, height);
}

/***********************************************************************
 *
 * Set Values method:	Set specified arguments into widget
 *
 ***********************************************************************/

static Boolean
SetValues(
	     Widget current,
	     Widget request GCC_UNUSED,
	     Widget new,
	     ArgList args,
	     Cardinal *num_args)
{
    CommandWidget oldcbw = (CommandWidget) current;
    CommandWidget cbw = (CommandWidget) new;
    Boolean redisplay = FALSE, NoWidthArg = TRUE, NoHeightArg = TRUE,
    DoResize = FALSE;
    int i;

    if (oldcbw->command.useHelp != cbw->command.useHelp) {
	if (cbw->command.useHelp == TRUE)	/* helper becomes activated */
	{
	    if (cbw->command.helpText != NULL)
		CreateHelpWidget(cbw);
	} else
	    DestroyHelpWidget(cbw);	/* helper becomes deactivated */
    }

    if (oldcbw->command.helpText != cbw->command.helpText) {
	/* Help text is changed: If the helper widget is created, we have to change
	 * the text resource. If not, we may be have to create the widget if the
	 * useHelp resource is set to true and the text is not NULL. If the text
	 * changes to NULL, we have to destroy the widget if it exists.
	 */
	if (cbw->command.helpWidget != NULL)
	    if (cbw->command.helpText != NULL)
		XtVaSetValues(cbw->command.helpLabel, XtNlabel,
			      cbw->command.helpText, NULL);
	    else
		DestroyHelpWidget(cbw);
	else {
	    if ((cbw->command.helpText != NULL) && (cbw->command.useHelp == TRUE))
		CreateHelpWidget(cbw);
	}
    }
    if (oldcbw->command.highlight_thickness != cbw->command.highlight_thickness)
	DoResize = TRUE;

    if (oldcbw->core.sensitive != cbw->core.sensitive && !cbw->core.sensitive) {
	cbw->command.set = FALSE;	/* about to become insensitive */
	redisplay = TRUE;
    }

    if (oldcbw->command.highlightMode != cbw->command.highlightMode)
	redisplay = TRUE;

    /* We have to resize width and height if the widget geometry
     * is recalculated by the underlying label. In this case the label shrinks.
     */
    for (i = 0; (Cardinal) i < *num_args; i++) {
	if (strcmp(XtNwidth, args[i].name) == 0)
	    NoWidthArg = FALSE;
	if (strcmp(XtNheight, args[i].name) == 0)
	    NoHeightArg = FALSE;
    }

    if ((oldcbw->core.width != cbw->core.width) && NoWidthArg) {
	cbw->core.width = CalcWidth(cbw);
	DoResize = TRUE;
    }
    if ((oldcbw->core.height != cbw->core.height) && NoHeightArg) {
	cbw->core.height = CalcHeight(cbw);
	DoResize = TRUE;
    }
    if (DoResize || (oldcbw->label.label != cbw->label.label)) {
	RepositionLabel(current, new);
	if (cbw->label.truncLabel)
	    TruncateLabelString((LabelWidget) new, oldcbw->core.width -
				(unsigned) (2 * (cbw->simple.borderWidth +
				cbw->command.highlight_thickness)) -
				LEFT_OFFSET(cbw));

	redisplay = TRUE;
    }

    return (redisplay);
}

/***********************************************************************
 *
 *  Action Procedures
 *
 ***********************************************************************/

/***********************************************************************
 *
 * Set:		Change the button style from raised to sunken for a
 *		pressed button.
 *
 ***********************************************************************/

static void
Set(
       Widget w,
       XEvent *event GCC_UNUSED,
       String *params GCC_UNUSED,
       Cardinal *num_params GCC_UNUSED)
{
    CommandWidget cbw = (CommandWidget) w;
    unsigned int width, height;
    int offset;

    if (cbw->command.set == FALSE) {
	cbw->command.set = TRUE;

	if (XtIsRealized(w)) {
	    if (cbw->command.helpWidget) {
		if (cbw->command.helpTimer == 0)
		    XtPopdown(cbw->command.helpWidget);
		else
		    XtRemoveTimeOut(cbw->command.helpTimer);
		cbw->command.helpTimer = 0;
	    }
	    offset = (int) cbw->command.highlight_thickness;
	    width = (unsigned) (w->core.width - 2 * offset);
	    height = (unsigned) (w->core.height - 2 * offset);

	    XawSunkenRectangle(w, offset, offset, width, height);
	}
    }
}

/***********************************************************************
 *
 * Unset:	Change the button style from sunken to raised for a
 *		released button.
 *
 ***********************************************************************/

static void
Unset(
	 Widget w,
	 XEvent *event GCC_UNUSED,
	 String *params GCC_UNUSED,
	 Cardinal *num_params GCC_UNUSED)
{
    CommandWidget cbw = (CommandWidget) w;
    unsigned int width, height;
    int offset;

    if (cbw->command.set == TRUE) {
	cbw->command.set = FALSE;
	if (XtIsRealized(w)) {
	    offset = (int) cbw->command.highlight_thickness;
	    width = (unsigned) (w->core.width - 2 * offset);
	    height = (unsigned) (w->core.height - 2 * offset);

	    XawRaisedRectangle(w, offset, offset, width, height);
	}
    }
}

/***********************************************************************
 *
 * Reset:	Reset the button style
 *
 ***********************************************************************/

static void
Reset(
	 Widget w,
	 XEvent *event,
	 String *params,
	 Cardinal *num_params)
{
    CommandWidget cbw = (CommandWidget) w;

    if (cbw->command.set)
	Unset(w, event, params, num_params);
    Unhighlight(w, event, params, num_params);
}

/***********************************************************************
 *
 * Highlight:	Highlight the button decoration for the entering
 *		mouse pointer and start the timer for the helper.
 *
 ***********************************************************************/

static void
Highlight(
	     Widget w,
	     XEvent *event GCC_UNUSED,
	     String *params GCC_UNUSED,
	     Cardinal *num_params GCC_UNUSED)
{
    CommandWidget cbw = (CommandWidget) w;
    unsigned int width, height;
    int offset;

    if (XtIsRealized(w)) {
	if ((cbw->command.set == FALSE) &&
	    (cbw->command.highlightMode))	/* New highlight mode */
	{
	    offset = (int) cbw->command.highlight_thickness;
	    width = (unsigned) (w->core.width - 2 * offset);
	    height = (unsigned) (w->core.height - 2 * offset);

	    XawRaisedRectangle(w, offset, offset, width, height);
	}
	if (cbw->command.helpWidget)
	    cbw->command.helpTimer =
		XtAppAddTimeOut(XtWidgetToApplicationContext(w), HELP_TIMER,
				PopupHelper, w);
    }
}

/***********************************************************************
 *
 * Unhighlight:	Unhighlight the button decoration for the leaving
 *		mouse pointer.
 *
 ***********************************************************************/

static void
Unhighlight(
	       Widget w,
	       XEvent *event GCC_UNUSED,
	       String *params GCC_UNUSED,
	       Cardinal *num_params GCC_UNUSED)
{
    CommandWidget cbw = (CommandWidget) w;
    unsigned int width, height;
    int offset;

    if (XtIsRealized(w)) {
	if ((cbw->command.set == FALSE) &&
	    (cbw->command.highlightMode))	/* New highlight mode */
	{
	    offset = (int) cbw->command.highlight_thickness;
	    width = (unsigned) (w->core.width - 2 * offset);
	    height = (unsigned) (w->core.height - 2 * offset);

	    XawFlatRectangle(w, offset, offset, width, height);
	}
	if (cbw->command.helpTimer) {
	    XtRemoveTimeOut(cbw->command.helpTimer);
	    cbw->command.helpTimer = 0;
	} else if (cbw->command.helpWidget)
	    XtPopdown(cbw->command.helpWidget);
    }
}

/***********************************************************************
 *
 * Notify:	Call the Callbacks as result of a pressed button.
 *
 ***********************************************************************/

static void
Notify(
	  Widget w,
	  XEvent *event GCC_UNUSED,
	  String *params GCC_UNUSED,
	  Cardinal *num_params GCC_UNUSED)
{
    CommandWidget cbw = (CommandWidget) w;

    /* check to be sure state is still Set so that user can cancel
       the action (e.g. by moving outside the window, in the default
       bindings.
     */
    if (cbw->command.set)
	XtCallCallbackList(w, cbw->command.callbacks, NULL);
}

/***********************************************************************
 *
 * QueryGeometry:	Check if the geometry request of the manager
 *			widget is acceptable.
 *
 ***********************************************************************/

static XtGeometryResult
QueryGeometry(
		 Widget w,
		 XtWidgetGeometry * intended,
		 XtWidgetGeometry * preferred)
{
    CommandWidget cbw = (CommandWidget) w;
    Boolean widthOk, heightOk;

    /* This is commands preferred width and height */

    preferred->request_mode = CWWidth | CWHeight;
    preferred->width = (Dimension) (cbw->label.label_width + 2 *
				    cbw->label.internal_width + (int)
				    LEFT_OFFSET(cbw) +
				    2 * cbw->simple.borderWidth + 2 * cbw->command.highlight_thickness);
    preferred->height = (Dimension) (cbw->label.label_height + 2 *
				     cbw->label.internal_height +
				     2 * cbw->simple.borderWidth + 2 * cbw->command.highlight_thickness);

    /* Now let us see, if the geometry request is acceptable in any way */

    if ((intended->request_mode & (CWWidth | CWHeight)) == (CWWidth | CWHeight)) {
	widthOk = (intended->width >= preferred->width);
	heightOk = (intended->height >= preferred->height);

	if (widthOk && heightOk)
	    return (XtGeometryYes);
	if (widthOk || heightOk)
	    return (XtGeometryAlmost);
	return (XtGeometryNo);
    }
    return (XtGeometryYes);
}
