/*
 * $XTermId: Toggle.h,v 1.3 2025/01/19 14:11:32 tom Exp $
 * $Xorg: Toggle.h,v 1.4 2001/02/09 02:03:47 xorgcvs Exp $
 *
Copyright 2015,2025 Thomas E. Dickey
Copyright 2002  Roland Krause
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
 * ToggleP.h - Private definitions for Toggle widget
 *
 * Author: Chris D. Peterson
 *         MIT X Consortium
 *         kit@expo.lcs.mit.edu
 *
 * Date:   January 12, 1989
 */

#ifndef _XawToggle_h
#define _XawToggle_h

/***********************************************************************
 *
 * Toggle Widget
 *
 ***********************************************************************/

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
 leftBitmap	     LeftBitmap		Bitmap		None *
 leftClipMask	     LeftBitmap		ClipMask	None *
 bitmap		     Pixmap		Bitmap		None *
 clipMask	     Pixmap		ClipMask	None *

 COMMAND:
 helpText	     Label		String		NULL
 helpEncoding	     Encoding		unsigned char	XawTextEncoding8bit
 helpBackground	     Background		Pixel		LightGoldenrodYellow
 useHelp	     Boolean		Boolean		True
 highlightThickness  Thickness		Dimension	2
 highlightMode       Boolean		Boolean		True
 callback	     Callback		XtCallbackList	NULL
 translations	     Translations	TranslationTable see doc or source
 accelerators	     Accelerators	AcceleratorTable NULL
 ancestorSensitive   AncestorSensitive	Boolean		True

 TOGGLE:
 radioGroup          RadioGroup         Widget          NULL              +
 radioData           RadioData          Pointer         (caddr_t) Widget  ++
 state               State              Boolean         Off


* Standard XBM bitmap with depth 1 or XPM styled pixmap with depth > 1

+ To use the toggle as a radio toggle button, set this resource to point to
  any other widget in the radio group.

++ This is the data returned from a call to XtToggleGetCurrent, by default
   this is set to the name of toggle widget.

*/

/*
 * These should be in StringDefs.h but aren't so we will define
 * them here if they are needed.
 */


#define XtCWidget "Widget"
#define XtCState "State"
#define XtCRadioGroup "RadioGroup"
#define XtCRadioData "RadioData"

#ifndef _XtStringDefs_h_
#define XtRWidget "Widget"
#endif

#define XtNstate "state"
#define XtNradioGroup "radioGroup"
#define XtNradioData "radioData"

extern WidgetClass               toggleWidgetClass;

typedef struct _ToggleClassRec   *ToggleWidgetClass;
typedef struct _ToggleRec        *ToggleWidget;


/************************************************************
 *
 * Public Functions
 *
 ************************************************************/

_XFUNCPROTOBEGIN

/*	Function Name: XawToggleChangeRadioGroup
 *	Description: Allows a toggle widget to change radio lists.
 *	Arguments: w - The toggle widget to change lists.
 *                 radio_group - any widget in the new list.
 *	Returns: none.
 */

extern void XawToggleChangeRadioGroup(
    Widget		/* w */,
    Widget		/* radio_group */
);

/*	Function Name: XawToggleGetCurrent
 *	Description: Returns the RadioData associated with the toggle
 *                   widget that is currently active in a toggle list.
 *	Arguments: radio_group - any toggle widget in the toggle list.
 *	Returns: The XtNradioData associated with the toggle widget.
 */

extern XtPointer XawToggleGetCurrent(
    Widget		/* radio_group */
);

/*	Function Name: XawToggleSetCurrent
 *	Description: Sets the Toggle widget associated with the
 *                   radio_data specified.
 *	Arguments: radio_group - any toggle widget in the toggle list.
 *                 radio_data - radio data of the toggle widget to set.
 *	Returns: none.
 */

extern void XawToggleSetCurrent(
    Widget		/* radio_group */,
    XtPointer		/* radio_data */
);

/*	Function Name: XawToggleUnsetCurrent
 *	Description: Unsets all Toggles in the radio_group specified.
 *	Arguments: radio_group - any toggle widget in the toggle list.
 *	Returns: none.
 */

extern void XawToggleUnsetCurrent(
    Widget		/* radio_group */
);

_XFUNCPROTOEND

#endif /* _XawToggle_h */
