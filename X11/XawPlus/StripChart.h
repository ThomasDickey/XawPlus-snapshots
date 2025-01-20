/*
 * $XTermId: StripChart.h,v 1.3 2025/01/19 15:20:11 tom Exp $
 * $Xorg: StripChart.h,v 1.5 2001/02/09 02:03:46 xorgcvs Exp $
 */

/***********************************************************

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

************************************************************************/

#ifndef _XawStripChart_h
#define _XawStripChart_h

#include <X11/Intrinsic.h>

/***********************************************************************
 *
 * StripChart Widget
 *
 ***********************************************************************/

/* StripChart resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer		NULL

 RECTANGLE:
 borderWidth	     BorderWidth	Dimension	1
 height		     Height		Dimension	120
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	120
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
 highlightColor	     Background		Pixel		grey90
 shadowColor	     Background		Pixel		grey40
 buttonBorderWidth   Width		Dimension	2

 STRIPCHART:
 update		     Interval		int		10 (seconds)
 minScale	     Scale		int		1
 foreground	     Foreground		Pixel		XtDefaultForeground
 highlight	     Foreground		Pixel		XtDefaultForeground
 getValue	     Callback		XtCallbackList	NULL
 jumpScroll	     JumpScroll		int		1/2 width

*/

#define DEFAULT_JUMP -1

#ifndef _XtStringDefs_h_
#define XtNhighlight "highlight"
#define XtNupdate "update"
#endif

#define XtCJumpScroll "JumpScroll"
#define XtCScale "Scale"

#define XtNgetValue "getValue"
#define XtNjumpScroll "jumpScroll"
#define XtNminScale "minScale"
#define XtNscale "scale"
#define XtNvmunix "vmunix"

typedef struct _StripChartRec *StripChartWidget;
typedef struct _StripChartClassRec *StripChartWidgetClass;

extern WidgetClass stripChartWidgetClass;

#endif /* _XawStripChart_h */
/* DON'T ADD STUFF AFTER THIS #endif */
