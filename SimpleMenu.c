/*
 * $XTermId: SimpleMenu.c,v 1.9 2024/04/28 23:51:14 tom Exp $
 * $Xorg: SimpleMenu.c,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 */

/*
Copyright 2015-2022,2024  Thomas E. Dickey
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
 * SimpleMenu.c - Source code file for SimpleMenu widget.
 *
 * Date:    April 3, 1989
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

#include <X11/XawPlus/Cardinals.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/SimpleMenP.h>
#include <X11/XawPlus/SmeBSB.h>
#include <X11/XawPlus/Simple.h>	/* Resource names needed */

#include <X11/Xmu/Initer.h>
#include <X11/Xmu/CharSet.h>

#define streq(a, b)        ( strcmp((a), (b)) == 0 )

#define offset(field) XtOffsetOf(SimpleMenuRec, simple_menu.field)
#define core_offset(field) XtOffsetOf(SimpleMenuRec, core.field)

static XtResource resources[] =
{

/* Label Resources */

    {XtNlabel, XtCLabel, XtRString, sizeof(String),
     offset(label_string), XtRString, NULL},
    {XtNlabelClass, XtCLabelClass, XtRPointer, sizeof(WidgetClass),
     offset(label_class), XtRImmediate, (XtPointer) NULL},

/* Layout Resources */

    {XtNrowHeight, XtCRowHeight, XtRDimension, sizeof(Dimension),
     offset(row_height), XtRImmediate, (XtPointer) 0},
    {XtNtopMargin, XtCVerticalMargins, XtRDimension, sizeof(Dimension),
     offset(top_margin), XtRImmediate, (XtPointer) 0},
    {XtNbottomMargin, XtCVerticalMargins, XtRDimension, sizeof(Dimension),
     offset(bottom_margin), XtRImmediate, (XtPointer) 0},

/* 3D resources */

    {XtNhighlightColor, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(highlightColor), XtRString, "grey90"},
    {XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
     core_offset(background_pixel), XtRString, "grey75"},
    {XtNshadowColor, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(shadowColor), XtRString, "grey40"},
    {XtNbuttonBorderWidth, XtCWidth, XtRDimension, sizeof(Dimension),
     offset(borderWidth), XtRImmediate, (XtPointer) 2},

/* Misc. Resources */

    {XtNallowShellResize, XtCAllowShellResize, XtRBoolean, sizeof(Boolean),
     XtOffsetOf(SimpleMenuRec, shell.allow_shell_resize),
     XtRImmediate, (XtPointer) TRUE},
    {XtNcursor, XtCCursor, XtRCursor, sizeof(Cursor),
     offset(cursor), XtRImmediate, (XtPointer) None},
    {XtNmenuOnScreen, XtCMenuOnScreen, XtRBoolean, sizeof(Boolean),
     offset(menu_on_screen), XtRImmediate, (XtPointer) TRUE},
    {XtNpopupOnEntry, XtCPopupOnEntry, XtRWidget, sizeof(Widget),
     offset(popup_entry), XtRWidget, NULL},
    {XtNbackingStore, XtCBackingStore, XtRBackingStore, sizeof(int),
     offset(backing_store),
     XtRImmediate, (XtPointer) (Always + WhenMapped + NotUseful)}
};
#undef offset
#undef core_offset
/* *INDENT-OFF* */
static char defaultTranslations[] =
    "<EnterWindow>:     highlight()             \n\
     <LeaveWindow>:     unhighlight()           \n\
     <BtnMotion>:       highlight()             \n\
     <BtnUp>:           MenuPopdown() notify() unhighlight()";

/*
 * Semi Public function definitions.
 */

static void Redisplay(Widget w, XEvent * event, Region region);
static void Realize(Widget w, XtValueMask * mask, XSetWindowAttributes * attrs);
static void Resize(Widget w);
static void ChangeManaged(Widget w);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void ClassInitialize(void);
static void ClassPartInitialize(WidgetClass wc);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static Boolean SetValuesHook(Widget w, ArgList arglist, Cardinal *num_args);
static XtGeometryResult GeometryManager(Widget w, XtWidgetGeometry * request, XtWidgetGeometry * reply);

/*
 * Action Routine Definitions
 */

static void Highlight(Widget w, XEvent * event, String * params, Cardinal * num_params);
static void Unhighlight(Widget w, XEvent * event, String * params, Cardinal * num_params);
static void Notify(Widget w, XEvent * event, String * params, Cardinal * num_params);
static void PositionMenuAction(Widget w, XEvent * event, String * params, Cardinal * num_params);

/*
 * Private Function Definitions.
 */

static void MakeSetValuesRequest(Widget w, Dimension width, Dimension height);
static void CreateLabel(Widget w);
static void Layout(Widget w, Dimension *width_ret, Dimension *height_ret);
static void AddPositionAction(XtAppContext app_con, XPointer data);
static void PositionMenu(Widget w, XPoint * location);
static void Popdown(Widget w, XtPointer clientData, XtPointer callData);
static Dimension GetMenuWidth(Widget w, Widget w_ent);
static Dimension GetMenuHeight(Widget w);
static Widget FindMenu(Widget widget, String name);
static SmeObject GetEventEntry(Widget w, XEvent * event);
static void MoveMenu(Widget w, Position x, Position y);
static void DrawMenu(Widget w);

static XtActionsRec actionsList[] =
{
  {"notify",            Notify},
  {"highlight",         Highlight},
  {"unhighlight",       Unhighlight},
};

static CompositeClassExtensionRec extension_rec = {
    /* next_extension */  NULL,
    /* record_type */     NULLQUARK,
    /* version */         XtCompositeExtensionVersion,
    /* record_size */     sizeof(CompositeClassExtensionRec),
    /* accepts_objects */ TRUE,
    /* allows_change_managed_set */ FALSE,
};

#define superclass (&overrideShellClassRec)

SimpleMenuClassRec simpleMenuClassRec = {
  {
    /* superclass         */    (WidgetClass) superclass,
    /* class_name         */    "SimpleMenu",
    /* size               */    sizeof(SimpleMenuRec),
    /* class_initialize   */	ClassInitialize,
    /* class_part_initialize*/	ClassPartInitialize,
    /* Class init'ed      */	FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */	NULL,
    /* realize            */    Realize,
    /* actions            */    actionsList,
    /* num_actions        */    XtNumber(actionsList),
    /* resources          */    resources,
    /* resource_count     */	XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    TRUE,
    /* compress_exposure  */    XtExposeCompressSeries,
    /* compress_enterleave*/ 	TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    Resize,
    /* expose             */    Redisplay,
    /* set_values         */    SetValues,
    /* set_values_hook    */	SetValuesHook,
    /* set_values_almost  */	XtInheritSetValuesAlmost,
    /* get_values_hook    */	NULL,
    /* accept_focus       */    NULL,
    /* intrinsics version */	XtVersion,
    /* callback offsets   */    NULL,
    /* tm_table		  */    defaultTranslations,
    /* query_geometry	  */    NULL,
    /* display_accelerator*/    NULL,
    /* extension	  */    NULL
  },{
    /* geometry_manager   */    GeometryManager,
    /* change_managed     */    ChangeManaged,
    /* insert_child	  */	XtInheritInsertChild,
    /* delete_child	  */	XtInheritDeleteChild,
    /* extension	  */    NULL
  },{
    /* Shell extension	  */    NULL
  },{
    /* Override extension */    NULL
  },{
    /* Simple Menu extension*/  NULL
  }
};
/* *INDENT-ON* */

WidgetClass simpleMenuWidgetClass = (WidgetClass) & simpleMenuClassRec;

#define ForAllChildren(smw, childP) \
  for ( (childP) = (SmeObject *) (smw)->composite.children ; \
        (childP) < (SmeObject *) ( (smw)->composite.children + \
				 (smw)->composite.num_children ) ; \
        (childP)++ )

/************************************************************
 *
 * Semi-Public Functions.
 *
 ************************************************************/

/*      Function Name: ClassInitialize
 *      Description: Class Initialize routine, called only once.
 *      Arguments: none.
 *      Returns: none.
 */

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

    XawInitializeWidgetSet();
    XtAddConverter(XtRString, XtRBackingStore, XmuCvtStringToBackingStore,
		   NULL, 0);
    XtSetTypeConverter(XtRString, XtRPixel, XtCvtStringToPixel, colConvertArg,
		       XtNumber(colConvertArg), XtCacheByDisplay, NULL);
    XmuAddInitializer(AddPositionAction, NULL);
}

/*      Function Name: ClassPartInitialize
 *      Description: Class Part Initialize routine, called for every
 *                   subclass.  Makes sure that the subclasses pick up
 *                   the extension record.
 *      Arguments: wc - the widget class of the subclass.
 *      Returns: none.
 */

static void
ClassPartInitialize(WidgetClass wc)
{
    SimpleMenuWidgetClass smwc = (SimpleMenuWidgetClass) wc;

/*
 * Make sure that our subclass gets the extension rec too.
 */

    extension_rec.next_extension = smwc->composite_class.extension;
    smwc->composite_class.extension = (XtPointer) &extension_rec;
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
    XGCValues values;
    SimpleMenuWidget smw = (SimpleMenuWidget) new;

    XmuCallInitializers(XtWidgetToApplicationContext(new));

    if (smw->simple_menu.label_class == NULL)
	smw->simple_menu.label_class = smeBSBObjectClass;

    smw->simple_menu.label = NULL;
    smw->simple_menu.entry_set = NULL;
    smw->simple_menu.recursive_set_values = FALSE;

    if (smw->simple_menu.label_string != NULL)
	CreateLabel(new);

    smw->simple_menu.menu_width = TRUE;

    if (smw->core.width == 0) {
	smw->simple_menu.menu_width = FALSE;
	smw->core.width = GetMenuWidth(new, (Widget) NULL);
    }

    smw->simple_menu.menu_height = TRUE;

    if (smw->core.height == 0) {
	smw->simple_menu.menu_height = FALSE;
	smw->core.height = GetMenuHeight(new);
    }
    /* Get the required GCs */

    values.foreground = smw->core.background_pixel;
    smw->simple_menu.backgrGC = XtGetGC(new, GCForeground, &values);

    values.foreground = smw->simple_menu.highlightColor;
    smw->simple_menu.highlightGC = XtGetGC(new, GCForeground, &values);

    values.foreground = smw->simple_menu.shadowColor;
    smw->simple_menu.shadowGC = XtGetGC(new, GCForeground, &values);

/*
 * Add a popdown callback to ungrab the pointer
 */
    XtAddCallback(new, XtNpopdownCallback, Popdown, (XtPointer) NULL);
}

/*      Function Name: Redisplay
 *      Description: Redisplays the contents of the widget.
 *      Arguments: w - the simple menu widget.
 *                 event - the X event that caused this redisplay.
 *                 region - the region the needs to be repainted.
 *      Returns: none.
 */

static void
Redisplay(
	     Widget w,
	     XEvent *event GCC_UNUSED,
	     Region region)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject *entry;
    SmeObjectClass class;

    DrawMenu(w);

    /*
     * Check and Paint each of the entries - including the label.
     */

    ForAllChildren(smw, entry) {
	if (!XtIsManaged((Widget) *entry))
	    continue;

	if (region != NULL)
	    switch (XRectInRegion(region, (int) (*entry)->rectangle.x,
				  (int) (*entry)->rectangle.y,
				  (unsigned int) (*entry)->rectangle.width,
				  (unsigned int) (*entry)->rectangle.height)) {
	    case RectangleIn:
	    case RectanglePart:
		break;
	    default:
		continue;
	    }
	class = (SmeObjectClass) (*entry)->object.widget_class;

	if (class->rect_class.expose != NULL)
	    (class->rect_class.expose) ((Widget) *entry, NULL, NULL);
    }
    /* For any reason we don't get pointer events, if modifiers like
     * like Lock (key CapsLock)  or Mod2 (key NumLock) are set.
     * Therefore I add a grab here to get all the events. As a side
     * effect I set the cursor here. The previously used callback
     * is not longer necessary and therefore removed.
     */

    (void) XtGrabPointer(w, FALSE, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
			 GrabModeAsync, GrabModeAsync, None,
			 smw->simple_menu.cursor, CurrentTime);
}

/*      Function Name: Realize
 *      Description: Realizes the widget.
 *      Arguments: w - the simple menu widget.
 *                 mask - value mask for the window to create.
 *                 attrs - attributes for the window to create.
 *      Returns: none
 */

static void
Realize(
	   Widget w,
	   XtValueMask * mask,
	   XSetWindowAttributes * attrs)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;

    attrs->cursor = smw->simple_menu.cursor;
    *mask |= CWCursor;
    if ((smw->simple_menu.backing_store == Always) ||
	(smw->simple_menu.backing_store == NotUseful) ||
	(smw->simple_menu.backing_store == WhenMapped)) {
	*mask |= CWBackingStore;
	attrs->backing_store = smw->simple_menu.backing_store;
    } else
	*mask &= (XtValueMask) ~ CWBackingStore;

    /* Realize methods are not chained! Therefore we
     * have to call the realize method of our superclass
     * here to create a window.
     */
    (*superclass->core_class.realize) (w, mask, attrs);
}

/*      Function Name: Resize
 *      Description: Handle the menu being resized bigger.
 *      Arguments: w - the simple menu widget.
 *      Returns: none.
 */

static void
Resize(Widget w)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject *entry;

    if (!XtIsRealized(w))
	return;

    ForAllChildren(smw, entry)	/* reset width of all entries. */
	if (XtIsManaged((Widget) *entry))
	(*entry)->rectangle.width = smw->core.width;

    Redisplay(w, (XEvent *) NULL, (Region) NULL);
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
    SimpleMenuWidget smw_old = (SimpleMenuWidget) current;
    SimpleMenuWidget smw_new = (SimpleMenuWidget) new;
    Display *Dpy = XtDisplay(current);
    Boolean ret_val = FALSE, layout = FALSE;

    if (!XtIsRealized(current))
	return (FALSE);

    if (!smw_new->simple_menu.recursive_set_values) {
	if (smw_new->core.width != smw_old->core.width) {
	    smw_new->simple_menu.menu_width = (smw_new->core.width != 0);
	    layout = TRUE;
	}
	if (smw_new->core.height != smw_old->core.height) {
	    smw_new->simple_menu.menu_height = (smw_new->core.height != 0);
	    layout = TRUE;
	}
    }

    if (smw_old->simple_menu.cursor != smw_new->simple_menu.cursor)
	XDefineCursor(XtDisplay(new),
		      XtWindow(new), smw_new->simple_menu.cursor);

    if (smw_old->simple_menu.label_string != smw_new->simple_menu.label_string) {
	if (smw_new->simple_menu.label_string == NULL)	/* Destroy. */
	    XtDestroyWidget((Widget) smw_old->simple_menu.label);
	else if (smw_old->simple_menu.label_string == NULL)	/* Create. */
	    CreateLabel(new);
	else {			/* Change. */
	    Arg arglist[1];

	    XtSetArg(arglist[0], XtNlabel, smw_new->simple_menu.label_string);
	    XtSetValues((Widget) smw_new->simple_menu.label, arglist, ONE);
	}
    }

    if (smw_old->simple_menu.label_class != smw_new->simple_menu.label_class)
	XtAppWarning(XtWidgetToApplicationContext(new),
		     "No Dynamic class change of the SimpleMenu Label.");

    if ((smw_old->simple_menu.top_margin != smw_new->simple_menu.top_margin) ||
	(smw_old->simple_menu.bottom_margin !=
	 smw_new->simple_menu.bottom_margin) /* filler.................  */ ) {
	layout = TRUE;
	ret_val = TRUE;
    }

    /* Check, if any color has changed. If so, free the old color cell and
     * store the new color in the GC.
     */
    if (smw_old->simple_menu.highlightColor != smw_new->simple_menu.highlightColor) {
	XSetForeground(Dpy, smw_new->simple_menu.highlightGC, smw_new->simple_menu.highlightColor);
	ret_val = TRUE;
    }
    if (smw_old->core.background_pixel != smw_new->core.background_pixel) {
	XSetForeground(Dpy, smw_new->simple_menu.backgrGC, smw_new->core.background_pixel);
	ret_val = TRUE;
    }
    if (smw_old->simple_menu.shadowColor != smw_new->simple_menu.shadowColor) {
	XSetForeground(Dpy, smw_new->simple_menu.shadowGC, smw_new->simple_menu.shadowColor);
	ret_val = TRUE;
    }
    /* Check, if the border width has changed */

    if (smw_old->simple_menu.borderWidth !=
	smw_new->simple_menu.borderWidth) ret_val = TRUE;

    if (layout)
	Layout(new, NULL, NULL);
    return (ret_val);
}

/*      Function Name: SetValuesHook
 *      Description: To handle a special case, this is passed the
 *                   actual arguments.
 *      Arguments: w - the menu widget.
 *                 arglist - the argument list passed to XtSetValues.
 *                 num_args - the number of args.
 *      Returns: none
 */

/*
 * If the user actually passed a width and height to the widget
 * then this MUST be used, rather than our newly calculated width and
 * height.
 */

static Boolean
SetValuesHook(
		 Widget w,
		 ArgList arglist,
		 Cardinal *num_args)
{
    Cardinal i;
    Dimension width, height;

    width = w->core.width;
    height = w->core.height;

    for (i = 0; i < *num_args; i++) {
	if (streq(arglist[i].name, XtNwidth))
	    width = (Dimension) arglist[i].value;
	if (streq(arglist[i].name, XtNheight))
	    height = (Dimension) arglist[i].value;
    }

    if ((width != w->core.width) || (height != w->core.height))
	MakeSetValuesRequest(w, width, height);
    return (FALSE);
}

/************************************************************
 *
 * Geometry Management routines.
 *
 ************************************************************/

/*	Function Name: GeometryManager
 *	Description: This is the SimpleMenu Widget's Geometry Manager.
 *	Arguments: w - the Menu Entry making the request.
 *                 request - requested new geometry.
 *                 reply - the allowed geometry.
 *	Returns: XtGeometry{Yes, No, Almost}.
 */

static XtGeometryResult
GeometryManager(
		   Widget w,
		   XtWidgetGeometry * request,
		   XtWidgetGeometry * reply)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) XtParent(w);
    SmeObject entry = (SmeObject) w;
    XtGeometryMask mode = request->request_mode;
    XtGeometryResult answer;
    Dimension old_height, old_width;

    if (!(mode & CWWidth) && !(mode & CWHeight))
	return (XtGeometryNo);

    reply->width = request->width;
    reply->height = request->height;

    old_width = entry->rectangle.width;
    old_height = entry->rectangle.height;

    Layout(w, &(reply->width), &(reply->height));

/*
 * Since we are an override shell and have no parent there is no one to
 * ask to see if this geom change is okay, so I am just going to assume
 * we can do whatever we want.  If you subclass be very careful with this
 * assumption, it could bite you.
 *
 * Chris D. Peterson - Sept. 1989.
 */

    if ((reply->width == request->width) &&
	(reply->height == request->height)) {

	if (mode & XtCWQueryOnly) {	/* Actually perform the layout. */
	    entry->rectangle.width = old_width;
	    entry->rectangle.height = old_height;
	} else {
	    Layout((Widget) smw, (Dimension *) NULL, (Dimension *) NULL);
	}
	answer = XtGeometryDone;
    } else {
	entry->rectangle.width = old_width;
	entry->rectangle.height = old_height;

	if (((reply->width == request->width) && !(mode & CWHeight)) ||
	    ((reply->height == request->height) && !(mode & CWWidth)) ||
	    ((reply->width == request->width) &&
	     (reply->height == request->height)))
	    answer = XtGeometryNo;
	else {
	    answer = XtGeometryAlmost;
	    reply->request_mode = 0;
	    if (reply->width != request->width)
		reply->request_mode |= CWWidth;
	    if (reply->height != request->height)
		reply->request_mode |= CWHeight;
	}
    }
    return (answer);
}

/*	Function Name: ChangeManaged
 *	Description: called whenever a new child is managed.
 *	Arguments: w - the simple menu widget.
 *	Returns: none.
 */

static void
ChangeManaged(Widget w)
{
    Layout(w, (Dimension *) NULL, (Dimension *) NULL);
}

/************************************************************
 *
 * Global Action Routines.
 *
 * These actions routines will be added to the application's
 * global action list.
 *
 ************************************************************/

/*      Function Name: PositionMenuAction
 *      Description: Positions the simple menu widget.
 *      Arguments: w - a widget (no the simple menu widget.)
 *                 event - the event that caused this action.
 *                 params, num_params - parameters passed to the routine.
 *                                      we expect the name of the menu here.
 *      Returns: none
 */

static void
PositionMenuAction(
		      Widget w,
		      XEvent *event,
		      String *params,
		      Cardinal *num_params)
{
    Widget menu;
    XPoint loc;
    char error_buf[BUFSIZ];
    char *ebp;
    int len;
    char *fmt = "Xaw SimpleMenuWidget: could not find menu named: \"%s\"";

    if (*num_params != 1) {
	XtAppWarning(XtWidgetToApplicationContext(w),
		     "Xaw - SimpleMenuWidget: position menu action expects only one\n"
		     "parameter, which is the name of the menu.");
	return;
    }

    if ((menu = FindMenu(w, params[0])) == NULL) {
	if ((size_t) (len = (int) (strlen(fmt) + strlen(params[0]))) <
	    sizeof error_buf)
	    ebp = error_buf;
	else
	    ebp = XtMalloc((Cardinal) (len + 1));
	if (ebp == NULL) {
	    ebp = error_buf;
	    strcpy(ebp, "Xaw - SimpleMenuWidget: could not find menu");
	} else
	    (void) sprintf(ebp, fmt, params[0]);

	XtAppWarning(XtWidgetToApplicationContext(w), ebp);
	if (ebp != error_buf)
	    XtFree(ebp);
	return;
    }

    switch (event->type) {
    case ButtonPress:
    case ButtonRelease:
	loc.x = (short) event->xbutton.x_root;
	loc.y = (short) event->xbutton.y_root;
	PositionMenu(menu, &loc);
	break;
    case EnterNotify:
    case LeaveNotify:
	loc.x = (short) event->xcrossing.x_root;
	loc.y = (short) event->xcrossing.y_root;
	PositionMenu(menu, &loc);
	break;
    case MotionNotify:
	loc.x = (short) event->xmotion.x_root;
	loc.y = (short) event->xmotion.y_root;
	PositionMenu(menu, &loc);
	break;
    default:
	PositionMenu(menu, (XPoint *) NULL);
	break;
    }
}

/************************************************************
 *
 * Widget Action Routines.
 *
 ************************************************************/

/*      Function Name: Unhighlight
 *      Description: Unhighlights current entry.
 *      Arguments: w - the simple menu widget.
 *                 event - the event that caused this action.
 *                 params, num_params - ** NOT USED **
 *      Returns: none
 */

static void
Unhighlight(
	       Widget w,
	       XEvent *event GCC_UNUSED,
	       String *params GCC_UNUSED,
	       Cardinal *num_params GCC_UNUSED)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject entry = smw->simple_menu.entry_set;
    SmeObjectClass class;

    if (entry == NULL)
	return;

    smw->simple_menu.entry_set = NULL;
    class = (SmeObjectClass) entry->object.widget_class;
    (class->sme_class.unhighlight) ((Widget) entry);
}

/*      Function Name: Highlight
 *      Description: Highlights current entry.
 *      Arguments: w - the simple menu widget.
 *                 event - the event that caused this action.
 *                 params, num_params - ** NOT USED **
 *      Returns: none
 */

static void
Highlight(
	     Widget w,
	     XEvent *event,
	     String *params,
	     Cardinal *num_params)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject entry;
    SmeObjectClass class;

    if (!XtIsSensitive(w))
	return;

    entry = GetEventEntry(w, event);

    if (entry == smw->simple_menu.entry_set)
	return;

    Unhighlight(w, event, params, num_params);

    if (entry == NULL)
	return;

    if (!XtIsSensitive((Widget) entry)) {
	smw->simple_menu.entry_set = NULL;
	return;
    }

    smw->simple_menu.entry_set = entry;
    class = (SmeObjectClass) entry->object.widget_class;

    (class->sme_class.highlight) ((Widget) entry);
}

/*      Function Name: Notify
 *      Description: Notify user of current entry.
 *      Arguments: w - the simple menu widget.
 *                 event - the event that caused this action.
 *                 params, num_params - ** NOT USED **
 *      Returns: none
 */

static void
Notify(
	  Widget w,
	  XEvent *event GCC_UNUSED,
	  String *params GCC_UNUSED,
	  Cardinal *num_params GCC_UNUSED)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject entry = smw->simple_menu.entry_set;
    SmeObjectClass class;

    if ((entry == NULL) || !XtIsSensitive((Widget) entry))
	return;

    class = (SmeObjectClass) entry->object.widget_class;
    (class->sme_class.notify) ((Widget) entry);
}

/************************************************************
 *
 * Public Functions.
 *
 ************************************************************/

/*	Function Name: XawSimpleMenuAddGlobalActions
 *	Description: adds the global actions to the simple menu widget.
 *	Arguments: app_con - the appcontext.
 *	Returns: none.
 */

void
XawSimpleMenuAddGlobalActions(XtAppContext app_con)
{
    XtInitializeWidgetClass(simpleMenuWidgetClass);
    XmuCallInitializers(app_con);
}

/*	Function Name: XawSimpleMenuGetActiveEntry
 *	Description: Gets the currently active (set) entry.
 *	Arguments: w - the smw widget.
 *	Returns: the currently set entry or NULL if none is set.
 */

Widget
XawSimpleMenuGetActiveEntry(Widget w)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;

    return ((Widget) smw->simple_menu.entry_set);
}

/*	Function Name: XawSimpleMenuClearActiveEntry
 *	Description: Unsets the currently active (set) entry.
 *	Arguments: w - the smw widget.
 *	Returns: none.
 */

void
XawSimpleMenuClearActiveEntry(Widget w)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;

    smw->simple_menu.entry_set = NULL;
}

/************************************************************
 *
 * Private Functions.
 *
 ************************************************************/

/*	Function Name: CreateLabel
 *	Description: Creates a the menu label.
 *	Arguments: w - the smw widget.
 *	Returns: none.
 *
 * Creates the label object and makes sure it is the first child in
 * in the list.
 */

static void
CreateLabel(Widget w)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    Widget *child, *next_child;
    int i;
    Arg args[2];

    if ((smw->simple_menu.label_string == NULL) || (smw->simple_menu.label
						    != NULL)) {
	XtAppWarning(XtWidgetToApplicationContext(w),
		     "Xaw Simple Menu Widget: Label string is NULL or label already exists.\n"
		     "No label created.");
	return;
    }

    XtSetArg(args[0], XtNlabel, smw->simple_menu.label_string);
    XtSetArg(args[1], XtNjustify, XtJustifyCenter);
    smw->simple_menu.label = (SmeObject)
	XtCreateManagedWidget("menuLabel",
			      smw->simple_menu.label_class, w,
			      args, TWO);

    next_child = NULL;
    for (child = smw->composite.children + smw->composite.num_children,
	 i = (int) smw->composite.num_children; i > 0; i--, child--) {
	if (next_child != NULL)
	    *next_child = *child;
	next_child = child;
    }
    *child = (Widget) smw->simple_menu.label;
}

/*	Function Name: Layout
 *	Description: lays the menu entries out all nice and neat.
 *	Arguments: w - See below (+++)
 *                 width_ret, height_ret - The returned width and
 *                                         height values.
 *	Returns: none.
 *
 * if width == NULL || height == NULL then it assumes the you do not care
 * about the return values, and just want a relayout.
 *
 * if this is not the case then it will set width_ret and height_ret
 * to be width and height that the child would get if it were layed out
 * at this time.
 *
 * +++ "w" can be the simple menu widget or any of its object children.
 */

static void
Layout(
	  Widget w,
	  Dimension *width_ret,
	  Dimension *height_ret)
{
    SmeObject current_entry, *entry;
    SimpleMenuWidget smw;
    Dimension width, height;
    Boolean do_layout = ((height_ret == NULL) || (width_ret == NULL));
    Boolean allow_change_size;
    height = 0;

    if (XtIsSubclass(w, simpleMenuWidgetClass)) {
	smw = (SimpleMenuWidget) w;
	current_entry = NULL;
    } else {
	smw = (SimpleMenuWidget) XtParent(w);
	current_entry = (SmeObject) w;
    }

    allow_change_size = (!XtIsRealized((Widget) smw) ||
			 (smw->shell.allow_shell_resize));

    if (smw->simple_menu.menu_height)
	height = smw->core.height;
    else if (do_layout) {
	height = smw->simple_menu.top_margin + smw->simple_menu.borderWidth;
	ForAllChildren(smw, entry) {
	    if (!XtIsManaged((Widget) *entry))
		continue;

	    if ((smw->simple_menu.row_height != 0) &&
		(*entry != smw->simple_menu.label))
		(*entry)->rectangle.height = smw->simple_menu.row_height;

	    (*entry)->rectangle.y = (Position) height;
	    (*entry)->rectangle.x = (Position) smw->simple_menu.borderWidth;
	    height += (*entry)->rectangle.height;
	}
	height += (Dimension) (smw->simple_menu.bottom_margin + smw->simple_menu.borderWidth);
    } else {
	if ((smw->simple_menu.row_height != 0) &&
	    (current_entry != smw->simple_menu.label))
	    height = smw->simple_menu.row_height;
    }

    if (smw->simple_menu.menu_width)
	width = smw->core.width;
    else if (allow_change_size)
	width = GetMenuWidth((Widget) smw, (Widget) current_entry);
    else
	width = smw->core.width;

    if (do_layout) {
	ForAllChildren(smw, entry)
	    if (XtIsManaged((Widget) *entry))
	    (*entry)->rectangle.width = (Dimension) (width - 2 * smw->simple_menu.borderWidth);

	if (allow_change_size)
	    MakeSetValuesRequest((Widget) smw, width, height);
    } else {
	*width_ret = width;
	if (height != 0)
	    *height_ret = height;
    }
}

/*	Function Name: AddPositionAction
 *	Description: Adds the XawPositionSimpleMenu action to the global
 *                   action list for this appcon.
 *	Arguments: app_con - the application context for this app.
 *                 data - NOT USED.
 *	Returns: none.
 */

static void
AddPositionAction(
		     XtAppContext app_con,
		     XPointer data GCC_UNUSED)
{
    static XtActionsRec pos_action[] =
    {
	{"XawPositionSimpleMenu", PositionMenuAction},
    };

    XtAppAddActions(app_con, pos_action, XtNumber(pos_action));
}

/*	Function Name: FindMenu
 *	Description: Find the menu give a name and reference widget.
 *	Arguments: widget - reference widget.
 *                 name   - the menu widget's name.
 *	Returns: the menu widget or NULL.
 */

static Widget
FindMenu(
	    Widget widget,
	    String name)
{
    Widget w, menu;

    for (w = widget; w != NULL; w = XtParent(w))
	if ((menu = XtNameToWidget(w, name)) != NULL)
	    return (menu);
    return (NULL);
}

/*	Function Name: PositionMenu
 *	Description: Places the menu
 *	Arguments: w - the simple menu widget.
 *                 location - a pointer the the position or NULL.
 *	Returns: none.
 */

static void
PositionMenu(
		Widget w,
		XPoint * location)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject entry;
    XPoint t_point;

    if (location == NULL) {
	Window junk1, junk2;
	int root_x, root_y, junkX, junkY;
	unsigned int junkM;

	location = &t_point;
	if (XQueryPointer(XtDisplay(w), XtWindow(w), &junk1, &junk2,
			  &root_x, &root_y, &junkX, &junkY, &junkM) == FALSE) {
	    XtAppWarning(XtWidgetToApplicationContext(w),
			 "Xaw Simple Menu Widget: Could not find location of mouse pointer");
	    return;
	}
	location->x = (short) root_x;
	location->y = (short) root_y;
    }

    /*
     * The width will not be correct unless it is realized.
     */

    XtRealizeWidget(w);

    location->x -= (Position) w->core.width / 2;

    if (smw->simple_menu.popup_entry == NULL)
	entry = smw->simple_menu.label;
    else
	entry = smw->simple_menu.popup_entry;

    if (entry != NULL)
	location->y -= (short) (entry->rectangle.y + entry->rectangle.height
				/ 2);

    MoveMenu(w, (Position) location->x, (Position) location->y);
}

/*	Function Name: MoveMenu
 *	Description: Actually moves the menu, may force it to
 *                   to be fully visable if menu_on_screen is TRUE.
 *	Arguments: w - the simple menu widget.
 *                 x, y - the current location of the widget.
 *	Returns: none
 */

static void
MoveMenu(Widget w, Position x, Position y)
{
    Arg arglist[2];
    Cardinal num_args = 0;
    SimpleMenuWidget smw = (SimpleMenuWidget) w;

    if (smw->simple_menu.menu_on_screen) {
	int width = w->core.width + 2 * w->core.border_width;
	int height = w->core.height + 2 * w->core.border_width;

	if (x >= 0) {
	    int scr_width = WidthOfScreen(XtScreen(w));
	    if (x + width > scr_width)
		x = (Position) (scr_width - width);
	}
	if (x < 0)
	    x = 0;

	if (y >= 0) {
	    int scr_height = HeightOfScreen(XtScreen(w));
	    if (y + height > scr_height)
		y = (Position) (scr_height - height);
	}
	if (y < 0)
	    y = 0;
    }

    XtSetArg(arglist[num_args], XtNx, x);
    num_args++;
    XtSetArg(arglist[num_args], XtNy, y);
    num_args++;
    XtSetValues(w, arglist, num_args);
}

/*	Function Name: Popdown
 *	Description: Removes the pointer grab
 *	Arguments: w - the widget
 *	Returns: None.
 */

static void
Popdown(Widget w,
	XtPointer clientData GCC_UNUSED,
	XtPointer callData GCC_UNUSED)
{
    XtUngrabPointer(w, CurrentTime);
}

/*      Function Name: MakeSetValuesRequest
 *      Description: Makes a (possibly recursive) call to SetValues,
 *                   I take great pains to not go into an infinite loop.
 *      Arguments: w - the simple menu widget.
 *                 width, height - the size of the ask for.
 *      Returns: none
 */

static void
MakeSetValuesRequest(
			Widget w,
			Dimension width,
			Dimension height)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    Arg arglist[2];
    Cardinal num_args = (Cardinal) 0;

    if (!smw->simple_menu.recursive_set_values) {
	if ((smw->core.width != width) || (smw->core.height != height)) {
	    smw->simple_menu.recursive_set_values = TRUE;
	    XtSetArg(arglist[num_args], XtNwidth, width);
	    num_args++;
	    XtSetArg(arglist[num_args], XtNheight, height);
	    num_args++;
	    XtSetValues(w, arglist, num_args);
	} else if (XtIsRealized((Widget) smw))
	    Redisplay((Widget) smw, (XEvent *) NULL, (Region) NULL);
    }
    smw->simple_menu.recursive_set_values = FALSE;
}

/*      Function Name: GetMenuWidth
 *      Description: Sets the length of the widest entry in pixels.
 *      Arguments: w - the simple menu widget.
 *      Returns: width of menu.
 */

static Dimension
GetMenuWidth(Widget w, Widget w_ent)
{
    SmeObject cur_entry = (SmeObject) w_ent;
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    Dimension width, widest = (Dimension) 0;
    SmeObject *entry;

    if (smw->simple_menu.menu_width)
	return (smw->core.width);

    ForAllChildren(smw, entry) {
	XtWidgetGeometry preferred;

	if (!XtIsManaged((Widget) *entry))
	    continue;

	if (*entry != cur_entry) {
	    XtQueryGeometry((Widget) *entry, (XtWidgetGeometry *) NULL, &preferred);

	    if (preferred.request_mode & CWWidth)
		width = preferred.width;
	    else
		width = (*entry)->rectangle.width;
	} else
	    width = (*entry)->rectangle.width;

	if (width > widest)
	    widest = width;
    }

    return (widest);
}

/*      Function Name: GetMenuHeight
 *      Description: Sets the length of the widest entry in pixels.
 *      Arguments: w - the simple menu widget.
 *      Returns: width of menu.
 */

static Dimension
GetMenuHeight(Widget w)
{
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject *entry;
    Dimension height;

    if (smw->simple_menu.menu_height)
	return (smw->core.height);

    height = smw->simple_menu.top_margin + smw->simple_menu.bottom_margin;

    if (smw->simple_menu.row_height == 0) {
	ForAllChildren(smw, entry)
	    if (XtIsManaged((Widget) *entry))
	    height += (*entry)->rectangle.height;
    } else
	height += (Dimension) (smw->simple_menu.row_height * smw->composite.num_children);

    return (height);
}

/*      Function Name: GetEventEntry
 *      Description: Gets an entry given an event that has X and Y coords.
 *      Arguments: w - the simple menu widget.
 *                 event - the event.
 *      Returns: the entry that this point is in.
 */

static SmeObject
GetEventEntry(
		 Widget w,
		 XEvent *event)
{
    Position x_loc, y_loc;
    SimpleMenuWidget smw = (SimpleMenuWidget) w;
    SmeObject *entry;

    switch (event->type) {
    case MotionNotify:
	x_loc = (Position) event->xmotion.x;
	y_loc = (Position) event->xmotion.y;
	break;
    case EnterNotify:
    case LeaveNotify:
	x_loc = (Position) event->xcrossing.x;
	y_loc = (Position) event->xcrossing.y;
	break;
    case ButtonPress:
    case ButtonRelease:
	x_loc = (Position) event->xbutton.x;
	y_loc = (Position) event->xbutton.y;
	break;
    default:
	XtAppError(XtWidgetToApplicationContext(w),
		   "Unknown event type in GetEventEntry().");
	return (NULL);
    }

    if ((x_loc < 0) || (x_loc >= (int) smw->core.width) || (y_loc < 0) ||
	(y_loc >= (int) smw->core.height))
	return (NULL);

    ForAllChildren(smw, entry) {
	if (!XtIsManaged((Widget) *entry))
	    continue;

	if (((*entry)->rectangle.y < y_loc) &&
	    ((*entry)->rectangle.y + (int) (*entry)->rectangle.height > y_loc)) {
	    if (*entry == smw->simple_menu.label)
		return (NULL);	/* cannot select the label. */
	    else
		return (*entry);
	}
    }

    return (NULL);
}

/*      Function Name:	DrawMenu
 *      Description:	Draw the menu background
 *      Arguments:	w - the simple menu widget.
 *      Returns:	none
 */

static void
DrawMenu(Widget w)
{
    SimpleMenuWidget simpleMenu = (SimpleMenuWidget) w;
    int xMin, xMax, yMin, yMax, i;
    Display *Dpy = XtDisplay(w);
    Drawable Win = simpleMenu->core.window;

    XFillRectangle(Dpy, Win, simpleMenu->simple_menu.backgrGC,
		   0, 0, simpleMenu->core.width, simpleMenu->core.height);

    /* Calculate points for upper horizontal border an draw it */

    xMin = 0;
    yMin = 0;
    xMax = simpleMenu->core.width - 2;
    for (i = 0; i < simpleMenu->simple_menu.borderWidth; i++) {
	XDrawLine(Dpy, Win, simpleMenu->simple_menu.highlightGC, xMin, yMin,
		  xMax, yMin);
	xMin++;
	xMax--;
	yMin++;
    }
    /* Calculate points for left vertical border an draw it */

    xMin = 0;
    yMin = 1;
    yMax = simpleMenu->core.height - 2;
    for (i = 0; i < simpleMenu->simple_menu.borderWidth; i++) {
	XDrawLine(Dpy, Win, simpleMenu->simple_menu.highlightGC, xMin, yMin,
		  xMin, yMax);
	xMin++;
	yMin++;
	yMax--;
    }
    /* Calculate points for horizontal lower border an draw it */

    xMin = simpleMenu->simple_menu.borderWidth;
    xMax = simpleMenu->core.width - simpleMenu->simple_menu.borderWidth;
    yMin = simpleMenu->core.height - simpleMenu->simple_menu.borderWidth;
    for (i = 0; i < simpleMenu->simple_menu.borderWidth; i++) {
	XDrawLine(Dpy, Win, simpleMenu->simple_menu.shadowGC, xMin, yMin,
		  xMax, yMin);
	xMin--;
	xMax++;
	yMin++;
    }
    /* Calculate points for right vertical border an draw it */

    xMin = simpleMenu->core.width - simpleMenu->simple_menu.borderWidth;
    yMin = simpleMenu->simple_menu.borderWidth;
    yMax = simpleMenu->core.height - 1 - simpleMenu->simple_menu.borderWidth;
    for (i = 0; i < simpleMenu->simple_menu.borderWidth; i++) {
	XDrawLine(Dpy, Win, simpleMenu->simple_menu.shadowGC, xMin, yMin,
		  xMin, yMax);
	xMin++;
	yMin--;
	yMax++;
    }
}
