/*
 * $XTermId: SmeBSB.h,v 1.3 2025/01/19 15:20:11 tom Exp $
 * $Xorg: SmeBSB.h,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
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
 */

/*
 * SmeBSB.h - Public Header file for SmeBSB object.
 *
 * This is the public header file for the Athena BSB Sme object.
 * It is intended to be used with the simple menu widget.  This object
 * provides bitmap - string - bitmap style entries.
 *
 * Date:    April 3, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 *
 * This file contains modifications for XawPlus, Roland Krause 2002
 */

#ifndef _SmeBSB_h
#define _SmeBSB_h

#include <X11/Xmu/Converters.h>
#include <X11/XawPlus/Sme.h>

/****************************************************************
 *
 * SmeBSB object
 *
 ****************************************************************

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer		NULL

 RECTANGLE:
 borderWidth	     BorderWidth	Dimension	0
 height		     Height		Dimension	0
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	0
 x		     Position		Position	0
 y		     Position		Position	0

 SME:
 callback            Callback           Callback        NULL
 international	     International      Boolean		False
 highlightColor	     Background		Pixel		grey90
 shadowColor	     Background		Pixel		grey40

 SMEBSB:
 foreground          Foreground         Pixel           XtDefaultForeground
 font                Font               XFontStruct *   XtDefaultFont
 fontSet	     FontSet		FontSet		XtDefaultFontSet
 encoding	     Encoding		unsigned char	XawTextEncoding8bit
 label               Label              String          Name of entry
 justify	     Justify		Justify	        XtJustifyLeft
 vertSpace           VertSpace          int             25
 leftMargin          HorizontalMargins  Dimension       4
 rightMargin         HorizontalMargins  Dimension       4
 leftBitmap          LeftBitmap         Bitmap          None
 leftClipMask	     LeftBitmap		ClipMask	None
 rightBitmap         RightBitmap        Bitmap          None
 rightClipMask	     RightBitmap	ClipMask	None
*/

typedef struct _SmeBSBClassRec    *SmeBSBObjectClass;
typedef struct _SmeBSBRec         *SmeBSBObject;

extern WidgetClass smeBSBObjectClass;

#define XawTextEncoding8bit	0
#define XawTextEncodingChar2b	1

#define XtNleftBitmap	"leftBitmap"
#define XtNleftClipMask "leftClipMask"
#define XtNrightBitmap	"rightBitmap"
#define XtNrightClipMask "rightClipMask"
#define XtNleftMargin	"leftMargin"
#define XtNrightMargin	"rightMargin"
#define XtNvertSpace	"vertSpace"
#define XtNencoding	"encoding"

#define XtCLeftBitmap	"LeftBitmap"
#define XtCHorizontalMargins "HorizontalMargins"
#define XtCRightBitmap	"RightBitmap"
#define XtCVertSpace	"VertSpace"
#define XtCEncoding	"Encoding"

#define XtRClipMask	"ClipMask"

#endif /* _SmeBSB_h */
