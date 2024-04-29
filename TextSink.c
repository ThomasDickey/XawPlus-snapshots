/*
 * $XTermId: TextSink.c,v 1.9 2024/04/28 23:41:45 tom Exp $
 * $Xorg: TextSink.c,v 1.4 2001/02/09 02:03:46 xorgcvs Exp $
 */

/*
Copyright 2015-2022,2024  Thomas E. Dickey
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

/*
 * Author:  Chris Peterson, MIT X Consortium.
 *
 * Much code taken from X11R3 AsciiSink.
 */

/*
 * TextSink.c - TextSink object. (For use with the text widget).
 *
 */

#include "private.h"

#include <ctype.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/TextSinkP.h>
#include <X11/XawPlus/TextP.h>

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

static void ClassInit(void);
static void ClassPartInitialize(WidgetClass /* wc */ );
static void Initialize(Widget /* request */
		       , Widget /* new */ , ArgList	/* args */
		       ,Cardinal * /* num_args */ );
static void Destroy(Widget /* w */ );
static Boolean SetValues(Widget /* current */
			 , Widget /* request */
			 , Widget /* new */
			 , ArgList /* args */
			 , Cardinal * /* num_args */ );

static int MaxHeight(Widget /* w */ , int /* lines */ );
static int MaxLines(Widget /* w */ , unsigned /* height */ );
static void DisplayText(Widget /* w */ , int /* x */
			, int /* y */ , XawTextPosition		/* pos1 */
			,XawTextPosition /* pos2 */
			, Bool /* highlight */ );
static void InsertCursor(Widget /* w */ , int /* x */
			 , int /* y */ , XawTextInsertState	/* state */
);
static void ClearToBackground(Widget /* w */
			      , int /* x */
			      , int /* y */
			      , unsigned /* width */
			      , unsigned /* height */ );
static void FindPosition(Widget /* w */ , XawTextPosition	/* fromPos */
			 ,int /* fromx */ , int /* width */
			 , Bool /* stopAtWordBreak */
			 , XawTextPosition * /* resPos */
			 , int * /* resWidth */
			 , int * /* resHeight */ );
static void FindDistance(Widget /* w */ , XawTextPosition	/* fromPos */
			 ,int /* fromx */ , XawTextPosition	/* toPos */
			 ,int * /* resWidth */
			 , XawTextPosition * /* resPos */
			 , int * /* resHeight */ );
static void Resolve(Widget /* w */ , XawTextPosition	/* pos */
		    ,int /* fromx */ , int /* width */
		    , XawTextPosition * /* resPos */ );
static void SetTabs(Widget /* w */ , int /* tab_count */
		    , short * /* tabs */ );
static void GetCursorBounds(Widget /* w */
			    , XRectangle * /* rect */ );

#define offset(field) XtOffsetOf(TextSinkRec, text_sink.field)
static XtResource resources[] =
{
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
     offset(foreground), XtRString, XtDefaultForeground},
    {XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
     offset(background), XtRString, "grey75"},
};
#undef offset

#define SuperClass		(&objectClassRec)
TextSinkClassRec textSinkClassRec =
{
    {
/* core_class fields */
    /* superclass               */ (WidgetClass) SuperClass,
    /* class_name               */ "TextSink",
    /* widget_size              */ sizeof(TextSinkRec),
    /* class_initialize         */ ClassInit,
    /* class_part_initialize    */ ClassPartInitialize,
    /* class_inited             */ FALSE,
    /* initialize               */ Initialize,
    /* initialize_hook          */ NULL,
    /* obj1                     */ NULL,
    /* obj2                     */ NULL,
    /* obj3                     */ 0,
    /* resources                */ resources,
    /* num_resources            */ XtNumber(resources),
    /* xrm_class                */ NULLQUARK,
    /* obj4                     */ FALSE,
    /* obj5                     */ FALSE,
    /* obj6                     */ FALSE,
    /* obj7                     */ FALSE,
    /* destroy                  */ Destroy,
    /* obj8                     */ NULL,
    /* obj9                     */ NULL,
    /* set_values               */ SetValues,
    /* set_values_hook          */ NULL,
    /* obj10                    */ NULL,
    /* get_values_hook          */ NULL,
    /* obj11                    */ NULL,
    /* version                  */ XtVersion,
    /* callback_private         */ NULL,
    /* obj12                    */ NULL,
    /* obj13                    */ NULL,
    /* obj14                    */ NULL,
    /* extension                */ NULL
    },
/* textSink_class fields */
    {
    /* DisplayText              */ DisplayText,
    /* InsertCursor             */ InsertCursor,
    /* ClearToBackground        */ ClearToBackground,
    /* FindPosition             */ FindPosition,
    /* FindDistance             */ FindDistance,
    /* Resolve                  */ Resolve,
    /* MaxLines                 */ MaxLines,
    /* MaxHeight                */ MaxHeight,
    /* SetTabs                  */ SetTabs,
    /* GetCursorBounds          */ GetCursorBounds,
    }
};

WidgetClass textSinkObjectClass = (WidgetClass) & textSinkClassRec;

/*
 * Class initialize method:	Initialize widget class:
 * 				Install required converters
 */
static void
ClassInit(void)
{
    /* Arguments for the color converter */

    static XtConvertArgRec colConvertArg[] =
    {
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec, core.screen),
	 sizeof(Screen *)},
	{XtWidgetBaseOffset, (XtPointer) XtOffsetOf(WidgetRec,
	 core.colormap), sizeof(Colormap)}
    };

    XawInitializeWidgetSet();
    XtSetTypeConverter(XtRString, XtRPixel, XtCvtStringToPixel,
		       colConvertArg, XtNumber(colConvertArg),
		       XtCacheByDisplay, NULL);
}

/*
 * Class part initialize method
 */
static void
ClassPartInitialize(WidgetClass wc)
{
    TextSinkObjectClass t_src, superC;

    t_src = (TextSinkObjectClass) wc;
    superC = (TextSinkObjectClass) t_src->object_class.superclass;

/*
 * We don't need to check for null super since we'll get to TextSink
 * eventually.
 */

    if (t_src->text_sink_class.DisplayText == XtInheritDisplayText)
	t_src->text_sink_class.DisplayText = superC->text_sink_class.DisplayText;

    if (t_src->text_sink_class.InsertCursor == XtInheritInsertCursor)
	t_src->text_sink_class.InsertCursor =
	    superC->text_sink_class.InsertCursor;

    if (t_src->text_sink_class.ClearToBackground == XtInheritClearToBackground)
	t_src->text_sink_class.ClearToBackground =
	    superC->text_sink_class.ClearToBackground;

    if (t_src->text_sink_class.FindPosition == XtInheritFindPosition)
	t_src->text_sink_class.FindPosition =
	    superC->text_sink_class.FindPosition;

    if (t_src->text_sink_class.FindDistance == XtInheritFindDistance)
	t_src->text_sink_class.FindDistance =
	    superC->text_sink_class.FindDistance;

    if (t_src->text_sink_class.Resolve == XtInheritResolve)
	t_src->text_sink_class.Resolve = superC->text_sink_class.Resolve;

    if (t_src->text_sink_class.MaxLines == XtInheritMaxLines)
	t_src->text_sink_class.MaxLines = superC->text_sink_class.MaxLines;

    if (t_src->text_sink_class.MaxHeight == XtInheritMaxHeight)
	t_src->text_sink_class.MaxHeight = superC->text_sink_class.MaxHeight;

    if (t_src->text_sink_class.SetTabs == XtInheritSetTabs)
	t_src->text_sink_class.SetTabs = superC->text_sink_class.SetTabs;

    if (t_src->text_sink_class.GetCursorBounds == XtInheritGetCursorBounds)
	t_src->text_sink_class.GetCursorBounds =
	    superC->text_sink_class.GetCursorBounds;
}

/*	Function Name: Initialize
 *	Description: Initializes the TextSink Object.
 *	Arguments: request, new - the requested and new values for the object
 *                                instance.
 *	Returns: none.
 *
 */

static void
Initialize(
	      Widget request GCC_UNUSED,
	      Widget new,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    TextSinkObject sink = (TextSinkObject) new;

    sink->text_sink.tab_count = 0;	/* Initialize the tab stops. */
    sink->text_sink.tabs = NULL;
    sink->text_sink.char_tabs = NULL;
}

/*	Function Name: Destroy
 *	Description: This function cleans up when the object is
 *                   destroyed.
 *	Arguments: w - the TextSink Object.
 *	Returns: none.
 */

static void
Destroy(Widget w)
{
    TextSinkObject sink = (TextSinkObject) w;

    XtFree((char *) sink->text_sink.tabs);
    XtFree((char *) sink->text_sink.char_tabs);
}

/*	Function Name: SetValues
 *	Description: Sets the values for the TextSink
 *	Arguments: current - current state of the object.
 *                 request - what was requested.
 *                 new - what the object will become.
 *	Returns: True if redisplay is needed.
 */

/* ARGSUSED */
static Boolean
SetValues(
	     Widget current,
	     Widget request GCC_UNUSED,
	     Widget new,
	     ArgList args GCC_UNUSED,
	     Cardinal *num_args GCC_UNUSED)
{
    TextSinkObject w = (TextSinkObject) new;
    TextSinkObject old_w = (TextSinkObject) current;

    if (w->text_sink.foreground != old_w->text_sink.foreground)
	((TextWidget) XtParent(new))->text.redisplay_needed = True;

    return FALSE;
}

/************************************************************
 *
 * Class specific methods.
 *
 ************************************************************/

/*	Function Name: DisplayText
 *	Description: Stub function that in subclasses will display text.
 *	Arguments: w - the TextSink Object.
 *                 x, y - location to start drawing text.
 *                 pos1, pos2 - location of starting and ending points
 *                              in the text buffer.
 *                 highlight - hightlight this text?
 *	Returns: none.
 *
 * This function doesn't actually display anything, it is only a place
 * holder.
 */

/* ARGSUSED */
static void
DisplayText(
	       Widget w GCC_UNUSED,
	       int x GCC_UNUSED,
	       int y GCC_UNUSED,
	       XawTextPosition pos1 GCC_UNUSED,
	       XawTextPosition pos2 GCC_UNUSED,
	       Bool highlight GCC_UNUSED)
{
    return;
}

/*	Function Name: InsertCursor
 *	Description: Places the InsertCursor.
 *	Arguments: w - the TextSink Object.
 *                 x, y - location for the cursor.
 *                 staye - whether to turn the cursor on, or off.
 *	Returns: none.
 *
 * This function doesn't actually display anything, it is only a place
 * holder.
 */

/* ARGSUSED */
static void
InsertCursor(
		Widget w GCC_UNUSED,
		int x GCC_UNUSED,
		int y GCC_UNUSED,
		XawTextInsertState state GCC_UNUSED)
{
    return;
}

/*	Function Name: ClearToBackground
 *	Description: Clears a region of the sink to the background color.
 *	Arguments: w - the TextSink Object.
 *                 x, y  - location of area to clear.
 *                 width, height - size of area to clear
 *	Returns: void.
 *
 */

/* ARGSUSED */
static void
ClearToBackground(
		     Widget w,
		     int x,
		     int y,
		     unsigned width,
		     unsigned height)
{
/*
 * Don't clear in height or width are zero.
 * XClearArea() has special semantic for these values.
 */

    if ((height == 0) || (width == 0))
	return;
    XClearArea(XtDisplayOfObject(w), XtWindowOfObject(w),
	       x, y, width, height, False);
}

/*	Function Name: FindPosition
 *	Description: Finds a position in the text.
 *	Arguments: w - the TextSink Object.
 *                 fromPos - reference position.
 *                 fromX   - reference location.
 *                 width,  - width of section to paint text.
 *                 stopAtWordBreak - returned position is a word break?
 *                 resPos - Position to return.      *** RETURNED ***
 *                 resWidth - Width actually used.   *** RETURNED ***
 *                 resHeight - Height actually used. *** RETURNED ***
 *	Returns: none (see above).
 */

/* ARGSUSED */
static void
FindPosition(
		Widget w GCC_UNUSED,
		XawTextPosition fromPos,
		int fromx GCC_UNUSED,
		int width GCC_UNUSED,
		Bool stopAtWordBreak GCC_UNUSED,
		XawTextPosition * resPos,
		int *resWidth,
		int *resHeight)
{
    *resPos = fromPos;
    *resHeight = *resWidth = 0;
}

/*	Function Name: FindDistance
 *	Description: Find the Pixel Distance between two text Positions.
 *	Arguments: w - the TextSink Object.
 *                 fromPos - starting Position.
 *                 fromX   - x location of starting Position.
 *                 toPos   - end Position.
 *                 resWidth - Distance between fromPos and toPos.
 *                 resPos   - Acutal toPos used.
 *                 resHeight - Height required by this text.
 *	Returns: none.
 */

/* ARGSUSED */
static void
FindDistance(
		Widget w GCC_UNUSED,
		XawTextPosition fromPos,
		int fromx GCC_UNUSED,
		XawTextPosition toPos GCC_UNUSED,
		int *resWidth,
		XawTextPosition * resPos,
		int *resHeight)
{
    *resWidth = *resHeight = 0;
    *resPos = fromPos;
}

/*	Function Name: Resolve
 *	Description: Resloves a location to a position.
 *	Arguments: w - the TextSink Object.
 *                 pos - a reference Position.
 *                 fromx - a reference Location.
 *                 width - width to move.
 *                 resPos - the resulting position.
 *	Returns: none
 */

/* ARGSUSED */
static void
Resolve(
	   Widget w GCC_UNUSED,
	   XawTextPosition pos,
	   int fromx GCC_UNUSED,
	   int width GCC_UNUSED,
	   XawTextPosition * resPos)
{
    *resPos = pos;
}

/*	Function Name: MaxLines
 *	Description: Finds the Maximum number of lines that will fit in
 *                   a given height.
 *	Arguments: w - the TextSink Object.
 *                 height - height to fit lines into.
 *	Returns: the number of lines that will fit.
 */

/* ARGSUSED */
static int
MaxLines(
	    Widget w GCC_UNUSED,
	    unsigned height GCC_UNUSED)
{
    /*
     * The fontset has gone down to descent Sink Widget, so
     * the functions such MaxLines, SetTabs... are bound to the descent.
     *
     * by Li Yuhong, Jan. 15, 1991
     */
    return 0;
}

/*	Function Name: MaxHeight
 *	Description: Finds the Minium height that will contain a given number
 *                   lines.
 *	Arguments: w - the TextSink Object.
 *                 lines - the number of lines.
 *	Returns: the height.
 */

/* ARGSUSED */
static int
MaxHeight(
	     Widget w GCC_UNUSED,
	     int lines GCC_UNUSED)
{
    return 0;
}

/*	Function Name: SetTabs
 *	Description: Sets the Tab stops.
 *	Arguments: w - the TextSink Object.
 *                 tab_count - the number of tabs in the list.
 *                 tabs - the text positions of the tabs.
 *	Returns: none
 */

static void
SetTabs(
	   Widget w GCC_UNUSED,
	   int tab_count GCC_UNUSED,
	   short *tabs GCC_UNUSED)
{
    return;
}

/*	Function Name: GetCursorBounds
 *	Description: Finds the bounding box for the insert curor (caret).
 *	Arguments: w - the TextSinkObject.
 *                 rect - an X rectance containing the cursor bounds.
 *	Returns: none (fills in rect).
 */

/* ARGSUSED */
static void
GetCursorBounds(
		   Widget w GCC_UNUSED,
		   XRectangle * rect)
{
    rect->x = rect->y = (short) (rect->width = rect->height = 0);
}

/************************************************************
 *
 * Public Functions.
 *
 ************************************************************/

/*	Function Name: XawTextSinkDisplayText
 *	Description: Stub function that in subclasses will display text.
 *	Arguments: w - the TextSink Object.
 *                 x, y - location to start drawing text.
 *                 pos1, pos2 - location of starting and ending points
 *                              in the text buffer.
 *                 highlight - hightlight this text?
 *	Returns: none.
 *
 * This function doesn't actually display anything, it is only a place
 * holder.
 */

/* ARGSUSED */
void
XawTextSinkDisplayText(Widget w,
		       Position x, Position y,
		       XawTextPosition pos1, XawTextPosition pos2,
		       Boolean highlight)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    (*class->text_sink_class.DisplayText) (w, x, y, pos1, pos2, highlight);
}

/*	Function Name: XawTextSinkInsertCursor
 *	Description: Places the InsertCursor.
 *	Arguments: w - the TextSink Object.
 *                 x, y - location for the cursor.
 *                 staye - whether to turn the cursor on, or off.
 *	Returns: none.
 *
 * This function doesn't actually display anything, it is only a place
 * holder.
 */

/* ARGSUSED */
void
XawTextSinkInsertCursor(Widget w,
			Position x, Position y, XawTextInsertState state)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    (*class->text_sink_class.InsertCursor) (w, x, y, state);
}

/*	Function Name: XawTextSinkClearToBackground
 *	Description: Clears a region of the sink to the background color.
 *	Arguments: w - the TextSink Object.
 *                 x, y  - location of area to clear.
 *                 width, height - size of area to clear
 *	Returns: void.
 *
 * This function doesn't actually display anything, it is only a place
 * holder.
 */

/* ARGSUSED */
void
XawTextSinkClearToBackground(Widget w,
			     Position x, Position y,
			     Dimension width, Dimension height)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    (*class->text_sink_class.ClearToBackground) (w, x, y, width, height);
}

/*	Function Name: XawTextSinkFindPosition
 *	Description: Finds a position in the text.
 *	Arguments: w - the TextSink Object.
 *                 fromPos - reference position.
 *                 fromX   - reference location.
 *                 width,  - width of section to paint text.
 *                 stopAtWordBreak - returned position is a word break?
 *                 resPos - Position to return.      *** RETURNED ***
 *                 resWidth - Width actually used.   *** RETURNED ***
 *                 resHeight - Height actually used. *** RETURNED ***
 *	Returns: none (see above).
 */

/* ARGSUSED */
void
XawTextSinkFindPosition(Widget w, XawTextPosition fromPos, int fromx,
			int width,
			Boolean stopAtWordBreak,
			XawTextPosition * resPos, int *resWidth, int *resHeight)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    (*class->text_sink_class.FindPosition) (w, fromPos, fromx, width,
					    stopAtWordBreak,
					    resPos, resWidth, resHeight);
}

/*	Function Name: XawTextSinkFindDistance
 *	Description: Find the Pixel Distance between two text Positions.
 *	Arguments: w - the TextSink Object.
 *                 fromPos - starting Position.
 *                 fromX   - x location of starting Position.
 *                 toPos   - end Position.
 *                 resWidth - Distance between fromPos and toPos.
 *                 resPos   - Acutal toPos used.
 *                 resHeight - Height required by this text.
 *	Returns: none.
 */

/* ARGSUSED */
void
XawTextSinkFindDistance(Widget w, XawTextPosition fromPos, int fromx,
			XawTextPosition toPos, int *resWidth,
			XawTextPosition * resPos, int *resHeight)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    (*class->text_sink_class.FindDistance) (w, fromPos, fromx, toPos,
					    resWidth, resPos, resHeight);
}

/*	Function Name: XawTextSinkResolve
 *	Description: Resloves a location to a position.
 *	Arguments: w - the TextSink Object.
 *                 pos - a reference Position.
 *                 fromx - a reference Location.
 *                 width - width to move.
 *                 resPos - the resulting position.
 *	Returns: none
 */

/* ARGSUSED */
void
XawTextSinkResolve(Widget w, XawTextPosition pos, int fromx, int width,
		   XawTextPosition * resPos)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    (*class->text_sink_class.Resolve) (w, pos, fromx, width, resPos);
}

/*	Function Name: XawTextSinkMaxLines
 *	Description: Finds the Maximum number of lines that will fit in
 *                   a given height.
 *	Arguments: w - the TextSink Object.
 *                 height - height to fit lines into.
 *	Returns: the number of lines that will fit.
 */

/* ARGSUSED */
int
XawTextSinkMaxLines(Widget w,
		    Dimension height)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    return ((*class->text_sink_class.MaxLines) (w, height));
}

/*	Function Name: XawTextSinkMaxHeight
 *	Description: Finds the Minimum height that will contain a given number
 *                   lines.
 *	Arguments: w - the TextSink Object.
 *                 lines - the number of lines.
 *	Returns: the height.
 */

/* ARGSUSED */
int
XawTextSinkMaxHeight(Widget w, int lines)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    return ((*class->text_sink_class.MaxHeight) (w, lines));
}

/*	Function Name: XawTextSinkSetTabs
 *	Description: Sets the Tab stops.
 *	Arguments: w - the TextSink Object.
 *                 tab_count - the number of tabs in the list.
 *                 tabs - the text positions of the tabs.
 *	Returns: none
 */

void
XawTextSinkSetTabs(Widget w, int tab_count, int *tabs)
{
    if (tab_count > 0) {
	TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;
	short *char_tabs = (short *) XtMalloc((unsigned) tab_count * sizeof(short));
	short *tab;
	int i;

	for (i = tab_count, tab = char_tabs; i; i--)
	    *tab++ = (short) *tabs++;

	(*class->text_sink_class.SetTabs) (w, tab_count, char_tabs);
	XtFree((char *) char_tabs);
    }
}

/*	Function Name: XawTextSinkGetCursorBounds
 *	Description: Finds the bounding box for the insert curor (caret).
 *	Arguments: w - the TextSinkObject.
 *                 rect - an X rectance containing the cursor bounds.
 *	Returns: none (fills in rect).
 */

/* ARGSUSED */
void
XawTextSinkGetCursorBounds(Widget w, XRectangle * rect)
{
    TextSinkObjectClass class = (TextSinkObjectClass) w->core.widget_class;

    (*class->text_sink_class.GetCursorBounds) (w, rect);
}
