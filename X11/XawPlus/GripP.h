/*
 * $XTermId: GripP.h,v 1.3 2025/01/19 15:20:11 tom Exp $
 * $Xorg: GripP.h,v 1.4 2001/02/09 02:03:43 xorgcvs Exp $
 */

/*************************************************************************

Copyright 2022,2025  Thomas E. Dickey
Copyright 1987, 1988, 1998  The Open Group

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
 *  GripP.h - Private definitions for Grip widget (Used by VPane Widget)
 *
 */

#ifndef _XawGripP_h
#define _XawGripP_h

#include <X11/XawPlus/Grip.h>
#include <X11/XawPlus/SimpleP.h>

/*****************************************************************************
 *
 * Grip Widget Private Data
 *
 *****************************************************************************/

#define DEFAULT_GRIP_SIZE 12

/* New fields for the Grip widget class record */
typedef struct {int empty;} GripClassPart;

/* Full Class record declaration */
typedef struct _GripClassRec {
    CoreClassPart   	core_class;
    SimpleClassPart  	simple_class;
    GripClassPart    	grip_class;
} GripClassRec;

extern GripClassRec gripClassRec;

/* New fields for the Grip widget record */
typedef struct {
  XtCallbackList grip_action;
} GripPart;

/*****************************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************************/

typedef struct _GripRec {
   CorePart     core;
   SimplePart   simple;
   GripPart     grip;
} GripRec;

#endif /* _XawGripP_h */

