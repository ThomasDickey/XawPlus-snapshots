/*
 * $XTermId: SimpleMenu.h,v 1.3 2025/01/19 14:11:32 tom Exp $
 * $Xorg: SimpleMenu.h,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 *
Copyright 2015,2025 Thomas E. Dickey
Copyright 2002 Roland Krause
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
 *
 * Author:  Chris D. Peterson, MIT X Consortium
 */

/*
 * SimpleMenu.h - Public Header file for SimpleMenu widget.
 *
 * This is the public header file for the Athena SimpleMenu widget.
 * It is intended to provide one pane pulldown and popup menus within
 * the framework of the X Toolkit.  As the name implies it is a first and
 * by no means complete implementation of menu code. It does not attempt to
 * fill the needs of all applications, but does allow a resource oriented
 * interface to menus.
 *
 * Date:    April 3, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 */

#ifndef _SimpleMenu_h
#define _SimpleMenu_h

#include <X11/Shell.h>
#include <X11/Xmu/Converters.h>

/****************************************************************
 *
 * SimpleMenu widget
 *
 ****************************************************************/

/* SimpleMenu Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer		NULL

 RECTANGLE:
 sensitive	     Sensitive		Boolean		True
 borderWidth	     BorderWidth	Dimension	1
 height		     Height		Dimension	0
 width		     Width		Dimension	0
 x		     Position		Position	0
 y		     Position		Position	0

 CORE:
 border		     BorderColor	Pixel		XtDefaultForeground
 background	     Background		Pixel		grey75
 mappedWhenManaged   MappedWhenManaged	Boolean		True

 SHELL:
 allowShellResize    AllowShellResize	Boolean		True
 geometry	     Geometry		String		NULL
 overrideRedirect    OverrideRedirect	Boolean		see documentation
 popdownCallback     Callback		Callback	see documentation
 popupCallback       Callback		Callback	see documentation
 saveUnder	     SaveUnder		Boolean		True

 SIMPLEMENU:
 backingStore	     BackingStore	BackingStore	default
 menuOnScreen	     MenuOnScreen	Boolean		True
 popupOnEntry	     PopupOnEntry	Widget		NULL
 cursor              Cursor             Cursor          None
 label               Label              String          NULL (No label)
 labelClass          LabelClass         Pointer         smeBSBObjectClass
 rowHeight           RowHeight          Dimension       Height of Font
 bottomMargin        VerticalMargins    Dimension       VerticalSpace
 topMargin           VerticalMargins    Dimension       VerticalSpace

 NEW SIMPLEMENU RESOURCES:
 highlightColor	     Background		Pixel		grey90
 shadowColor	     Background		Pixel		grey40
 buttonBorderWidth   Width		Dimension	2		*)

 *) Border width of the menu.

*/

typedef struct _SimpleMenuClassRec*	SimpleMenuWidgetClass;
typedef struct _SimpleMenuRec*		SimpleMenuWidget;

extern WidgetClass simpleMenuWidgetClass;

#define XtNcursor "cursor"
#define XtNbottomMargin "bottomMargin"
#define XtNcolumnWidth "columnWidth"
#define XtNlabelClass "labelClass"
#define XtNmenuOnScreen "menuOnScreen"
#define XtNpopupOnEntry "popupOnEntry"
#define XtNrowHeight "rowHeight"
#define XtNtopMargin "topMargin"

#define XtCColumnWidth "ColumnWidth"
#define XtCLabelClass "LabelClass"
#define XtCMenuOnScreen "MenuOnScreen"
#define XtCPopupOnEntry "PopupOnEntry"
#define XtCRowHeight "RowHeight"
#define XtCVerticalMargins "VerticalMargins"

/************************************************************
 *
 * Public Functions.
 *
 ************************************************************/

_XFUNCPROTOBEGIN

/*	Function Name: XawSimpleMenuAddGlobalActions
 *	Description: adds the global actions to the simple menu widget.
 *	Arguments: app_con - the appcontext.
 *	Returns: none.
 */

extern void XawSimpleMenuAddGlobalActions(
    XtAppContext	/* app_con */
);

/*	Function Name: XawSimpleMenuGetActiveEntry
 *	Description: Gets the currently active (set) entry.
 *	Arguments: w - the smw widget.
 *	Returns: the currently set entry or NULL if none is set.
 */

extern Widget XawSimpleMenuGetActiveEntry(
    Widget		/* w */
);

/*	Function Name: XawSimpleMenuClearActiveEntry
 *	Description: Unsets the currently active (set) entry.
 *	Arguments: w - the smw widget.
 *	Returns: none.
 */

extern void XawSimpleMenuClearActiveEntry(
    Widget		/* w */
);

_XFUNCPROTOEND

#endif /* _SimpleMenu_h */
