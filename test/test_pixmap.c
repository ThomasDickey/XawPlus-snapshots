/*
 * $XTermId: test_pixmap.c,v 1.2 2025/01/19 14:12:49 tom Exp $
 */

/* Test the use of a pixmap and a clip mask.
 * (c) Roland Krause, 2000
 */

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/Label.h>
#include <X11/xpm.h>

#include "exit.xpm"

int main(int argc, char **argv)
{
        XtAppContext	ac;
        Widget		w, topLevel;
	Pixmap		exitPix, exitClip;
	Window		win;
	Display		*disp;

	XtSetLanguageProc(NULL, NULL, NULL);
        topLevel = XtAppInitialize(&ac, "Hello", NULL, 0, &argc, argv, NULL, NULL, 0);

	/* Important: After a XtAppInitialize() it is possible to determine the display
	 * but not the window. To get a widgets window it has to be realized, otherwise
	 * there is no window defined. This is the reason I take the root window here.
	 * The root window always exists if there is a display.
	 */
	disp = XtDisplay(topLevel);
	win  = RootWindow(disp, XDefaultScreen(disp));
        (void)XpmCreatePixmapFromData(disp, win, exit_xpm, &exitPix, &exitClip, NULL);

        w = XtVaCreateManagedWidget("hello", labelWidgetClass, topLevel,
		XtNbitmap, exitPix, XtNclipMask, exitClip, NULL);

        XtRealizeWidget(topLevel);
        XtAppMainLoop(ac);
        return 0;
}

