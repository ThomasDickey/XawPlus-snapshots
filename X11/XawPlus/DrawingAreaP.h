
/***********************************************************************
 *
 * Drawing Area Widget Private Data
 *
 * This widget adds a backing store machanism to the simple
 * widget. So it becomes usable as a drawing area for any
 * painting. Note that all Xlib painting functions are replaced
 * by a version, that works with a widget.
 *
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

#ifndef _DrawingAreaP_h
#define _DrawingAreaP_h

#include <X11/XawPlus/SimpleP.h>
#include <X11/XawPlus/DrawingArea.h>

/* New fields for the drawing area widget class record */

typedef struct {int foo;} DrawingAreaClassPart;


/* Full class record declaration */

typedef struct _DrawingAreaClassRec {
    CoreClassPart		core_class;
    SimpleClassPart		simple_class;
    DrawingAreaClassPart	drawingArea_class;
} DrawingAreaClassRec;

extern DrawingAreaClassRec drawingAreaClassRec;


/* New fields for the DrawingArea widget record */

typedef struct {

    /* private data */

    Pixmap	 BackingStore;	/* Backing store for the drawing area */
    GC		 gc;		/* Default graphics context */
    Dimension	 max_width,	/* Width and height for resizing */
		 max_height;
} DrawingAreaPart;


/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _DrawingAreaRec {
    CorePart		core;
    SimplePart		simple;
    DrawingAreaPart	draw;
} DrawingAreaRec;

#endif /* _DrawingAreaP_h */
