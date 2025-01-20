/*
 * $XTermId: CommandP.h,v 1.3 2025/01/19 15:20:11 tom Exp $
 * $Xorg: CommandP.h,v 1.4 2001/02/09 02:03:43 xorgcvs Exp $
 */

/***************************************************************************

Copyright 2022,2025  Thomas E. Dickey
Copyright 1987, 1988, 1998  The Open Group

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

This file contains modifications for XawPlus, Roland Krause 2000

****************************************************************************/

/*
 * CommandP.h - Private definitions for Command widget
 *
 */

#ifndef _XawCommandP_h
#define _XawCommandP_h

#include <X11/XawPlus/Command.h>
#include <X11/XawPlus/LabelP.h>

/***********************************************************************
 *
 * Command Widget Private Data
 *
 ***********************************************************************/

#define HELP_TIMER		1200	/* Time until the help window pops up */
#define DEFAULT_THICKNESS 	2	/* Default for 'highlight_thickness' */

/***********************************************************************
 *
 *  Class structure
 *
 ***********************************************************************/


/* New fields for the Command widget class record */

typedef struct _CommandClass
{
  int makes_compiler_happy;  /* not used */
} CommandClassPart;

/* Full class record declaration */

typedef struct _CommandClassRec
{
  CoreClassPart		core_class;
  SimpleClassPart	simple_class;
  LabelClassPart	label_class;
  CommandClassPart	command_class;
} CommandClassRec;

extern CommandClassRec commandClassRec;

/***************************************
 *
 *  Instance (widget) structure
 *
 **************************************/

/* New fields for the Command widget record */

typedef struct {
    String		helpText;		/* Text for the tooltip */
    Boolean		useHelp;		/* Use tooltip or not ? */
    Pixel		helpBackground;		/* Background color of tooltip */

    Dimension		highlight_thickness;	/* Border around the button */
    Boolean		highlightMode;		/* New highlight mode */
    XtCallbackList	callbacks;

/* private state */

    Boolean     	set;

    XtIntervalId	helpTimer;   		/* used for the tooltip */
    Widget		helpWidget,
			helpLabel;
} CommandPart;

/* Full widget declaration */

typedef struct _CommandRec {
    CorePart         	core;
    SimplePart	     	simple;
    LabelPart	     	label;
    CommandPart      	command;
} CommandRec;

#endif /* _XawCommandP_h */
