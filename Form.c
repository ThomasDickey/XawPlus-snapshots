/*
 * $XTermId: Form.c,v 1.9 2024/04/29 00:02:54 tom Exp $
 * $Xorg: Form.c,v 1.4 2001/02/09 02:03:43 xorgcvs Exp $
 */

/**************************************************************************

Copyright 2015-2022,2024  Thomas E. Dickey
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

***************************************************************************/

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/CharSet.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/FormP.h>
/* *INDENT-OFF* */
/* Private Definitions */

static int default_value = -99999;

static XtResource resources[] = {
    {XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
      XtOffsetOf(FormRec, core.background_pixel), XtRString, "grey75"},
    {XtNdefaultDistance, XtCThickness, XtRInt, sizeof(int),
	XtOffsetOf(FormRec, form.default_spacing), XtRImmediate, (XtPointer)4}
};

static XtEdgeType defEdge = XtRubber;

#define Offset(field) XtOffsetOf(FormConstraintsRec, form.field)
static XtResource formConstraintResources[] = {
    {XtNtop, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(top), XtREdgeType, (XtPointer)&defEdge},
    {XtNbottom, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(bottom), XtREdgeType, (XtPointer)&defEdge},
    {XtNleft, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(left), XtREdgeType, (XtPointer)&defEdge},
    {XtNright, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(right), XtREdgeType, (XtPointer)&defEdge},
    {XtNhorizDistance, XtCThickness, XtRInt, sizeof(int),
	Offset(dx), XtRInt, (XtPointer) &default_value},
    {XtNfromHoriz, XtCWidget, XtRWidget, sizeof(Widget),
	Offset(horiz_base), XtRWidget, (XtPointer)NULL},
    {XtNvertDistance, XtCThickness, XtRInt, sizeof(int),
	Offset(dy), XtRInt, (XtPointer) &default_value},
    {XtNfromVert, XtCWidget, XtRWidget, sizeof(Widget),
	Offset(vert_base), XtRWidget, (XtPointer)NULL},
    {XtNresizable, XtCBoolean, XtRBoolean, sizeof(Boolean),
	Offset(allow_resize), XtRImmediate, (XtPointer) FALSE},
};
#undef Offset

static void ClassInitialize(void);
static void ClassPartInitialize(WidgetClass class);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Resize(Widget w);
static void ConstraintInitialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static Boolean ConstraintSetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static XtGeometryResult GeometryManager(Widget w, XtWidgetGeometry *request, XtWidgetGeometry *reply);
static XtGeometryResult PreferredGeometry(Widget widget, XtWidgetGeometry *request, XtWidgetGeometry *reply);
static void ChangeManaged(Widget w);
static Boolean Layout(FormWidget fw, unsigned width, unsigned height, Bool force_relayout);

static void LayoutChild(Widget w);
static void ResizeChildren(Widget w);

FormClassRec formClassRec = {
  { /* core_class fields */
    /* superclass         */    (WidgetClass) &constraintClassRec,
    /* class_name         */    "Form",
    /* widget_size        */    sizeof(FormRec),
    /* class_initialize   */    ClassInitialize,
    /* class_part_init    */    ClassPartInitialize,
    /* class_inited       */    FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */    NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    TRUE,
    /* compress_exposure  */    XtExposeCompressSeries,
    /* compress_enterleave*/    TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    Resize,
    /* expose             */    XtInheritExpose,
    /* set_values         */    SetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */    NULL,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	PreferredGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension          */	NULL
  },
  { /* composite_class fields */
    /* geometry_manager   */   GeometryManager,
    /* change_managed     */   ChangeManaged,
    /* insert_child       */   XtInheritInsertChild,
    /* delete_child       */   XtInheritDeleteChild,
    /* extension          */   NULL
  },
  { /* constraint_class fields */
    /* subresourses       */   formConstraintResources,
    /* subresource_count  */   XtNumber(formConstraintResources),
    /* constraint_size    */   sizeof(FormConstraintsRec),
    /* initialize         */   ConstraintInitialize,
    /* destroy            */   NULL,
    /* set_values         */   ConstraintSetValues,
    /* extension          */   NULL
  },
  { /* form_class fields */
    /* layout             */   Layout
  }
};
/* *INDENT-ON* */

WidgetClass formWidgetClass = (WidgetClass) & formClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static XrmQuark XtQChainLeft, XtQChainRight, XtQChainTop, XtQChainBottom, XtQRubber;

/* ARGSUSED */
static void
_CvtStringToEdgeType(
			XrmValuePtr args GCC_UNUSED,
			Cardinal *num_args GCC_UNUSED,
			XrmValuePtr fromVal,
			XrmValuePtr toVal)
{
    static XtEdgeType edgeType;
    XrmQuark q;
    char lowerName[40];

    if (strlen((char *) fromVal->addr) < sizeof lowerName) {
	XmuCopyISOLatin1Lowered(lowerName, (char *) fromVal->addr);
	q = XrmStringToQuark(lowerName);
	if (q == XtQChainLeft)
	    edgeType = XtChainLeft;
	else if (q == XtQChainRight)
	    edgeType = XtChainRight;
	else if (q == XtQChainTop)
	    edgeType = XtChainTop;
	else if (q == XtQChainBottom)
	    edgeType = XtChainBottom;
	else if (q == XtQRubber)
	    edgeType = XtRubber;
	else {
	    toVal->size = 0;
	    toVal->addr = NULL;
	    return;
	}
	toVal->size = sizeof edgeType;
	toVal->addr = (XPointer) & edgeType;
	return;
    }
    toVal->addr = NULL;
    toVal->size = 0;
}

static void
ClassInitialize(void)
{
    static XtConvertArgRec parentCvtArgs[] =
    {
	{XtBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.parent),
	 sizeof(Widget)}
    };
    static XtConvertArgRec colConvertArg[] =
    {
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.screen),
	 sizeof(Screen *)},
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec,
	 core.colormap), sizeof(Colormap)}
    };

    XawInitializeWidgetSet();
    XtQChainLeft = XrmPermStringToQuark("chainleft");
    XtQChainRight = XrmPermStringToQuark("chainright");
    XtQChainTop = XrmPermStringToQuark("chaintop");
    XtQChainBottom = XrmPermStringToQuark("chainbottom");
    XtQRubber = XrmPermStringToQuark("rubber");

    XtAddConverter(XtRString, XtREdgeType, _CvtStringToEdgeType, NULL, 0);
    XtSetTypeConverter(XtRString, XtRWidget, XmuNewCvtStringToWidget,
		       parentCvtArgs, XtNumber(parentCvtArgs), XtCacheNone, NULL);
    XtSetTypeConverter(XtRString, XtRPixel, XtCvtStringToPixel,
		       colConvertArg, XtNumber(colConvertArg),
		       XtCacheByDisplay, NULL);
}

static void
ClassPartInitialize(WidgetClass class)
{
    FormWidgetClass c = (FormWidgetClass) class;
    FormWidgetClass super = (FormWidgetClass)
    c->core_class.superclass;

    if (c->form_class.layout == XtInheritLayout)
	c->form_class.layout = super->form_class.layout;
}

/* ARGSUSED */
static void
Initialize(
	      Widget request GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    FormWidget fw = (FormWidget) new;

    fw->form.old_width = fw->core.width;
    fw->form.old_height = fw->core.height;
    fw->form.no_refigure = False;
    fw->form.needs_relayout = False;
    fw->form.resize_in_layout = True;
    fw->form.resize_is_no_op = False;
}

/*	Function Name: ChangeFormGeometry
 *	Description: Ask the parent to change the form widget's geometry.
 *	Arguments: w - the Form widget.
 *                 query_only - TRUE if this is only a query.
 *                 width, height - the new width and height.
 *                 ret_width, ret_height - the actual size the form is allowed
 *                                         to resize to.
 *	Returns: TRUE of children may always be resized.
 */

static Boolean
ChangeFormGeometry(
		      Widget w,
		      Boolean query_only,
		      Dimension width,
		      Dimension height,
		      Dimension *ret_width,
		      Dimension *ret_height)
{
    FormWidget fw = (FormWidget) w;
    Boolean always_resize_children;
    XtGeometryResult result;
    XtWidgetGeometry request, return_request;

    /*
     * If we are already at the desired size then there is no need
     * to ask our parent of we can change size.
     */

    if ((width == fw->core.width) && (height == fw->core.height))
	return (TRUE);

    request.width = width;
    request.height = height;
    request.request_mode = CWWidth | CWHeight;
    if (query_only)
	request.request_mode |= XtCWQueryOnly;

    /*
     * Do no invoke the resize rules if our size changes here.
     */

    fw->form.resize_is_no_op = TRUE;

    result = XtMakeGeometryRequest(w, &request, &return_request);
    if (result == XtGeometryAlmost) {
	request = return_request;
	(void) XtMakeGeometryRequest(w, &request, &return_request);
	always_resize_children = FALSE;
    } else
	always_resize_children = (result == XtGeometryYes);

    fw->form.resize_is_no_op = FALSE;

    if (ret_width != NULL)
	*ret_width = request.width;
    if (ret_height != NULL)
	*ret_height = request.height;

    return (always_resize_children);
}

/*	Function Name: Layout
 *	Description: Moves all the children around.
 *	Arguments: fw - the Form widget.
 *                 width, height - ** UNUSED **.
 *                 force_relayout - will force the children to be
 *                                 moved, even if some go past the edge
 *                                 of the form.
 *	Returns: True if the children are allowed to move from their
 *               current locations to the new ones.
 */

/* ARGSUSED */
static Boolean
Layout(
	  FormWidget fw,
	  unsigned width GCC_UNUSED,
	  unsigned height GCC_UNUSED,
	  Bool force_relayout)
{
    int num_children = (int) fw->composite.num_children;
    WidgetList children = fw->composite.children;
    Widget *childP;
    Dimension maxx, maxy;
    Boolean ret_val;

    for (childP = children; childP - children < num_children; childP++) {
	FormConstraints form = (FormConstraints) (*childP)->core.constraints;
	form->form.layout_state = LayoutPending;
    }

    maxx = maxy = 1;
    for (childP = children; childP - children < num_children; childP++) {
	if (XtIsManaged(*childP)) {
	    FormConstraints form;
	    Position x, y;

	    form = (FormConstraints) (*childP)->core.constraints;

	    LayoutChild(*childP);

	    x = (Position) (form->form.new_x + (*childP)->core.width +
			    ((*childP)->core.border_width << 1));
	    if (x > (int) maxx)
		maxx = (Dimension) x;

	    y = (Position) (form->form.new_y + (*childP)->core.height +
			    ((*childP)->core.border_width << 1));
	    if (y > (int) maxy)
		maxy = (Dimension) y;
	}
    }

    fw->form.preferred_width = (maxx += (Dimension) fw->form.default_spacing);
    fw->form.preferred_height = (maxy += (Dimension) fw->form.default_spacing);

    if (fw->form.resize_in_layout) {
	Boolean always_resize_children;

	always_resize_children =
	    ChangeFormGeometry((Widget) fw, FALSE, maxx, maxy,
			       (Dimension *) NULL, (Dimension *) NULL);

	fw->form.old_width = fw->core.width;
	fw->form.old_height = fw->core.height;

	ret_val = (always_resize_children || ((fw->core.width >= maxx) &&
					      (fw->core.height >= maxy)));

	if (force_relayout)
	    ret_val = TRUE;

	if (ret_val)
	    ResizeChildren((Widget) fw);
    } else
	ret_val = False;

    fw->form.needs_relayout = False;
    return ret_val;
}

/*	Function Name: ResizeChildren
 *	Description: Resizes all children to new_x and new_y.
 *	Arguments: w - the form widget.
 *	Returns: none.
 */

static void
ResizeChildren(Widget w)
{
    FormWidget fw = (FormWidget) w;
    int num_children = (int) fw->composite.num_children;
    WidgetList children = fw->composite.children;
    Widget *childP;

    for (childP = children; childP - children < num_children; childP++) {
	FormConstraints form;

	if (!XtIsManaged(*childP))
	    continue;

	form = (FormConstraints) (*childP)->core.constraints;
	if (fw->form.no_refigure) {
/*
 * I am changing the widget wrapper w/o modifing the window.  This is
 * risky, but I can get away with it since I am the parent of this
 * widget, and he must ask me for any geometry changes.
 *
 * The window will be updated when no_refigure is set back to False.
 */
	    (*childP)->core.x = form->form.new_x;
	    (*childP)->core.y = form->form.new_y;
	} else
	    XtMoveWidget(*childP, form->form.new_x, form->form.new_y);
    }
}

static void
LayoutChild(Widget w)
{
    FormConstraints form = (FormConstraints) w->core.constraints;
    Widget ref;

    switch (form->form.layout_state) {

    case LayoutPending:
	form->form.layout_state = LayoutInProgress;
	break;

    case LayoutDone:
	return;

    case LayoutInProgress:
	{
	    String subs[2];
	    Cardinal num_subs = 2;
	    subs[0] = w->core.name;
	    subs[1] = w->core.parent->core.name;
	    XtAppWarningMsg(XtWidgetToApplicationContext(w),
			    "constraintLoop", "xawFormLayout",
			    "XawToolkitError",
			    "constraint loop detected while laying out child '%s' in FormWidget '%s'",
			    subs, &num_subs);
	    return;
	}
    }

    form->form.new_x = (Position) form->form.dx;
    form->form.new_y = (Position) form->form.dy;
    if ((ref = form->form.horiz_base) != (Widget) NULL) {
	FormConstraints ref_form = (FormConstraints) ref->core.constraints;

	LayoutChild(ref);
	form->form.new_x += (Position) (ref_form->form.new_x +
					ref->core.width +
					(ref->core.border_width << 1));
    }
    if ((ref = form->form.vert_base) != (Widget) NULL) {
	FormConstraints ref_form = (FormConstraints) ref->core.constraints;

	LayoutChild(ref);
	form->form.new_y += (Position) (ref_form->form.new_y +
					ref->core.height +
					(ref->core.border_width << 1));
    }

    form->form.layout_state = LayoutDone;
}

static Position
TransformCoord(
		  Position loc,
		  Dimension old,
		  Dimension new,
		  XtEdgeType type)
{
    if (type == XtRubber) {
	if (((int) old) > 0)
	    loc = (Position) ((int) (loc * new) / (int) old);
    } else if (type == XtChainBottom || type == XtChainRight)
	loc += (Position) ((Position) new - (Position) old);

    /* I don't see any problem with returning values less than zero. */

    return (loc);
}

static void
Resize(Widget w)
{
    FormWidget fw = (FormWidget) w;
    WidgetList children = fw->composite.children;
    int num_children = (int) fw->composite.num_children;
    Widget *childP;
    Position x, y;
    Dimension width, height;

    if (!fw->form.resize_is_no_op)
	for (childP = children; childP - children < num_children; childP++) {
	    FormConstraints form = (FormConstraints) (*childP)->core.constraints;
	    if (!XtIsManaged(*childP))
		continue;
	    x = TransformCoord((*childP)->core.x, fw->form.old_width,
			       fw->core.width, form->form.left);
	    y = TransformCoord((*childP)->core.y, fw->form.old_height,
			       fw->core.height, form->form.top);

	    form->form.virtual_width =
		TransformCoord((Position) ((*childP)->core.x
					   + form->form.virtual_width
					   + 2 * (*childP)->core.border_width),
			       fw->form.old_width, fw->core.width,
			       form->form.right)
		- (x + 2 * (*childP)->core.border_width);

	    form->form.virtual_height =
		TransformCoord((Position) ((*childP)->core.y
					   + form->form.virtual_height
					   + 2 * (*childP)->core.border_width),
			       fw->form.old_height, fw->core.height,
			       form->form.bottom)
		- (y + 2 * (*childP)->core.border_width);

	    width = ((Dimension)
		     (form->form.virtual_width < 1) ? 1 : form->form.virtual_width);
	    height = ((Dimension)
		      (form->form.virtual_height < 1) ? 1 : form->form.virtual_height);

	    XtConfigureWidget(*childP, x, y, (Dimension) width, (Dimension) height,
			      (*childP)->core.border_width);
	}

    fw->form.old_width = fw->core.width;
    fw->form.old_height = fw->core.height;
}

/*
 * I don't want to even think about what ``Almost'' would mean - Chris.
 */

/* ARGSUSED */
static XtGeometryResult
GeometryManager(
		   Widget w,
		   XtWidgetGeometry * request,
		   XtWidgetGeometry * reply GCC_UNUSED)		/* RETURN */
{
    Dimension old_width, old_height;
    FormWidget fw = (FormWidget) XtParent(w);
    FormConstraints form = (FormConstraints) w->core.constraints;
    XtWidgetGeometry allowed;
    XtGeometryResult ret_val;

    if ((request->request_mode & (XtGeometryMask) ~ (XtCWQueryOnly | CWWidth
						     | CWHeight)) ||
	!form->form.allow_resize) {

	/* If GeometryManager is invoked during a SetValues call on a child
	 * then it is necessary to compute a new layout if ConstraintSetValues
	 * allowed any constraint changes. */

	if (fw->form.needs_relayout)
	    (*((FormWidgetClass) fw->core.widget_class)->form_class.layout)
		(fw, 0, 0, True);
	return (XtGeometryNo);
    }

    if (request->request_mode & CWWidth)
	allowed.width = request->width;
    else
	allowed.width = w->core.width;

    if (request->request_mode & CWHeight)
	allowed.height = request->height;
    else
	allowed.height = w->core.height;

    if (allowed.width == w->core.width && allowed.height == w->core.height) {

	/* If GeometryManager is invoked during a SetValues call on a child
	 * then it is necessary to compute a new layout if ConstraintSetValues
	 * allowed any constraint changes. */

	if (fw->form.needs_relayout)
	    (*((FormWidgetClass) fw->core.widget_class)->form_class.layout)
		(fw, 0, 0, True);
	return (XtGeometryNo);
    }

    /*
     * Remember the old size, and then set the size to the requested size.
     */

    old_width = w->core.width;
    old_height = w->core.height;
    w->core.width = allowed.width;
    w->core.height = allowed.height;

    if (request->request_mode & XtCWQueryOnly) {
	Boolean always_resize_children;
	Dimension ret_width, ret_height;

	fw->form.resize_in_layout = FALSE;

	(*((FormWidgetClass) fw->core.widget_class)->form_class.layout)
	    (fw, w->core.width, w->core.height,
	     FALSE);

	/*
	 * Reset the size of this child back to what it used to be.
	 */

	w->core.width = old_width;
	w->core.height = old_height;

	fw->form.resize_in_layout = TRUE;

	always_resize_children = ChangeFormGeometry(w, TRUE,
						    fw->form.preferred_width,
						    fw->form.preferred_height,
						    &ret_width, &ret_height);

	if (always_resize_children ||
	    ((ret_width >= fw->form.preferred_width) &&
	     (ret_height >= fw->form.preferred_height))) {
	    ret_val = XtGeometryYes;
	} else
	    ret_val = XtGeometryNo;
    } else {
	if ((*((FormWidgetClass) fw->core.widget_class)->form_class.layout)
	    (fw, w->core.width, w->core.height,
	     FALSE)) {
	    form->form.virtual_width = (short) w->core.width;	/* reset virtual */
	    form->form.virtual_height = (short) w->core.height;		/* width and height. */
	    if (fw->form.no_refigure) {
/*
 * I am changing the widget wrapper w/o modifing the window.  This is
 * risky, but I can get away with it since I am the parent of this
 * widget, and he must ask me for any geometry changes.
 *
 * The window will be updated when no_refigure is set back to False.
 */
		form->form.deferred_resize = True;
		ret_val = XtGeometryDone;
	    } else
		ret_val = XtGeometryYes;
	} else {
	    w->core.width = old_width;
	    w->core.height = old_height;
	    ret_val = XtGeometryNo;
	}
    }

    return (ret_val);
}

/* ARGSUSED */
static Boolean
SetValues(
	     Widget current GCC_UNUSED,
	     Widget request GCC_UNUSED,
	     Widget new GCC_UNUSED,
	     ArgList args GCC_UNUSED,
	     Cardinal *num_args GCC_UNUSED)
{
    return (FALSE);
}

/* ARGSUSED */
static void
ConstraintInitialize(
			Widget request GCC_UNUSED,
			Widget new,
			ArgList args GCC_UNUSED,
			Cardinal *num_args GCC_UNUSED)
{
    FormConstraints form = (FormConstraints) new->core.constraints;
    FormWidget fw = (FormWidget) new->core.parent;

    form->form.virtual_width = (short) (int) new->core.width;
    form->form.virtual_height = (short) (int) new->core.height;

    if (form->form.dx == default_value)
	form->form.dx = fw->form.default_spacing;

    if (form->form.dy == default_value)
	form->form.dy = fw->form.default_spacing;

    form->form.deferred_resize = False;
}

/*ARGSUSED*/
static Boolean
ConstraintSetValues(
		       Widget current,
		       Widget request GCC_UNUSED,
		       Widget new,
		       ArgList args GCC_UNUSED,
		       Cardinal *num_args GCC_UNUSED)
{
    FormConstraints cfc = (FormConstraints) current->core.constraints;
    FormConstraints nfc = (FormConstraints) new->core.constraints;

    if (cfc->form.top != nfc->form.top ||
	cfc->form.bottom != nfc->form.bottom ||
	cfc->form.left != nfc->form.left ||
	cfc->form.right != nfc->form.right ||
	cfc->form.dx != nfc->form.dx ||
	cfc->form.dy != nfc->form.dy ||
	cfc->form.horiz_base != nfc->form.horiz_base ||
	cfc->form.vert_base != nfc->form.vert_base) {

	FormWidget fp = (FormWidget) XtParent(new);

	/* If there are no subclass ConstraintSetValues procedures remaining
	 * to be invoked, and if there is no geometry request about to be
	 * made, then invoke the new layout now; else defer it. */

	if (XtClass(XtParent(new)) == formWidgetClass &&
	    current->core.x == new->core.x &&
	    current->core.y == new->core.y &&
	    current->core.width == new->core.width &&
	    current->core.height == new->core.height &&
	    current->core.border_width == new->core.border_width)
	    Layout(fp, 0, 0, True);
	else
	    fp->form.needs_relayout = True;
    }
    return (FALSE);
}

static void
ChangeManaged(Widget w)
{
    FormWidget fw = (FormWidget) w;
    FormConstraints form;
    WidgetList children, childP;
    int num_children = (int) fw->composite.num_children;
    Widget child;

    /*
     * Reset virtual width and height for all children.
     */

    for (children = childP = fw->composite.children;
	 childP - children < num_children; childP++) {
	child = *childP;
	if (XtIsManaged(child)) {
	    form = (FormConstraints) child->core.constraints;

/*
 * If the size is one (1) then we must not change the virtual sizes, as
 * they contain useful information.  If someone actually wants a widget of
 * width or height one (1) in a form widget he will lose, can't win them all.
 *
 * Chris D. Peterson 2/9/89.
 */

	    if (child->core.width != 1)
		form->form.virtual_width = (short) child->core.width;
	    if (child->core.height != 1)
		form->form.virtual_height = (short) child->core.height;
	}
    }
    (*((FormWidgetClass) w->core.widget_class)->form_class.layout)
	((FormWidget) w, w->core.width,
	 w->core.height, TRUE);
}

static XtGeometryResult
PreferredGeometry(
		     Widget widget,
		     XtWidgetGeometry * request,
		     XtWidgetGeometry * reply)
{
    FormWidget w = (FormWidget) widget;

    reply->width = w->form.preferred_width;
    reply->height = w->form.preferred_height;
    reply->request_mode = CWWidth | CWHeight;
    if ((request->request_mode & (CWWidth | CWHeight)) == (CWWidth |
	CWHeight) &&
	(request->width == reply->width) && (request->height == reply->height))
	return XtGeometryYes;
    else if ((reply->width == w->core.width) && (reply->height == w->core.height))
	return XtGeometryNo;
    else
	return XtGeometryAlmost;
}

/**********************************************************************
 *
 * Public routines
 *
 **********************************************************************/

/*
 * Set or reset figuring (ignored if not realized)
 */

void
XawFormDoLayout(Widget w,
		Boolean doit)
{
    Widget *childP;
    FormWidget fw = (FormWidget) w;
    int num_children = (int) fw->composite.num_children;
    WidgetList children = fw->composite.children;

    if (((fw->form.no_refigure = !doit) == TRUE) || !XtIsRealized(w))
	return;

    for (childP = children; childP - children < num_children; childP++) {
	Widget w2 = *childP;
	if (XtIsManaged(w2)) {
	    FormConstraints form = (FormConstraints) w2->core.constraints;

	    /*
	     * Xt Configure widget is too smart, and optimizes out
	     * my changes.
	     */

	    XMoveResizeWindow(XtDisplay(w2), XtWindow(w2),
			      w2->core.x, w2->core.y,
			      w2->core.width, w2->core.height);

	    if (form->form.deferred_resize &&
		XtClass(w2)->core_class.resize != (XtWidgetProc) NULL) {
		(*(XtClass(w2)->core_class.resize)) (w2);
		form->form.deferred_resize = False;
	    }
	}
    }
}
