/*
 * $XTermId: test_scrbar.c,v 1.2 2025/01/19 14:12:49 tom Exp $
 */

/* Scrollbar test
 * (c) Roland Krause, 2000
 */

#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/Box.h>
#include <X11/XawPlus/Label.h>
#include <X11/XawPlus/Scrollbar.h>

int  WidgetWidth = 300;

/* --- JumpProc() --------------------------------------------------------- */

/* Handle slider move events.
 */
void JumpProc(Widget w, XtPointer client_data, XtPointer percent)
{
	char str[20];

	sprintf(str, "%f", *(float *)percent);
	XtVaSetValues((Widget)client_data, XtNlabel, str, XtNwidth, WidgetWidth, NULL);
}

/* --- ScrollProc() ------------------------------------------------------- */

/* Handle events that are triggered through the arrow keys at the ends
 * of the scrollbar or through clicking in the left space between the
 * slider and the arrow keys.
 */
void ScrollProc(Widget w, XtPointer client_data, XtPointer position)
{
	float	curpos, newpos, width, pos;
	int	ipos = (int)position;
	int	max  = WidgetWidth >> 2;
	char	str[20];

	/* Set the jump width of the scrollbar
	 */
	width = WidgetWidth;
	if (abs(ipos) > max)
	{
	   if (ipos < 0) pos = -max;
	   else pos = max;
	}
	else pos = ipos;

	/* Get the current position of the scrollbar
	 * and calculate the new position.
	 */
	XtVaGetValues(w, XtNtopOfThumb, &curpos, NULL);
	newpos = (width * curpos + pos) / width;
	if (newpos < 0.0) newpos = 0.0;
	if (newpos > 1.0) newpos = 1.0;

	/* Set the slider position and put the position value
	 * into the label.
	 */
	XawScrollbarSetThumb(w, newpos, -1.0);
/*	XtVaSetValues(w, XtNtopOfThumb, &newpos, NULL); */

	sprintf(str, "%f", newpos);
	XtVaSetValues((Widget)client_data, XtNlabel, str,  XtNwidth, WidgetWidth, NULL);
}

/* --- main() --------------------------------------------------------------- */

int main(int argc, char *argv[])
{
        XtAppContext	ac;
        Widget		lw, sw, bw, topLevel;

	XtSetLanguageProc(NULL, NULL, NULL);
        topLevel = XtAppInitialize(&ac, "TestScrollbar", NULL, 0, &argc, argv, NULL, NULL, 0);

	bw = XtVaCreateManagedWidget("box", boxWidgetClass, topLevel,
		XtNorientation, XtorientVertical, NULL);

        lw = XtVaCreateManagedWidget("value", labelWidgetClass, bw,
		XtNlabel, "0.500000", XtNwidth, WidgetWidth, XtNjustify, XtJustifyCenter, NULL);

        sw = XtVaCreateManagedWidget("slider", scrollbarWidgetClass, bw,
		XtNwidth, WidgetWidth, XtNorientation, XtorientHorizontal, NULL);

	XtAddCallback(sw, XtNjumpProc, JumpProc, (XtPointer)lw);
	XtAddCallback(sw, XtNscrollProc, ScrollProc, (XtPointer)lw);

	XawScrollbarSetThumb(sw, 0.5, -1.0);

        XtRealizeWidget(topLevel);
        XtAppMainLoop(ac);
        return 0;
}


