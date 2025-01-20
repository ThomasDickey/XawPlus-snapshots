/*
 * $XTermId: Simple.h,v 1.3 2025/01/19 14:11:32 tom Exp $
 * $Xorg: Simple.h,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 */

/*************************************************************************

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

**************************************************************************/

#ifndef _Simple_h
#define _Simple_h

#include <X11/Xmu/Converters.h>

/****************************************************************
 *
 * Simple widgets
 *
 ****************************************************************/

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
 highlightColor	     Background		Pixel		grey90
 shadowColor	     Background		Pixel		grey40
 buttonBorderWidth   Width		Dimension	2

*/

#define XtNhighlightColor		"highlightColor"
#define XtNshadowColor			"shadowColor"
#define XtNbuttonBorderWidth		"buttonBorderWidth"
#define XtNcursor 			"cursor"
#define XtNcursorName			"cursorName"
#define XtNinsensitiveBorder		"insensitiveBorder"

#define XtCInsensitive			"Insensitive"

#ifndef XtCInternational
#define XtCInternational		"International"
#endif

#ifndef XtNinternational
#define XtNinternational		"international"
#endif

typedef struct _SimpleClassRec	*SimpleWidgetClass;
typedef struct _SimpleRec	*SimpleWidget;

extern WidgetClass simpleWidgetClass;


/******************************************************************
 *
 * Exported class methods
 *
 *****************************************************************/

/* Simple has the following drawing methods :
 *
 *  XawFlatRectangle()	 : Draw a flat rectangle to hide the border
 *  XawRaisedRectangle() : Draw a rised 3D rectangle
 *  XawSunkenRectangle() : Draw a sunken 3D rectangle
 *  XawFlatButton()	 : Draw a flat button
 *  XawRaisedButton()    : Draw a raised 3D styled button
 *  XawSunkenButton()    : Draw a sunken 3D styled button
 *
 * All functions has the following arguments:
 *
 *  Arguments	  : w		 - the widget
 *		    x,y		 - origin of the rectangle
 *		    width, height- Dimension of the rectangle
 *  Returns	  : none
 */

_XFUNCPROTOBEGIN

extern void XawFlatRectangle(
	Widget,				/* w    */
	int, int,			/* x, y */
	unsigned int, unsigned int	/* width, height */
);

extern void XawRaisedRectangle(
	Widget,				/* w    */
	int, int,			/* x, y */
	unsigned int, unsigned int	/* width, height */
);

extern void XawSunkenRectangle(
	Widget,				/* w    */
	int, int,			/* x, y */
	unsigned int, unsigned int	/* width, height */
);

extern void XawFlatButton(
	Widget,				/* w    */
	int, int,			/* x, y */
	unsigned int, unsigned int	/* width, height */
);

extern void XawRaisedButton(
	Widget,				/* w    */
	int, int,			/* x, y */
	unsigned int, unsigned int	/* width, height */
);

extern void XawSunkenButton(
	Widget,				/* w    */
	int, int,			/* x, y */
	unsigned int, unsigned int	/* width, height */
);

_XFUNCPROTOEND

#endif /* _Simple_h */
