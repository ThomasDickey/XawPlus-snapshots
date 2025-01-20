/*
 * $XTermId: MenuButton.h,v 1.3 2025/01/19 15:20:11 tom Exp $
 * $Xorg: MenuButton.h,v 1.4 2001/02/09 02:03:44 xorgcvs Exp $
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

This file contains modifications for XawPlus, Roland Krause 2002

*/

/***********************************************************************
 *
 * MenuButton Widget
 *
 ***********************************************************************/

/*
 * MenuButton.h - Public Header file for MenuButton widget.
 *
 * This is the public header file for the Athena MenuButton widget.
 * It is intended to provide an easy method of activating pulldown menus.
 *
 * Date:    May 2, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 */

#ifndef _XawMenuButton_h
#define _XawMenuButton_h

#include <X11/XawPlus/Command.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer		NULL

 RECTANGLE:
 borderWidth	     BorderWidth	Dimension	1
 height		     Height		Dimension	text height
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	text width
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

 LABEL:
 font		     Font		XFontStruct*	XtDefaultFont
 fontSet	     FontSet		FontSet		XtDefaultFontSet
 encoding	     Encoding		unsigned char	XawTextEncoding8bit
 foreground	     Foreground		Pixel		XtDefaultForeground
 internalHeight	     Height		Dimension	2
 internalWidth	     Width		Dimension	4
 justify	     Justify		XtJustify	XtJustifyCenter
 resize		     Resize		Boolean		True
 truncateLabel	     Truncate		Boolean		True
 truncLeftSide	     Truncate		Boolean         False
 label		     Label		String		NULL
 leftBitmap	     LeftBitmap		Bitmap		None *)
 leftClipMask	     LeftBitmap		ClipMask	None *)
 bitmap		     Pixmap		Bitmap		None *)
 clipMask	     Pixmap		ClipMask	None *)

 COMMAND:
 highlightThickness  Thickness		Dimension	0 if shaped, else 2
 callback	     Callback		XtCallbackList	NULL
 translations	     Translations	TranslationTable see doc or source
 accelerators	     Accelerators	AcceleratorTable NULL
 ancestorSensitive   AncestorSensitive	Boolean		True

 MENUBUTTON:
 menuName            MenuName           String          "menu"

 *)  Standard XBM bitmap with depth 1 or XPM styled pixmap with depth > 1

*/

#define XtNmenuName "menuName"
#define XtCMenuName "MenuName"

extern WidgetClass     menuButtonWidgetClass;

typedef struct _MenuButtonClassRec   *MenuButtonWidgetClass;
typedef struct _MenuButtonRec        *MenuButtonWidget;

#endif /* _XawMenuButton_h */
