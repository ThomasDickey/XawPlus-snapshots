
/******************************************************************************************
 *
 * XawConvertStringToPixmap(), XawConvertStringToClipMask():
 *
 * Creates a depth-n Pixmap and its clip mask suitable for icons.
 * "string" represents a pixmap filename which may be absolute,
 * or relative to the global resource pixmapFilePath, class
 * PixmapFilePath.  If the resource is not defined, the default
 * value is the build symbol PIXMAPDIR.
 *
 * To use, include the following in your ClassInitialize procedure:
 *
 * static XtConvertArgRec screenConvertArg[] = {
 *  {XtWidgetBaseOffset, (XtPointer)XtOffsetOf(Widget, core.screen), sizeof(Screen *)},
 *  {XtWidgetBaseOffset, (XtPointer)XtOffsetOf(Widget, core.colormap), sizeof(Colormap)},
 * };
 *
 * XtAddConverter(XtRString, XtRPixmap, XawCvtStringToPixmap,
 *      screenConvertArg, XtNumber(screenConvertArg));
 *
 * Please don't use the new style installer XtSetTypeConverter()!
 * I used the old converter interface to avoid problems with editres.
 * Editres seems not to work correctly with new style converters.
 *
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
 *
 ******************************************************************************************/

#ifndef _StrToPmap_h
#define _StrToPmap_h

void XawCvtStringToPixmap(
#if NeedFunctionPrototypes
	XrmValuePtr,		/* Argument list for the converter	*/
	Cardinal *,		/* Number of arguments in the list	*/
	XrmValuePtr,		/* Resource to convert			*/
	XrmValuePtr		/* Conversion result			*/
#endif
);

void XawCvtStringToClipMask(
#if NeedFunctionPrototypes
	XrmValuePtr,		/* Argument list for the converter	*/
	Cardinal *,		/* Number of arguments in the list	*/
	XrmValuePtr,		/* Resource to convert			*/
	XrmValuePtr		/* Conversion result			*/
#endif
);

#endif

