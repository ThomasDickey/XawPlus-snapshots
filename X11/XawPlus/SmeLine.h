/*
 * $Xorg: SmeLine.h,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 *
Copyright 1989, 1998  The Open Group

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
 */

/*
 * SmeLine.h - Public Header file for SmeLine object.
 *
 * This is the public header file for the Athena SmeLine object.
 * It is intended to be used with the simple menu widget.  
 *
 * Date:    April 3, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium 
 *          kit@expo.lcs.mit.edu
 *
 * This file contains modifications for XawPlus, Roland Krause 2002
 */

#ifndef _SmeLine_h
#define _SmeLine_h

#include <X11/XawPlus/Sme.h>
#include <X11/Xmu/Converters.h>

/****************************************************************
 *
 * SmeLine Object
 *
 ****************************************************************/

/* Menu Entry Resources:

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

 SMELINE:
 lineWidth	     Width		Dimension	1


*/

#define XtCLineWidth "LineWidth"
#define XtCStipple "Stipple"

#define XtNlineWidth "lineWidth"
#define XtNstipple "stipple"

typedef struct _SmeLineClassRec*	SmeLineObjectClass;
typedef struct _SmeLineRec*	        SmeLineObject;

extern WidgetClass smeLineObjectClass;

#endif /* _SmeLine_h */
