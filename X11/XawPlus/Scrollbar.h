/*
 * $XTermId: Scrollbar.h,v 1.4 2025/01/19 14:11:32 tom Exp $
 * $Xorg: Scrollbar.h,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 */

/**************************************************************************

Copyright 2015,2025	Thomas E. Dickey
Copyright 2002  Roland Krause
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

***************************************************************************/

#ifndef _Scrollbar_h
#define _Scrollbar_h

/****************************************************************
 *
 * Scrollbar Widget
 *
 ****************************************************************/

#include <X11/Xmu/Converters.h>
#include <X11/Xfuncproto.h>

/* Scrollbar resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer		NULL

 RECTANGLE:
 borderWidth	     BorderWidth	Dimension	1
 height		     Height		Dimension	length or thickness
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	thickness or length
 x		     Position		Position	0
 y		     Position		Position	0

 CORE:
 border		     BorderColor	Pixel		XtDefaultForeground
 background	     Background		Pixel		grey75
 mappedWhenManaged   MappedWhenManaged	Boolean		True

 SIMPLE:
 cursor		     Cursor		Cursor		None
 cursorName	     Cursor		String		left_ptr
 pointerColor        Foreground         Pixel           XtDefaultForeground
 pointerColorBackground Background      Pixel           XtDefaultBackground
 insensitiveBorder   Insensitive	Pixmap		Gray
 international	     International      Boolean		False
 highlightColor	     Background		Pixel		grey90
 shadowColor	     Background		Pixel		grey40
 buttonBorderWidth   Width		Dimension	2

 SCROLLBAR:
 length		     Length		Dimension	1
 minimumThumb	     MinimumThumb	Dimension	12
 thickness	     Thickness		Dimension	16
 orientation	     Orientation	XtOrientation	XtorientVertical
 shown		     Shown		Float		0.0
 topOfThumb	     TopOfThumb		Float		0.0
 pickTop	     PickTop		Boolean		False

 accelerators	     Accelerators	AcceleratorTable NULL
 ancestorSensitive   AncestorSensitive	Boolean		True
 jumpProc	     Callback		XtCallbackList	NULL
 scrollProc	     Callback		XtCallbackList	NULL
 thumbProc	     Callback		XtCallbackList	NULL
 translations	     Translations	TranslationTable see source or doc

*/

/*
 * Most things we need are in StringDefs.h
 */

#define XtCMinimumThumb "MinimumThumb"
#define XtCShown	"Shown"
#define XtCTopOfThumb	"TopOfThumb"
#define XtCPickTop	"PickTop"

#define XtNminimumThumb "minimumThumb"
#define XtNtopOfThumb	"topOfThumb"
#define XtNpickTop	"pickTop"

typedef struct _ScrollbarRec	  *ScrollbarWidget;
typedef struct _ScrollbarClassRec *ScrollbarWidgetClass;

extern WidgetClass scrollbarWidgetClass;

_XFUNCPROTOBEGIN

extern void XawScrollbarSetThumb(
    Widget		/* scrollbar */,
    float		/* top */,
    float		/* shown */
);

_XFUNCPROTOEND

#endif /* _Scrollbar_h */
