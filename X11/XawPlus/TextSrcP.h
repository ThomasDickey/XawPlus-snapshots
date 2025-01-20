/*
 * $XTermId: TextSrcP.h,v 1.5 2025/01/19 14:06:01 tom Exp $
 * $Xorg: TextSrcP.h,v 1.4 2001/02/09 02:03:47 xorgcvs Exp $
 */

/**************************************************************************
Copyright 2015-2022,2025 Thomas E. Dickey
Copyright 2002  Roland Krause
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

**************************************************************************/

/*
 * TextSrcP.h - Private definitions for TextSrc object
 *
 */

#ifndef _XawTextSrcP_h
#define _XawTextSrcP_h

/***********************************************************************
 *
 * TextSrc Object Private Data
 *
 ***********************************************************************/

#include <X11/XawPlus/TextSrc.h>
#include <X11/XawPlus/TextP.h>	/* This source works with the Text widget. */

/************************************************************
 *
 * New fields for the TextSrc object class record.
 *
 ************************************************************/

#if 0
typedef struct {
  XtPointer		next_extension;
  XrmQuark		record_type;
  long			version;
  Cardinal		record_size;
  int			(*Input)();
} TextSrcExtRec, *TextSrcExt;
#endif

typedef XawTextPosition (*_XawSrcReadProc)
     (Widget, XawTextPosition, XawTextBlock*, int);

typedef int (*_XawSrcReplaceProc)
     (Widget, XawTextPosition, XawTextPosition, XawTextBlock*);

typedef XawTextPosition (*_XawSrcScanProc)
     (Widget, XawTextPosition, XawTextScanType, XawTextScanDirection,
      int, Bool);

typedef XawTextPosition (*_XawSrcSearchProc)
     (Widget, XawTextPosition, XawTextScanDirection, XawTextBlock*);

typedef void (*_XawSrcSetSelectionProc)
     (Widget, XawTextPosition, XawTextPosition, Atom);

typedef Boolean (*_XawSrcConvertSelectionProc)
     (Widget, Atom*, Atom*, Atom*, XtPointer*, unsigned long*, int*);

typedef struct _TextSrcClassPart {
    _XawSrcReadProc Read;
    _XawSrcReplaceProc Replace;
    _XawSrcScanProc Scan;
    _XawSrcSearchProc Search;
    _XawSrcSetSelectionProc SetSelection;
    _XawSrcConvertSelectionProc ConvertSelection;
} TextSrcClassPart;

/* Full class record declaration */
typedef struct _TextSrcClassRec {
    ObjectClassPart     object_class;
    TextSrcClassPart	textSrc_class;
} TextSrcClassRec;

extern TextSrcClassRec textSrcClassRec;

/* New fields for the TextSrc object record */
typedef struct {
    /* resources */
  XawTextEditType	edit_mode;
  XrmQuark		text_format;	/* 2 formats: FMT8BIT for Ascii */
					/*            FMTWIDE for ISO 10646 */
} TextSrcPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _TextSrcRec {
  ObjectPart    object;
  TextSrcPart	textSrc;
} TextSrcRec;

/******************************************************************
 *
 * Semiprivate declarations of functions used in other modules
 *
 ******************************************************************/

char* _XawTextWCToMB(
    Display* /* d */,
    wchar_t* /* wstr */,
    int*     /* len_in_out */
);

wchar_t* _XawTextMBToWC(
    Display*  /* d */,
    char*     /* str */,
    int*      /* len_in_out */
);

/************************************************************
 *
 * Private declarations.
 *
 ************************************************************/

#if 0	/* no longer used */
typedef XawTextPosition (*_XawTextPositionFunc)();
#endif

#define XtInheritInput                ((_XawTextPositionFunc) _XtInherit)
#define XtInheritRead                 ((_XawSrcReadProc)_XtInherit)
#define XtInheritReplace              ((_XawSrcReplaceProc)_XtInherit)
#define XtInheritScan                 ((_XawSrcScanProc)_XtInherit)
#define XtInheritSearch               ((_XawSrcSearchProc)_XtInherit)
#define XtInheritSetSelection         ((_XawSrcSetSelectionProc)_XtInherit)
#define XtInheritConvertSelection     ((_XawSrcConvertSelectionProc)_XtInherit)
#if 0
#define XtTextSrcExtVersion	      1
#define XtTextSrcExtTypeString        "XT_TEXTSRC_EXT"
#endif

#endif /* _XawTextSrcP_h */
