/*
 * $XTermId: IconList.h,v 1.3 2025/01/19 15:20:11 tom Exp $
 */

/* This is the replacement for the old IconList header file.
 * Since the IconList code is merged into the list widget,
 * this header file remains only for compatibility.
 *
 * If you try create an IconList widget as in XawPlus 2.x in fact
 * a compatible List widget will be created.
 *
 * Copyright 2022,2025  Thomas E. Dickey
 * Copyright (c) Roland Krause 2002, roland_krause@freenet.de
 *
 * This module is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _XawIconList_h
#define _XawIconList_h

/***********************************************************************
 *
 * Icon List Widget
 *
 ***********************************************************************/

#include <X11/XawPlus/List.h>

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
 highlightColor	     Background		Pixel		grey90
 shadowColor	     Background		Pixel		grey40
 buttonBorderWidth   Width		Dimension	2

 ICONLIST:
 callback            Callback           XtCallbackList  NULL
 pasteBuffer         Boolean            Boolean         False
 foreground	     Foreground		Pixel		XtDefaultForeground
 font		     Font		XFontStruct*	XtDefaultFont
 internalHeight	     Height		Dimension	2
 internalWidth	     Width		Dimension	4
 defaultColumns      Columns            int             2
 columnSpacing       Spacing            Dimension       6
 rowSpacing          Spacing            Dimension       4
 forceColumns        Columns            Boolean         False
 verticalList        Boolean            Boolean         False
 list                List		XawIconList*    NULL
 longest             Longest            int             0
 numberStrings       NumberStrings      int             0
 iconHeight	     Height		Dimension	0
 iconWidth	     Width		Dimension	0
 iconDepth	     Depth		Dimension	1

 */

#undef  XtNlist
#define XtNlist	XtNiconList
#undef  XtNiconList

#define XawIconListReturnStruct XawListReturnStruct

/* Class record constants */

extern WidgetClass iconListWidgetClass;

typedef struct _ListClassRec *IconListWidgetClass;
typedef struct _ListRec      *IconListWidget;

#endif /* _XawIconList_h */
/* DON'T ADD STUFF AFTER THIS #endif */
