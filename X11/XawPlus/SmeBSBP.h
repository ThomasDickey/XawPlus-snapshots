/*
 * $XTermId: SmeBSBP.h,v 1.3 2025/01/19 15:20:11 tom Exp $
 * $Xorg: SmeBSBP.h,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 *
Copyright 2022,2025  Thomas E. Dickey
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
 *
 * Author:  Chris D. Peterson, MIT X Consortium
 *
 * This file contains modifications for XawPlus, Roland Krause 2002
 */

/*
 * SmeP.h - Private definitions for Sme object
 *
 */

#ifndef _XawSmeBSBP_h
#define _XawSmeBSBP_h

/***********************************************************************
 *
 * Sme Object Private Data
 *
 ***********************************************************************/

#include <X11/XawPlus/SmeP.h>
#include <X11/XawPlus/SmeBSB.h>

/************************************************************
 *
 * New fields for the Sme Object class record.
 *
 ************************************************************/

typedef struct _SmeBSBClassPart {
  XtPointer extension;
} SmeBSBClassPart;

/* Full class record declaration */
typedef struct _SmeBSBClassRec {
    RectObjClassPart	rect_class;
    SmeClassPart	sme_class;
    SmeBSBClassPart	sme_bsb_class;
} SmeBSBClassRec;

extern SmeBSBClassRec smeBSBClassRec;

/* New fields for the Sme Object record */
typedef struct {
/* resources */
  String    label;		/* The entry label. */
  int       vert_space;		/* extra vert space to leave, as a percentage
				   of the font height of the label. */
  Pixmap    left_bitmap,
	    right_bitmap,	/* bitmaps to show */
	    left_clipmask,	/* and their clip masks */
	    right_clipmask;
  Dimension left_margin,
	    right_margin;	/* left and right margins. */
  Pixel     foreground;		/* foreground color. */
  XFontStruct *font;		/* The font to show the label */
  XFontSet  fontset;		/* .. or the font set */
  unsigned char encoding;	/* Type of encoding, 8 bit or 16 bit in UTF8 */
  XtJustify justify;		/* Justification for the label. */

/* private resources. */

  Boolean   set_values_area_cleared; /* Remember if we need to unhighlight. */
  GC	    norm_gc;		/* noral color gc. */
  GC 	    rev_gc;		/* reverse color gc. */
  GC 	    gray_gc, lgray_gc;	/* Normal color (grayed out) gc's. */
  GC	    invert_gc;		/* gc for flipping colors. */

  Dimension left_bitmap_width;	/* size and depth of each bitmap. */
  Dimension left_bitmap_height;
  unsigned int left_bitmap_depth;
  Dimension right_bitmap_width;
  Dimension right_bitmap_height;
  unsigned int right_bitmap_depth;
} SmeBSBPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _SmeBSBRec {
    ObjectPart		object;
    RectObjPart		rectangle;
    SmePart		sme;
    SmeBSBPart		sme_bsb;
} SmeBSBRec;

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

#endif /* _XawSmeBSBP_h */
