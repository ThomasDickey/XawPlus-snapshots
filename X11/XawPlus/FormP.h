/*
 * $XTermId: FormP.h,v 1.5 2022/12/13 01:01:00 tom Exp $
 * $Xorg: FormP.h,v 1.4 2001/02/09 02:03:43 xorgcvs Exp $
 */

/**************************************************************************
Copyright 2022  Thomas E. Dickey
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

/* Form widget private definitions */

#ifndef _XawFormP_h
#define _XawFormP_h

#include <X11/XawPlus/Form.h>

#define XtREdgeType "EdgeType"

typedef enum {LayoutPending, LayoutInProgress, LayoutDone} LayoutState;
#define XtInheritLayout ((Boolean (*)(FormWidget, unsigned, unsigned, Bool))_XtInherit)

typedef struct {
    Boolean	(*layout)(FormWidget, unsigned, unsigned, Bool);
} FormClassPart;

/*
 * Layout(
 *	FormWidget w	- the widget whose children are to be configured
 *	Dimension w, h	- bounding box of layout to be calculated
 *
 *  Stores preferred geometry in w->form.preferred_{width,height}.
 *  If w->form.resize_in_layout is True, then a geometry request
 *  may be made for the preferred bounding box if necessary.
 *
 *  Returns True if a geometry request was granted, False otherwise.
 */

typedef struct _FormClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ConstraintClassPart	constraint_class;
    FormClassPart	form_class;
} FormClassRec;

extern FormClassRec formClassRec;

typedef struct _FormPart {
    /* resources */
    int		default_spacing;    /* default distance between children */
    /* private state */
    Dimension	old_width, old_height; /* last known dimensions		 */
    int		no_refigure;	    /* no re-layout while > 0		 */
    Boolean	needs_relayout;	    /* next time no_refigure == 0	 */
    Boolean	resize_in_layout;   /* should layout() do geom request?  */
    Dimension	preferred_width, preferred_height; /* cached from layout */
    Boolean     resize_is_no_op;    /* Causes resize to take not action. */
} FormPart;

typedef struct _FormRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    FormPart		form;
} FormRec;

typedef struct _FormConstraintsPart {
/*
 * Constraint Resources.
 */
    XtEdgeType	top, bottom,	/* where to drag edge on resize		*/
		left, right;
    int		dx;		/* desired horiz offset			*/
    int		dy;		/* desired vertical offset		*/
    Widget	horiz_base;	/* measure dx from here if non-null	*/
    Widget	vert_base;	/* measure dy from here if non-null	*/
    Boolean	allow_resize;	/* TRUE if child may request resize	*/

/*
 * Private contstraint resources.
 */

/*
 * What the size of this child would be if we did not impose the
 * constraint the width and height must be greater than zero (0).
 */
    short	virtual_width, virtual_height;

/*
 * Temporary Storage for children's new possible possition.
 */

    Position new_x, new_y;

    LayoutState	layout_state;	/* temporary layout state		*/
    Boolean	deferred_resize; /* was resized while no_refigure is set */
} FormConstraintsPart;

typedef struct _FormConstraintsRec {
    FormConstraintsPart	form;
} FormConstraintsRec, *FormConstraints;

#endif /* _XawFormP_h */
