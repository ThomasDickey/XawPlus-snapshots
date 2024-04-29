/*
 * $XTermId: MenuButton.c,v 1.7 2024/04/28 23:57:51 tom Exp $
 * $Xorg: MenuButton.c,v 1.4 2001/02/09 02:03:44 xorgcvs Exp $
 */

/*
Copyright 2022,2024  Thomas E. Dickey
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

/***********************************************************************
 *
 * MenuButton Widget
 *
 ***********************************************************************/

/*
 * MenuButton.c - Source code for MenuButton widget.
 *
 * This is the source code for the Athena MenuButton widget.
 * It is intended to provide an easy method of activating pulldown menus.
 *
 * Date:    May 2, 1989
 *
 * By:      Chris D. Peterson
 *          MIT X Consortium
 *          kit@expo.lcs.mit.edu
 */

#include "private.h"

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <X11/XawPlus/XawInit.h>
#include <X11/XawPlus/MenuButtoP.h>
#include <X11/XawPlus/SimpleMenP.h>
/* *INDENT-OFF* */
static void ClassInitialize(void);
static void PopupMenu(Widget w, XEvent * event, String * params, Cardinal * num_params);

#define superclass ((CommandWidgetClass)&commandClassRec)

static char defaultTranslations[] =
    "<EnterWindow>:	highlight()\n\
     <LeaveWindow>:	reset()\n\
     <BtnDown>:		set() PopupMenu()";

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

#define offset(field) XtOffsetOf(MenuButtonRec, field)
static XtResource resources[] = {
  {
    XtNmenuName, XtCMenuName, XtRString, sizeof(String),
    offset(menu_button.menu_name), XtRString, (XtPointer)"menu"},
};
#undef offset

static XtActionsRec actionsList[] =
{
  {"PopupMenu",	PopupMenu}
};

MenuButtonClassRec menuButtonClassRec = {
  {
    (WidgetClass) superclass,		/* superclass		  */
    "MenuButton",			/* class_name		  */
    sizeof(MenuButtonRec),       	/* size			  */
    ClassInitialize,			/* class_initialize	  */
    NULL,				/* class_part_initialize  */
    FALSE,				/* class_inited		  */
    NULL,				/* initialize		  */
    NULL,				/* initialize_hook	  */
    XtInheritRealize,			/* realize		  */
    actionsList,			/* actions		  */
    XtNumber(actionsList),		/* num_actions		  */
    resources,				/* resources		  */
    XtNumber(resources),		/* resource_count	  */
    NULLQUARK,				/* xrm_class		  */
    FALSE,				/* compress_motion	  */
    XtExposeCompressSeries,		/* compress_exposure	  */
    TRUE,				/* compress_enterleave    */
    FALSE,				/* visible_interest	  */
    NULL,				/* destroy		  */
    XtInheritResize,			/* resize		  */
    XtInheritExpose,			/* expose		  */
    NULL,				/* set_values		  */
    NULL,				/* set_values_hook	  */
    XtInheritSetValuesAlmost,		/* set_values_almost	  */
    NULL,				/* get_values_hook	  */
    NULL,				/* accept_focus		  */
    XtVersion,				/* version		  */
    NULL,				/* callback_private	  */
    defaultTranslations,               	/* tm_table		  */
    XtInheritQueryGeometry,		/* query_geometry	  */
    XtInheritDisplayAccelerator,	/* display_accelerator	  */
    NULL				/* extension		  */
  },  /* CoreClass fields initialization */
  {
    XtInheritChangeSensitive		/* change_sensitive	  */
  },  /* SimpleClass fields initialization */
  {
    0					/* field not used 	  */
  },  /* LabelClass fields initialization */
  {
    0					/* field not used   	  */
  },  /* CommandClass fields initialization */
  {
    0					/* field not used   	  */
  },  /* MenuButtonClass fields initialization */
};
/* *INDENT-ON* */

  /* for public consumption */
WidgetClass menuButtonWidgetClass = (WidgetClass) & menuButtonClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void
ClassInitialize(void)
{
    XawInitializeWidgetSet();
    XtRegisterGrabAction(PopupMenu, True, ButtonPressMask | ButtonReleaseMask,
			 GrabModeAsync, GrabModeAsync);
}

static void
PopupMenu(
	     Widget w,
	     XEvent *event GCC_UNUSED,
	     String *params GCC_UNUSED,
	     Cardinal *num_params GCC_UNUSED)
{
    MenuButtonWidget mbw = (MenuButtonWidget) w;
    SimpleMenuWidget simpleMenu;
    Widget menu, temp;
    Arg arglist[2];
    Cardinal num_args;
    int menu_x, menu_y, menu_width, menu_height, button_height;
    Position button_x, button_y;

    temp = w;
    while (temp != NULL) {
	menu = XtNameToWidget(temp, mbw->menu_button.menu_name);
	if (menu == NULL)
	    temp = XtParent(temp);
	else
	    break;
    }

    if (menu == NULL) {
	char error_buf[BUFSIZ];
	char *errorp;
	int len;
	char *fmt = "MenuButton: Could not find menu widget named %s.";

	if ((size_t) (len = (int) (strlen(fmt) +
	    strlen(mbw->menu_button.menu_name))) < sizeof error_buf)
	    errorp = error_buf;
	else
	    errorp = XtMalloc((Cardinal) (len + 1));
	if (errorp == NULL) {
	    errorp = error_buf;
	    strcpy(errorp, "MenuButton: Could not find menu widget.");
	} else
	    (void) sprintf(errorp, fmt, mbw->menu_button.menu_name);
	XtAppWarning(XtWidgetToApplicationContext(w), errorp);
	if (errorp != error_buf)
	    XtFree(errorp);
	return;
    }
    if (!XtIsRealized(menu))
	XtRealizeWidget(menu);

    simpleMenu = (SimpleMenuWidget) menu;
    button_height = w->core.height + 2 * w->core.border_width;
    menu_width = simpleMenu->core.width + 2 * simpleMenu->core.border_width +
	2 * simpleMenu->simple_menu.borderWidth;
    menu_height = simpleMenu->core.height + 2 *
	simpleMenu->core.border_width +
	2 * simpleMenu->simple_menu.borderWidth;

    XtTranslateCoords(w, 0, 0, &button_x, &button_y);
    menu_x = button_x;
    menu_y = button_y + button_height;

    if (menu_x >= 0) {
	int scr_width = WidthOfScreen(XtScreen(menu));
	if (menu_x + menu_width > scr_width)
	    menu_x = scr_width - menu_width;
    }
    if (menu_x < 0)
	menu_x = 0;

    if (menu_y >= 0) {
	int scr_height = HeightOfScreen(XtScreen(menu));
	if (menu_y + menu_height > scr_height)
	    menu_y = scr_height - menu_height;
    }
    if (menu_y < 0)
	menu_y = 0;

    num_args = 0;
    XtSetArg(arglist[num_args], XtNx, menu_x);
    num_args++;
    XtSetArg(arglist[num_args], XtNy, menu_y);
    num_args++;
    XtSetValues(menu, arglist, num_args);

    /* Popup the menu */

    XtPopupSpringLoaded(menu);
}
