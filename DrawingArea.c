/*
 * $XTermId: DrawingArea.c,v 1.6 2024/04/29 00:03:55 tom Exp $
 */

/***********************************************************************
 *
 * Drawing Area Widget
 * A widget for drawing pictures using Xlib styled drawing functions.
 * This widget adds a backing store to the simple widget and defines
 * all(?) drawing functions known from the Xlib for this widget.
 *
 * Copyright 2022,2024  Thomas E. Dickey
 * Copyright (c) Roland Krause 2002, roland_krause@freenet.de
 *
 * This module is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***********************************************************************/

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/DrawingAreaP.h>
/* *INDENT-OFF* */
/* Prototypes */

static void Initialize(Widget junk, Widget new, ArgList args, Cardinal *num_args);
static void Destroy(Widget w);
static void Resize(Widget w);
static void Redisplay(Widget w, XEvent *event, Region reg);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);

DrawingAreaClassRec drawingAreaClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass)(&simpleClassRec),
    /* class_name		*/	"DrawingArea",
    /* widget_size		*/	sizeof(DrawingAreaRec),
    /* class_initialize		*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources	  	*/	NULL,
    /* num_resources	  	*/	0,
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	XtExposeCompressSeries,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	TRUE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* simple fields */
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
  { /* drawing area fields */
    /* dummy 			*/	0
  }
};
/* *INDENT-ON* */

WidgetClass drawingAreaWidgetClass = (WidgetClass) & drawingAreaClassRec;

/***********************************************************************
 *
 * Methods of this Widget
 *
 ***********************************************************************/

/***********************************************************************
 *
 * Initialize method:	Initialize widget instance. Set all values
 *			to zero or undefined and get a GC with the
 * 			background color as foreground color to
 *			initialize pixmaps.
 ***********************************************************************/

static void
Initialize(
	      Widget junk GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) new;
    XGCValues values;

    daw->draw.BackingStore = XtUnspecifiedPixmap;
    daw->draw.max_width = 0;
    daw->draw.max_height = 0;

    values.foreground = new->core.background_pixel;
    daw->draw.gc = XtGetGC(new, GCForeground, &values);
}

/***********************************************************************
 *
 * Destroy method:	Free the backing store pixmap and the gc
 *
 ***********************************************************************/

static void
Destroy(Widget w)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    XFreePixmap(XtDisplay(w), daw->draw.BackingStore);
    XtReleaseGC(w, daw->draw.gc);
}

/***********************************************************************
 *
 * SetValues method:	Handle changes of the background color
 *
 ***********************************************************************/

static Boolean
SetValues(
	     Widget current,
	     Widget request GCC_UNUSED,
	     Widget new,
	     ArgList args GCC_UNUSED,
	     Cardinal *num_args GCC_UNUSED)
{
    DrawingAreaWidget NewDaw = (DrawingAreaWidget) new;
    DrawingAreaWidget CurrentDaw = (DrawingAreaWidget) current;
    XGCValues values;

    if (CurrentDaw->core.background_pixel != NewDaw->core.background_pixel) {
	/* Changing of the background color: Release the GC for
	 * deleting the drawing area and create a new one with the
	 * new background color as foreground color
	 */
	XtReleaseGC(new, NewDaw->draw.gc);
	values.foreground = NewDaw->core.background_pixel;
	NewDaw->draw.gc = XtGetGC(new, GCForeground, &values);
    }
    /* Repainting the drawing area is not required. The new
     * background color is used after the next XawClearWindow().
     */
    return (FALSE);
}
/***********************************************************************
 *
 * Resize method:	Create a new pixmap with the given width and height,
 *			clear the pixmap and copy the old pixmap with the
 * 			minimum width and height in it. After this, free
 *			the old pixmap and store the new values for pixmap,
 *			width and height. If the old size is smaller
 *			than the new, don't change the pixmap.
 *
 ***********************************************************************/

static void
Resize(Widget w)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;
    Display *disp = XtDisplay(w);
    Pixmap new;
    Dimension width_max, height_max;

    if ((daw->draw.max_width < daw->core.width) || (daw->draw.max_height < daw->core.height)) {
	/* Determine minimum and maximum values for width and height */

	if (daw->draw.max_width < daw->core.width)
	    width_max = daw->core.width;
	else
	    width_max = daw->draw.max_width;

	if (daw->draw.max_height < daw->core.height)
	    height_max = daw->core.height;
	else
	    height_max = daw->draw.max_height;

	/* Create the new pixmap, clear it, copy the contents of the old pixmap
	 * in it and destroy the old pixmap
	 */
	new = XCreatePixmap(disp, daw->core.window, width_max, height_max, daw->core.depth);
	XFillRectangle(disp, new, daw->draw.gc, 0, 0, width_max, height_max);
	XCopyArea(disp, daw->draw.BackingStore, new, daw->draw.gc, 0, 0,
		  daw->draw.max_width, daw->draw.max_height, 0, 0);
	XFreePixmap(disp, daw->draw.BackingStore);

	/* Store the new values */

	daw->draw.BackingStore = new;
	daw->draw.max_width = width_max;
	daw->draw.max_height = height_max;
    }
}

/***********************************************************************
 *
 * Redisplay method: 	Repaint a part of the window from the backing
 * 			store if an event is given and if the widget
 *			is visible.
 *
 ***********************************************************************/

static void
Redisplay(
	     Widget w,
	     XEvent *event,
	     Region reg GCC_UNUSED)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;
    Display *disp = XtDisplay(w);

    /* Create a backing store pixmap if none exist */

    if (daw->draw.BackingStore == XtUnspecifiedPixmap) {
	daw->draw.BackingStore = XCreatePixmap(disp, daw->core.window,
					       daw->core.width,
					       daw->core.height, daw->core.depth);
	XFillRectangle(disp, daw->draw.BackingStore, daw->draw.gc,
		       0, 0, daw->core.width, daw->core.height);

	/* Save the actual height and width values */

	daw->draw.max_width = daw->core.width;
	daw->draw.max_height = daw->core.height;
    }
    /* Restore a part of the window if an event is given */

    if (event != NULL)
	XCopyArea(disp, daw->draw.BackingStore, daw->core.window,
		  daw->draw.gc, event->xexpose.x, event->xexpose.y,
		  (unsigned) event->xexpose.width, (unsigned) event->xexpose.height,
		  event->xexpose.x, event->xexpose.y);
}

/***********************************************************************
 *
 * Exported functions
 *
 * All functions are redefined Xlib drawing functions. The difference
 * is, that the first argument is the drawing area widget. The display
 * and drawable parameters are not required.
 *
 ***********************************************************************/

void
XawClearWindow(Widget w)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;
    Pixmap new;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XClearWindow(XtDisplay(w), XtWindow(w));

	/* Set the backing store pixmap to the minimum width and height */

	new = XCreatePixmap(XtDisplay(w), daw->core.window, daw->core.width,
			    daw->core.height, daw->core.depth);
	XFillRectangle(XtDisplay(w), new, daw->draw.gc, 0, 0,
		       daw->core.width, daw->core.height);
	XFreePixmap(XtDisplay(w), daw->draw.BackingStore);

	/* Store the new values */

	daw->draw.BackingStore = new;
	daw->draw.max_width = daw->core.width;
	daw->draw.max_height = daw->core.height;
    }
}

void
XawDrawPoint(
		Widget w,
		GC gc,
		int x,
		int y)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawPoint(XtDisplay(w), XtWindow(w), gc, x, y);
	XDrawPoint(XtDisplay(w), daw->draw.BackingStore, gc, x, y);
    }
}

void
XawDrawPoints(
		 Widget w,
		 GC gc,
		 XPoint * points,
		 int npoints,
		 int mode)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawPoints(XtDisplay(w), XtWindow(w), gc, points, npoints, mode);
	XDrawPoints(XtDisplay(w), daw->draw.BackingStore, gc, points,
		    npoints, mode);
    }
}

void
XawDrawLine(
	       Widget w,
	       GC gc,
	       int x1,
	       int y1,
	       int x2,
	       int y2)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawLine(XtDisplay(w), XtWindow(w), gc, x1, y1, x2, y2);
	XDrawLine(XtDisplay(w), daw->draw.BackingStore, gc, x1, y1, x2, y2);
    }
}

void
XawDrawLines(
		Widget w,
		GC gc,
		XPoint * points,
		int npoints,
		int mode)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawLines(XtDisplay(w), XtWindow(w), gc, points, npoints, mode);
	XDrawLines(XtDisplay(w), daw->draw.BackingStore, gc, points,
		   npoints, mode);
    }
}

void
XawDrawSegments(
		   Widget w,
		   GC gc,
		   XSegment * segments,
		   int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawSegments(XtDisplay(w), XtWindow(w), gc, segments, n);
	XDrawSegments(XtDisplay(w), daw->draw.BackingStore, gc, segments, n);
    }
}

void
XawDrawRectangle(
		    Widget w,
		    GC gc,
		    int x,
		    int y,
		    unsigned int width,
		    unsigned int height)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawRectangle(XtDisplay(w), XtWindow(w), gc, x, y, width, height);
	XDrawRectangle(XtDisplay(w), daw->draw.BackingStore, gc, x, y,
		       width, height);
    }
}

void
XawDrawRectangles(
		     Widget w,
		     GC gc,
		     XRectangle * rectangles,
		     int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawRectangles(XtDisplay(w), XtWindow(w), gc, rectangles, n);
	XDrawRectangles(XtDisplay(w), daw->draw.BackingStore, gc,
			rectangles, n);
    }
}

void
XawDrawArc(
	      Widget w,
	      GC gc,
	      int x,
	      int y,
	      unsigned int width,
	      unsigned int height,
	      int angle1,
	      int angle2)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawArc(XtDisplay(w), XtWindow(w), gc, x, y, width, height,
		     angle1, angle2);
	XDrawArc(XtDisplay(w), daw->draw.BackingStore, gc, x, y, width,
		 height, angle1, angle2);
    }
}

void
XawDrawArcs(
	       Widget w,
	       GC gc,
	       XArc * arcs,
	       int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawArcs(XtDisplay(w), XtWindow(w), gc, arcs, n);
	XDrawArcs(XtDisplay(w), daw->draw.BackingStore, gc, arcs, n);
    }
}

void
XawFillRectangle(
		    Widget w,
		    GC gc,
		    int x,
		    int y,
		    unsigned int width,
		    unsigned int height)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XFillRectangle(XtDisplay(w), XtWindow(w), gc, x, y, width, height);
	XFillRectangle(XtDisplay(w), daw->draw.BackingStore, gc, x, y,
		       width, height);
    }
}

void
XawFillRectangles(
		     Widget w,
		     GC gc,
		     XRectangle * rectangles,
		     int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XFillRectangles(XtDisplay(w), XtWindow(w), gc, rectangles, n);
	XFillRectangles(XtDisplay(w), daw->draw.BackingStore, gc,
			rectangles, n);
    }
}

void
XawFillArc(
	      Widget w,
	      GC gc,
	      int x,
	      int y,
	      unsigned int width,
	      unsigned int height,
	      int angle1,
	      int angle2)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XFillArc(XtDisplay(w), XtWindow(w), gc, x, y, width, height,
		     angle1, angle2);
	XFillArc(XtDisplay(w), daw->draw.BackingStore, gc, x, y, width,
		 height, angle1, angle2);
    }
}

void
XawFillArcs(
	       Widget w,
	       GC gc,
	       XArc * arcs,
	       int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XFillArcs(XtDisplay(w), XtWindow(w), gc, arcs, n);
	XFillArcs(XtDisplay(w), daw->draw.BackingStore, gc, arcs, n);
    }
}

void
XawFillPolygon(
		  Widget w,
		  GC gc,
		  XPoint * points,
		  int n,
		  int shape,
		  int mode)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XFillPolygon(XtDisplay(w), XtWindow(w), gc, points, n, shape, mode);
	XFillPolygon(XtDisplay(w), daw->draw.BackingStore, gc, points, n,
		     shape, mode);
    }
}

void
XawDrawString(
		 Widget w,
		 GC gc,
		 int x,
		 int y,
		 char *str,
		 int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawString(XtDisplay(w), XtWindow(w), gc, x, y, str, n);
	XDrawString(XtDisplay(w), daw->draw.BackingStore, gc, x, y, str, n);
    }
}

void
XawDrawImageString(
		      Widget w,
		      GC gc,
		      int x,
		      int y,
		      char *str,
		      int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawImageString(XtDisplay(w), XtWindow(w), gc, x, y, str, n);
	XDrawImageString(XtDisplay(w), daw->draw.BackingStore, gc, x, y,
			 str, n);
    }
}

void
XawDrawText(
	       Widget w,
	       GC gc,
	       int x,
	       int y,
	       XTextItem * item,
	       int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawText(XtDisplay(w), XtWindow(w), gc, x, y, item, n);
	XDrawText(XtDisplay(w), daw->draw.BackingStore, gc, x, y, item, n);
    }
}

void
XawDrawString16(
		   Widget w,
		   GC gc,
		   int x,
		   int y,
		   _Xconst XChar2b * str,
		   int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawString16(XtDisplay(w), XtWindow(w), gc, x, y, str, n);
	XDrawString16(XtDisplay(w), daw->draw.BackingStore, gc, x, y, str, n);
    }
}

void
XawDrawImageString16(
			Widget w,
			GC gc,
			int x,
			int y,
			_Xconst XChar2b * str,
			int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawImageString16(XtDisplay(w), XtWindow(w), gc, x, y, str, n);
	XDrawImageString16(XtDisplay(w), daw->draw.BackingStore, gc, x, y,
			   str, n);
    }
}

void
XawDrawText16(
		 Widget w,
		 GC gc,
		 int x,
		 int y,
		 XTextItem16 * item,
		 int n)
{
    DrawingAreaWidget daw = (DrawingAreaWidget) w;

    if (XtIsRealized(w)) {
	if (daw->core.visible)
	    XDrawText16(XtDisplay(w), XtWindow(w), gc, x, y, item, n);
	XDrawText16(XtDisplay(w), daw->draw.BackingStore, gc, x, y, item, n);
    }
}
