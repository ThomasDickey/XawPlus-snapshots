/*
 * $XTermId: List.h,v 1.5 2025/01/19 14:11:32 tom Exp $
 * $Xorg: List.h,v 1.4 2001/02/09 02:03:44 xorgcvs Exp $
 */

/*
Copyright 2015,2025	Thomas E. Dickey
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

/*  This is the List widget, it is useful to display a list, without the
 *  overhead of having a widget for each item in the list.  It allows
 *  the user to select an item in a list and notifies the application through
 *  a callback function.
 *
 *	Created: 	8/13/88
 *	By:		Chris D. Peterson
 *                      MIT X Consortium
 *
 * The following resources are added:
 *
 * list              List		XawIconList*    NULL
 * iconHeight	     Height		Dimension	0
 * iconWidth	     Width		Dimension	0
 * iconDepth	     Depth		Dimension	1
 *
 * The iconList is an array of strings and pixmaps. It is not required to support
 * all pixmap entries. Initialize unused pixmap entries with `XtUnspecifiedPixmap'.
 * If the resources iconWidth and/or iconHeight are set to 0 the widget don't
 * display any icon.
 * At this time, the icon is used as `left bitmap'. I plan to realize a new
 * ressource to define the position of the icon. Possible values are `left
 * bitmap', `right bitmap', `top bitmap' and `bottom bitmap'.
 */

#ifndef _XawList_h
#define _XawList_h

/***********************************************************************
 *
 * List Widget
 *
 ***********************************************************************/

#include <X11/XawPlus/Simple.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 OBJECT:
 destroyCallback     Callback		Pointer		NULL

 RECTANGLE:
 borderWidth	     BorderWidth	Dimension	1
 height		     Height		Dimension	0          **1
 sensitive	     Sensitive		Boolean		True
 width		     Width		Dimension	0          **1
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

 LIST:
 callback            Callback           XtCallbackList  NULL       **6
 pasteBuffer         Boolean            Boolean         False
 foreground	     Foreground		Pixel		XtDefaultForeground
 font		     Font		XFontStruct*	XtDefaultFont
 fontSet	     FontSet		FontSet		XtDefaultFontSet
 internalHeight	     Height		Dimension	2
 internalWidth	     Width		Dimension	4
 defaultColumns      Columns            int             2          **5
 columnSpacing       Spacing            Dimension       6
 rowSpacing          Spacing            Dimension       4
 forceColumns        Columns            Boolean         False      **5
 verticalList        Boolean            Boolean         False
 list                List               String *        NULL       **2
 iconList            List		XawIconList*    NULL	   **7
 longest             Longest            int             0          **3  **4
 numberStrings       NumberStrings      int             0          **4
 iconHeight	     Height		Dimension	0
 iconWidth	     Width		Dimension	0
 iconDepth	     Depth		Dimension	1


 **1 - If the Width or Height of the list widget is zero (0) then the value
       is set to the minimum size necessay to fit the entire list.

       If both Width and Height are zero then they are adjusted to fit the
       entire list that is created width the number of default columns
       specified in the defaultColumns resource.

 **2 - This is an array of strings the specify elements of the list.
       This resource must be specified if there is no icon list.
       (What good is a list widget without a list??  :-)

 **3 - Longest is the length of the widest string in pixels.

 **4 - If either of these values are zero (0) then the list widget calculates
       the correct value.

       (This allows you to make startup faster if you already have
        this information calculated)

       NOTE: If the numberStrings value is zero the list must
             be NULL terminated.

 **5 - By setting the List.Columns resource you can force the application to
       have a given number of columns.

 **6 - This returns the name and index of the item selected in an
       XawListReturnStruct that is pointed to by the client_data
       in the CallbackProc.

 **7 - This is an array of XawIconList entries with the elements of the list.
       This resource must be specified if there is no simple list.
*/


/*
 * Value returned when there are no highlighted objects.
 */

#define XAW_LIST_NONE -1

#define XtCList 		"List"
#define XtCSpacing 		"Spacing"
#define XtCColumns 		"Columns"
#define XtCLongest 		"Longest"
#define XtCNumberStrings 	"NumberStrings"

#define XtNcursor 		"cursor"
#define XtNcolumnSpacing 	"columnSpacing"
#define XtNdefaultColumns 	"defaultColumns"
#define XtNforceColumns		"forceColumns"
#define XtNlist			"list"
#define XtNiconList		"iconList"
#define XtNiconWidth		"iconWidth"
#define XtNiconHeight		"iconHeight"
#define XtNiconDepth		"iconDepth"
#define XtNlongest 		"longest"
#define XtNnumberStrings 	"numberStrings"
#define XtNpasteBuffer 		"pasteBuffer"
#define XtNrowSpacing 		"rowSpacing"
#define XtNverticalList 	"verticalList"

#ifndef XtNfontSet
#define XtNfontSet "fontSet"
#endif

#ifndef XtCFontSet
#define XtCFontSet "FontSet"
#endif

/* Class record constants */

extern WidgetClass listWidgetClass;

typedef struct _ListClassRec *ListWidgetClass;
typedef struct _ListRec      *ListWidget;

/* The list return structure. */

typedef struct _XawListReturnStruct {
  String string;
  int list_index;
} XawListReturnStruct;

/* The IconList structure used in lists with left bitmap */

typedef struct _XawIconList {
  Pixmap bitmap;	/* the icon, used as left bitmap */
  Pixmap clipMask;	/* drawing mask for the icon */
  String string;	/* text for the list entry */
} XawIconList;

/********************************************************************************
 *
 * Exported Functions
 *
 ********************************************************************************/

_XFUNCPROTOBEGIN

/* --- Handling of a simple text list ----------------------------------------- */

/*	Function Name: XawListChange.
 *	Description: Changes the LIST being used and shown.
 *	Arguments: w -		the list widget.
 *                 list -	the new list.
 *                 nitems -	the number of items in the list.
 *                 longest - 	the length (in Pixels) of the longest element
 *                           	in the list.
 *                 resize - 	if TRUE the the list widget will
 *                          	try to resize itself.
 *	Returns: none.
 *      NOTE:      If nitems of longest are <= 0 then they will be calculated.
 *                 If nitems is <= 0 then the list needs to be NULL terminated.
 */

extern void XawListChange(
    Widget		/* w */,
    String*		/* list */,
    int			/* nitems */,
    int			/* longest */,
    Boolean		/* resize */
);

/*	Function Name: XawListUnhighlight
 *	Description: unlights the current highlighted element.
 *	Arguments: w - the widget.
 *	Returns: none.
 */

extern void XawListUnhighlight(
    Widget		/* w */
);

/*	Function Name: XawListHighlight
 *	Description: Highlights the given item.
 *	Arguments: w - the list widget.
 *                 item - the item to highlight.
 *	Returns: none.
 */

extern void XawListHighlight(
    Widget		/* w */,
    int			/* item */
);

/*	Function Name: XawListShowCurrent
 *	Description: returns the currently highlighted LIST object.
 *	Arguments: w - the list widget.
 *	Returns: the info about the currently highlighted object.
 */

extern XawListReturnStruct * XawListShowCurrent(
    Widget		/* w */
);


/* --- Handling of a combined text / icon list -------------------------------- */

/*	Function Name: XawIconListChange.
 *	Description: Changes the ICONLIST being used and shown.
 *	Arguments: w - 		the list widget.
 *                 list - 	the new list.
 *                 nitems - 	the number of items in the list.
 *                 longest - 	the length (in Pixels) of the longest element
 *                           	in the list.
 *		   icon_w -	width of all icons
 *		   icon_h -	height of all icons
 *		   icon_d -	depth of all icons
 *                 resize - 	if TRUE the the list widget will
 *                          	try to resize itself.
 *	Returns: none.
 *      NOTE:      If nitems of longest are <= 0 then they will be calculated.
 *                 If nitems is <= 0 then the list needs to be NULL terminated.
 */

extern void XawIconListChange(
    Widget		/* w */,
    XawIconList*	/* list */,
    int			/* no of items in list */,
    int			/* longest string */,
    int			/* icon width  */,
    int			/* icon height */,
    int			/* icon depth  */,
    Boolean		/* resize */
);

#define XawIconListUnhighlight XawListUnhighlight
#define XawIconListHighlight   XawListHighlight
#define XawIconListShowCurrent XawListShowCurrent

_XFUNCPROTOEND

#endif /* _XawList_h */
