/*
 * $XTermId: StrToPmap.c,v 1.4 2022/12/13 00:53:17 tom Exp $
 */

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
 *		   screenConvertArg, XtNumber(screenConvertArg));
 *
 * Copyright 2022  Thomas E. Dickey
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

#include "private.h"

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/Drawing.h>

#include <X11/XawPlus/StrToPmap.h>

/******************************************************************************************
 *
 * XawCvtStringToPixmap(): Converter for XPM and XBM styled pixmaps
 * 			   If the filname ends with `.xpm', the converter
 * 			   try to convert it as XPM pixmap file, else
 *			   as XBM bitmap.
 *
 ******************************************************************************************/

void XawCvtStringToPixmap(args, numArgs, fromVal, toVal)
XrmValuePtr args;
Cardinal   *numArgs;
XrmValuePtr fromVal;
XrmValuePtr toVal;
{
    static Pixmap pixmap, clipMask;
    char *name = (char *)fromVal->addr;

    /* Number of arguments correct ? */

    if (*numArgs != 2)
    {
       XtWarning("XawCvtStringToPixmap: String to pixmap conversion "
		 "needs screen and colormap arguments.\n");
       return;
    }
    /* Check, if filename ends with `.xpm'. If so, try to convert the file
     * as XPM file format, else try bitmap file format.
     */
    if (strstr(name, ".xpm") == NULL)
         pixmap = XmuLocateBitmapFile(*((Screen **)args[0].addr),
				      name, NULL, 0, NULL, NULL, NULL, NULL);

    else pixmap = XawLocatePixmapFile(*((Screen **)args[0].addr),
				      *((Colormap *)args[1].addr),
				      name, NULL, 0, NULL, NULL, &clipMask);
    /* Conversion failed ? */

    if (pixmap == None) XtStringConversionWarning(name, "Pixmap");

    /* Store the result */

    toVal->size = sizeof(Pixmap);
    toVal->addr = (XtPointer)&pixmap;
}

/******************************************************************************************
 *
 * XawCvtStringToClipMask():Clip mask converter for XPM styled pixmaps.
 * 			    If the filname ends with `.xpm', the converter
 * 			    try to convert it as XPM pixmap file, else
 *			    the conversion fails.
 *
 ******************************************************************************************/

void XawCvtStringToClipMask(args, numArgs, fromVal, toVal)
XrmValuePtr args;
Cardinal   *numArgs;
XrmValuePtr fromVal;
XrmValuePtr toVal;
{
    static Pixmap clipMask;
    char *name = (char *)fromVal->addr;

    /* Number of arguments correct ? */

    if (*numArgs != 2)
    {
       XtWarning("XawCvtStringToClipMask: String to clipMask conversion "
		 "needs screen and colormap arguments.\n");
       return;
    }
    /* Check, if filename ends with `.xpm'. If so, try to convert the file
     * as XPM file format, else try bitmap file format.
     */
    if (strstr(name, ".xpm") == NULL)
      clipMask = XmuLocateBitmapFile(*((Screen **)args[0].addr), name, NULL, 0, NULL, NULL, NULL, NULL);
    else (void)XawLocatePixmapFile(*((Screen **)args[0].addr), *((Colormap *)args[1].addr),
				   name, NULL, 0, NULL, NULL, &clipMask);
    /* Conversion failed ? */

    if (clipMask == None) XtStringConversionWarning(name, "ClipMask");

    /* Store the result */

    toVal->size = sizeof(Pixmap);
    toVal->addr = (XtPointer)&clipMask;
}
