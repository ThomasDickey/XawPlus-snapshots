/*
 * $XTermId: DrawingArea.h,v 1.3 2025/01/19 14:12:49 tom Exp $
 *
 * Copyright 2015,2025 Thomas E. Dickey
 * Copyright 2002 Roland Krause
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
 */

#ifndef _DrawingArea_h
#define _DrawingArea_h

/*
 *
 * Drawing Area Widget
 *
 * This widget adds a backing store machanism to the simple
 * widget. So it becomes usable as a drawing area for any
 * painting. Note that all Xlib painting functions are replaced
 * by a version, that works with a widget.
 */
#include <X11/XawPlus/Simple.h>
#include <X11/Xfuncproto.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer		NULL

 RECTANGLE:
 borderWidth	     BorderWidth	Dimension	1
 height		     Height		Dimension	0
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	0
 x		     Position		Position	0
 y		     Position		Position	0

 CORE:
 border		     BorderColor	Pixel		XtDefaultForeground
 background	     Background		Pixel		grey75
 mappedWhenManaged   MappedWhenManaged	Boolean		True

 SIMPLE:
 cursor		     Cursor		Cursor		None
 cursorName	     Cursor		String		NULL
 pointerColor        Foreground         Pixel           XtDefaultForeground
 pointerColorBackground Background      Pixel           XtDefaultBackground
 insensitiveBorder   Insensitive	Pixmap		Gray
 international	     International      Boolean		False
 highlightColor	     Background		Pixel		grey90 *)
 shadowColor	     Background		Pixel		grey40 *)
 buttonBorderWidth   Width		Dimension	2      *)

 DRAWING AREA: 	     No additional resources

 *) Resource has no meaning for this widget

*/

typedef struct _DrawingAreaClassRec	*DrawingAreaWidgetClass;
typedef struct _DrawingAreaRec		*DrawingAreaWidget;

extern WidgetClass drawingAreaWidgetClass;

/******************************************************************
 *
 * Exported drawing functions
 *
 *****************************************************************/

_XFUNCPROTOBEGIN

/*  Function Name : XawClearWindow
 *  Description   : clear the drawing area
 *  Arguments	  : w - drawing area widget
 *  Returns	  : none
 */
extern void XawClearWindow(
    Widget		/* w */
);

/*  Function Name : XawDrawPoint
 *  Description   : draw a point using the given gc in the drawing area
 *  Arguments	  : w    - drawing area widget
 *		    gc   - graphics context
 *		    x, y - coordinates of the point
 *  Returns	  : none
 */
extern void XawDrawPoint(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int		/* coordinates */
);

/*  Function Name : XawDrawPoints
 *  Description   : draw points using the given gc in the drawing area
 *  Arguments	  : w        - drawing area widget
 *		    gc       - graphics context
 *		    points[] - list of point coordinates
 * 		    npoints  - number of points
 *		    mode     - drawing mode: Relative to origin or previous point
 *  Returns	  : none
 */
extern void XawDrawPoints(
    Widget,		/* w */
    GC,			/* graphics context */
    XPoint *,		/* list of point coordinates */
    int,		/* number of points */
    int			/* drawing mode */
);

/*  Function Name : XawDrawLine
 *  Description   : draw a line using the given gc in the drawing area
 *  Arguments	  : w	   - drawing area widget
 * 		    gc 	   - graphics context
 * 		    x1, y1 - starting point
 *		    x1, y2 - end point
 *  Returns	  : none
 */
extern void XawDrawLine(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* coordinates starting point */
    int, int		/* coordinates end point */
);

/*  Function Name : XawDrawLines
 *  Description   : draw connected lines using the given gc in the drawing area
 *  Arguments	  : w        - drawing area widget
 *		    gc       - graphics context
 *		    points[] - list of point coordinates
 * 		    npoints  - number of points
 *		    mode     - drawing mode: Relative to origin or previous point
 *  Returns	  : none
 */
extern void XawDrawLines(
    Widget,		/* w */
    GC,			/* graphics context */
    XPoint *,		/* list of point coordinates */
    int,		/* number of points */
    int			/* drawing mode */
);

/*  Function Name : XawDrawSegments
 *  Description   : draw unconnected lines in the drawing area
 *  Arguments	  : w		- drawing area widget
 *		    gc		- graphics context
 *		    segments[]  - list of line coordinates
 *		    n		- number of segments
 *  Returns	  : none
 */
extern void XawDrawSegments(
    Widget,		/* w */
    GC,			/* graphics context */
    XSegment *,		/* list of line coordinates */
    int			/* number of segments */
);

/*  Function Name : XawDrawRectangle
 *  Description   : draw an rectangle
 *  Arguments	  : w		  - drawing area widget
 *		    gc		  - graphics context
 *		    x,y		  - upper left edge of the rectangle
 *		    width, height - widht and height of the rectangle
 *  Returns	  : none
 */
extern void XawDrawRectangle(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* upper left edge */
    unsigned int,	/* widht and height */
    unsigned int
);

/*  Function Name : XawDrawRectangles
 *  Description   : draw rectangles
 *  Arguments	  : w		- drawing area widget
 *		    gc		- graphics context
 *		    rectangles  - list of rectangle coordinates
 *		    n		- number of rectangles
 *  Returns	  : none
 */
extern void XawDrawRectangles(
    Widget,		/* w */
    GC,			/* graphics context */
    XRectangle *,	/* rectangle coordinates */
    int			/* number of rectangles */
);

/*  Function Name : XawDrawArc
 *  Description   : draw a circle or an elipsoid or a part of it
 *  Arguments	  : w		   - drawing area widget
 *		    gc		   - graphics context
 *		    x,y		   - center position of the circle/elipsoid
 *		    width, height  - widht and height of the circle/elipsoid
 *		    angle1, angle2 - draw from angle 1 to angle 2
 *  Returns	  : none
 */
extern void XawDrawArc(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* center */
    unsigned int,	/* width */
    unsigned int,	/* height */
    int, int		/* start angle, end angle */
);

/*  Function Name : XawDrawArcs
 *  Description   : draw n circles or elipsoids
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    arcs - parameters of the circles/elipsoids
 *		    n	 - number of circles
 *  Returns	  : none
 */
extern void XawDrawArcs(
    Widget,		/* w */
    GC,			/* graphics context */
    XArc *,		/* list of parameters */
    int			/* number of circles */
);

/*  Function Name : XawFillRectangle
 *  Description   : draw a filled rectangle
 *  Arguments	  : w		  - drawing area widget
 *		    gc		  - graphics context
 *		    x,y		  - upper left edge of the rectangle
 *		    width, height - widht and height of the rectangle
 *  Returns	  : none
 */
extern void XawFillRectangle(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* upper left edge */
    unsigned int,	/* widht and height */
    unsigned int
);

/*  Function Name : XawFillRectangles
 *  Description   : draw n filled rectangles
 *  Arguments	  : w		- drawing area widget
 *		    gc		- graphics context
 *		    rectangles  - list of rectangle coordinates
 *		    n		- number of rectangles
 *  Returns	  : none
 */
extern void XawFillRectangles(
    Widget,		/* w */
    GC,			/* graphics context */
    XRectangle *,	/* rectangle coordinates */
    int			/* number of rectangles */
);

/*  Function Name : XawFillArc
 *  Description   : draw a filled circle or elipsoid
 *  Arguments	  : w		   - drawing area widget
 *		    gc		   - graphics context
 *		    x,y		   - center position of the circle/elipsoid
 *		    width, height  - widht and height of the circle/elipsoid
 *		    angle1, angle2 - draw from angle 1 to angle 2
 *  Returns	  : none
 */
extern void XawFillArc(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* center */
    unsigned int,	/* width */
    unsigned int,	/* height */
    int, int		/* start angle, end angle */
);

/*  Function Name : XawFillArcs
 *  Description   : draw n filled circles or elipsoids
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    arcs - parameters of the circles/elipsoids
 *		    n	 - number of circles
 *  Returns	  : none
 */
extern void XawFillArcs(
    Widget,		/* w */
    GC,			/* graphics context */
    XArc *,		/* list of parameters */
    int			/* number of circles */
);

/*  Function Name : XawFillPolygon
 *  Description   : draw a filled polygon
 *  Arguments	  : w	   - drawing area widget
 *		    gc	   - graphics context
 *		    points - point coordinates of the polygon
 *		    n	   - number of points
 *		    shape  - drawing mode complex, convex or nonconvex
 *		    mode   - relative or absolute coordinates
 *  Returns	  : none
 */
extern void XawFillPolygon(
    Widget,		/* w */
    GC,			/* graphics context */
    XPoint *,		/* list of points */
    int,		/* number of points */
    int,		/* drawing mode */
    int			/* relative or absolute coordinates */
);

/*****************************************************************
 *
 * Exported text drawing functions
 *
 *****************************************************************/

/*  Function Name : XawDrawString
 *  Description   : draw a string without filling the background
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    x,y  - position of the string in the widget
 *		    str  - the text string
 *		    n	 - number of characters
 *  Returns	  : none
 */
extern void XawDrawString(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* string position */
    char *,		/* the string */
    int			/* number of chararcters */
);

/*  Function Name : XawDrawImageString
 *  Description   : draw a string with filling the background
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    x,y  - position of the string in the widget
 *		    str  - the text string
 *		    n	 - number of characters
 *  Returns	  : none
 */
extern void XawDrawImageString(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* string position */
    char *,		/* the string */
    int			/* number of chararcters */
);

/*  Function Name : XawDrawText
 *  Description   : draw strings using the parameters in XTextItem
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    x,y  - position of the string in the widget
 *		    items- An array of *XTextItem* entries
 *		    n	 - number of items
 *  Returns	  : none
 */
extern void XawDrawText(
    Widget,		/* w */
    GC,			/* gc */
    int,		/* x */
    int,		/* y */
    XTextItem *,	/* items */
    int			/* nitems */
);

/*  Function Name : XawDrawString16
 *  Description   : draw a 2 byte string without filling the background
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    x,y  - position of the string in the widget
 *		    str  - the text string
 *		    n	 - number of characters
 *  Returns	  : none
 */
extern void XawDrawString16(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* string position */
    _Xconst XChar2b*,	/* the string */
    int			/* number of chararcters */
);

/*  Function Name : XawDrawImageString16
 *  Description   : draw a 2 byte string with filling the background
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    x,y  - position of the string in the widget
 *		    str  - the text string
 *		    n	 - number of characters
 *  Returns	  : none
 */
extern void XawDrawImageString16(
    Widget,		/* w */
    GC,			/* graphics context */
    int, int,		/* string position */
    _Xconst XChar2b*,	/* string */
    int			/* number of chararcters */
);

/*  Function Name : XawDrawText16
 *  Description   : draw strings using the parameters in XTextItem16
 *  Arguments	  : w	 - drawing area widget
 *		    gc	 - graphics context
 *		    x,y  - position of the string in the widget
 *		    items- An array of *XTextItem16* entries
 *		    n	 - number of items
 *  Returns	  : none
 */
extern void XawDrawText16(
    Widget,		/* w */
    GC,			/* gc */
    int,		/* x */
    int,		/* y */
    XTextItem16 *,	/* items */
    int			/* nitems */
);

_XFUNCPROTOEND

#endif /* _DrawingArea_h */
