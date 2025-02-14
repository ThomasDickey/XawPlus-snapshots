/*
 * $XTermId: Dialog.h,v 1.3 2025/01/19 14:11:32 tom Exp $
 * $Xorg: Dialog.h,v 1.4 2001/02/09 02:03:43 xorgcvs Exp $
 */

/************************************************************************

Copyright 2015,2025 Thomas E. Dickey
Copyright 2002 Roland Krause
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

************************************************************************/

#ifndef _XawDialog_h
#define _XawDialog_h

#include <X11/XawPlus/Form.h>

/***********************************************************************
 *
 * Dialog Widget
 *
 ***********************************************************************/

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer	  	NULL

 RECTANGLE:
 borderWidth	     BorderWidth	Dimension	1
 sensitive	     Sensitive		Boolean		True
 height		     Height		Dimension	computed at create
 width		     Width		Dimension	computed at create
 x		     Position		Position	0
 y		     Position		Position	0

 CORE:
 border		     BorderColor	Pixel		XtDefaultForeground
 background	     Background		Pixel		grey75
 mappedWhenManaged   MappedWhenManaged	Boolean		True

 FORM:
 defaultDistance     Thickness		int		4

 DIALOG:
 icon		     Pixmap		Bitmap		None
 clipMask	     Pixmap		ClipMask	None
 label		     Label		String		NULL
 value		     Value		String		NULL

*/

#define XtNicon "icon"

typedef struct _DialogClassRec	*DialogWidgetClass;
typedef struct _DialogRec	*DialogWidget;

extern WidgetClass dialogWidgetClass;

_XFUNCPROTOBEGIN

extern void XawDialogAddButton(
    Widget		/* dialog */,
    String		/* name */,
    XtCallbackProc	/* function */,
    XtPointer		/* client_data */
);

extern char *XawDialogGetValueString(
    Widget		/* w */
);

_XFUNCPROTOEND

#endif /* _XawDialog_h */
