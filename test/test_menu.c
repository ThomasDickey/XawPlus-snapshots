/* Test the menu in the 2 byte mode.
 * (c) Roland Krause, 2002
 */

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/XawPlus/Label.h>
#include <X11/XawPlus/SimpleMenu.h>	/* The menu */
#include <X11/XawPlus/SmeBSB.h>		/* For menu entries */


/* --- Fallback resources -------------------------------- */

char *fallback[] =
{
  "*encoding:		1",
  "*font: 		-*-fixed-*-*-*-*-20-*-*-*-*-*-iso10646-1",

  "*SmeBSB.leftMargin:	20",
  "*SmeBSB.rightMargin:	40",
  "*SmeBSB.rightBitmap:		./exit.xpm",
  "*SmeBSB.rightClipMask:	./exit.xpm",
  "*SmeBSB.leftBitmap:		./mini-question.xpm",
  "*SmeBSB.leftClipMask:	./mini-question.xpm",

  "*entry1.label:	① First entry",
  "*entry2.label:	② Second entry",
  "*entry3.label:	③ Third entry",
  "*entry4.label:	④ Forth entry",

  "*label.label:	Press the right mouse key!",
  "*label.height:	80",
  "*label.translations:	#override <Btn3Down>: "
			"XawPositionSimpleMenu(popupMenu) "
			"MenuPopup(popupMenu)",
  NULL
};


/* --- Main() -------------------------------------------- */

int main(int argc, char *argv[])
{
        XtAppContext		app;
        Widget			topLevel, menu, label;

	XtSetLanguageProc(NULL, NULL, NULL);
        topLevel = XtAppInitialize(&app, "MenuTest", NULL, 0,
				   &argc, argv, fallback, NULL, 0);

	/* Create the label and add an action to popup a menu */

	label = XtVaCreateManagedWidget("label", labelWidgetClass, topLevel, NULL);

	/* Create the menu and the menu entries */

	menu = XtCreatePopupShell("popupMenu", simpleMenuWidgetClass, topLevel, NULL, 0);
	(void) XtCreateManagedWidget("entry1", smeBSBObjectClass, menu, NULL, 0);
	(void) XtCreateManagedWidget("entry2", smeBSBObjectClass, menu, NULL, 0);
	(void) XtCreateManagedWidget("entry3", smeBSBObjectClass, menu, NULL, 0);
	(void) XtCreateManagedWidget("entry4", smeBSBObjectClass, menu, NULL, 0);

        XtRealizeWidget(topLevel);
        XtAppMainLoop(app);
        return 0;
}

