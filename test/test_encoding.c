/* Test the label in the 2 byte mode.
 * (c) Roland Krause, 2000
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/XawPlus/Label.h>

char *MbString =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
"abcdefghijklmnopqrstuvwxyz\n"
"0123456789\n"
"ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ\n"
"αβγδεζηθικλμνξοπρστυφχψω\n"
"←↖↑↗→↘↓↙←⇐⇖⇑⇗⇒⇘⇓⇙⇐";

int main(int argc, char **argv)
{
        XtAppContext	ac;
        Widget		w, topLevel;

	XtSetLanguageProc(NULL, NULL, NULL);
        topLevel = XtAppInitialize(&ac, "Hello", NULL, 0, &argc, argv, NULL, NULL, 0);

        w = XtVaCreateManagedWidget("hello", labelWidgetClass, topLevel,
		XtNfont, XLoadQueryFont(XtDisplay(topLevel), "-*-fixed-*-*-*-*-20-*-*-*-*-*-iso10646-1"),
		XtNencoding, XawTextEncodingChar2b, XtNlabel, MbString, NULL);

        XtRealizeWidget(topLevel);
        XtAppMainLoop(ac);
        return 0;
}

