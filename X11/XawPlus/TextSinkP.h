/*
 * $XTermId: TextSinkP.h,v 1.4 2025/01/19 14:06:01 tom Exp $
 * $Xorg: TextSinkP.h,v 1.4 2001/02/09 02:03:46 xorgcvs Exp $
 */

/**************************************************************************
Copyright 2022,2025 Thomas E. Dickey
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

************************************************************************/

/*
 * TextSinkP.h - Private definitions for TextSink object
 *
 */

#ifndef _XawTextSinkP_h
#define _XawTextSinkP_h

/***********************************************************************
 *
 * TextSink Object Private Data
 *
 ***********************************************************************/

#include <X11/XawPlus/TextSink.h>
#include <X11/XawPlus/TextP.h>		/* This source works with the Text widget. */
#include <X11/XawPlus/TextSrcP.h>	/* This source works with the Text Source. */

/************************************************************
 *
 * New fields for the TextSink object class record.
 *
 ************************************************************/

typedef void (*_XawSinkDisplayTextProc)
     (Widget, int, int, XawTextPosition, XawTextPosition, Bool);

typedef void (*_XawSinkInsertCursorProc)
     (Widget, int, int, XawTextInsertState);

typedef void (*_XawSinkClearToBackgroundProc)
     (Widget, int, int, unsigned int, unsigned int);

typedef void (*_XawSinkFindPositionProc)
     (Widget, XawTextPosition, int, int, Bool, XawTextPosition*, int*, int*);

typedef void (*_XawSinkFindDistanceProc)
     (Widget, XawTextPosition, int, XawTextPosition, int*,
      XawTextPosition*, int*);

typedef void (*_XawSinkResolveProc)
     (Widget, XawTextPosition, int, int, XawTextPosition*);

typedef int  (*_XawSinkMaxLinesProc)
     (Widget, unsigned int);

typedef int  (*_XawSinkMaxHeightProc)
     (Widget, int);

typedef void (*_XawSinkSetTabsProc)
     (Widget, int, short*);

typedef void (*_XawSinkGetCursorBoundsProc)
     (Widget, XRectangle*);

typedef struct _TextSinkClassPart {
    _XawSinkDisplayTextProc DisplayText;
    _XawSinkInsertCursorProc InsertCursor;
    _XawSinkClearToBackgroundProc ClearToBackground;
    _XawSinkFindPositionProc FindPosition;
    _XawSinkFindDistanceProc FindDistance;
    _XawSinkResolveProc Resolve;
    _XawSinkMaxLinesProc MaxLines;
    _XawSinkMaxHeightProc MaxHeight;
    _XawSinkSetTabsProc	SetTabs;
    _XawSinkGetCursorBoundsProc GetCursorBounds;
} TextSinkClassPart;

/* Full class record declaration */
typedef struct _TextSinkClassRec {
    ObjectClassPart     object_class;
    TextSinkClassPart	text_sink_class;
} TextSinkClassRec;

extern TextSinkClassRec textSinkClassRec;

/* New fields for the TextSink object record */
typedef struct {
    /* resources */
    Pixel foreground;		/* Foreground color. */
    Pixel background;		/* Background color. */

    /* private state. */
    Position *tabs;		/* The tab stops as pixel values. */
    short    *char_tabs;	/* The tabs stops as character values. */
    int      tab_count;		/* number of items in tabs */

} TextSinkPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _TextSinkRec {
  ObjectPart    object;
  TextSinkPart	text_sink;
} TextSinkRec;

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

#define XtInheritDisplayText	   ((_XawSinkDisplayTextProc)_XtInherit)
#define XtInheritInsertCursor	   ((_XawSinkInsertCursorProc)_XtInherit)
#define XtInheritClearToBackground ((_XawSinkClearToBackgroundProc)_XtInherit)
#define XtInheritFindPosition	   ((_XawSinkFindPositionProc)_XtInherit)
#define XtInheritFindDistance	   ((_XawSinkFindDistanceProc)_XtInherit)
#define XtInheritResolve	   ((_XawSinkResolveProc)_XtInherit)
#define XtInheritMaxLines	   ((_XawSinkMaxLinesProc)_XtInherit)
#define XtInheritMaxHeight	   ((_XawSinkMaxHeightProc)_XtInherit)
#define XtInheritSetTabs	   ((_XawSinkSetTabsProc)_XtInherit)
#define XtInheritGetCursorBounds   ((_XawSinkGetCursorBoundsProc)_XtInherit)

#endif /* _XawTextSinkP_h */
