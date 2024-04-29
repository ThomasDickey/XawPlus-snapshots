/*
 * $XTermId: Panner.c,v 1.6 2024/04/28 23:55:55 tom Exp $
 * $Xorg: Panner.c,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 *
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
 *
 * Author:  Jim Fulton, MIT X Consortium
 *
 * This file contains modifications for XawPlus, Roland Krause 2002
 */

#include "private.h"

#include <X11/IntrinsicP.h>	/* for toolkit routines */
#include <X11/StringDefs.h>	/* for XtN and XtC defines */
#include <X11/Xmu/CharSet.h>	/* for XmuCompareISOLatin1() */
#include <X11/XawPlus/XawInit.h>	/* for XawInitializeWidgetSet */
#include <X11/XawPlus/PannerP.h>	/* us */
#include <X11/Xmu/Misc.h>	/* for Min */
#include <X11/Xmu/StdCmap.h>	/* for XmuDistinguishablePixels */

#include <ctype.h>		/* for isascii() etc. */
/* *INDENT-OFF* */
static char defaultTranslations[] =
  "<Btn1Down>:    start() \n\
   <Btn1Motion>:  move() \n\
   <Btn1Up>:      notify() stop() \n\
   <Btn2Down>:    abort() \n\
   <Key>space:    page(+1p,+1p) \n\
   <Key>Delete:   page(-1p,-1p) \n\
   <Key>BackSpace:  page(-1p,-1p) \n\
   <Key>Left:     page(-.5p,+0) \n\
   <Key>Right:    page(+.5p,+0) \n\
   <Key>Up:       page(+0,-.5p) \n\
   <Key>Down:     page(+0,+.5p) \n\
   <Key>Home:     page(0,0) ";


static void ActionStart(Widget gw, XEvent *event, String *params, Cardinal *num_params);
static void ActionStop(Widget gw, XEvent *event, String *params, Cardinal *num_params);
static void ActionAbort(Widget gw, XEvent *event, String *params, Cardinal *num_params);
static void ActionMove(Widget gw, XEvent *event, String *params, Cardinal *num_params);
static void ActionPage(Widget gw, XEvent *event, String *params, Cardinal *num_params);
static void ActionNotify(Widget gw, XEvent *event, String *params, Cardinal *num_params);

static XtActionsRec actions[] = {
    { "start", ActionStart },		/* start tmp graphics */
    { "stop", ActionStop },		/* stop tmp graphics */
    { "abort", ActionAbort },		/* punt */
    { "move", ActionMove },		/* move tmp graphics on Motion event */
    { "page", ActionPage },		/* page around usually from keyboard */
    { "notify", ActionNotify }		/* callback new position */
};

/*
 * resources for the panner
 */
static XtResource resources[] = {
#define poff(field) XtOffsetOf(PannerRec, panner.field)
    { XtNallowOff, XtCAllowOff, XtRBoolean, sizeof(Boolean),
	poff(allow_off), XtRImmediate, (XtPointer) FALSE },
    { XtNresize, XtCResize, XtRBoolean, sizeof(Boolean),
	poff(resize_to_pref), XtRImmediate, (XtPointer) TRUE },
    { XtNreportCallback, XtCReportCallback, XtRCallback, sizeof(XtPointer),
	poff(report_callbacks), XtRCallback, (XtPointer) NULL },
    { XtNdefaultScale, XtCDefaultScale, XtRDimension, sizeof(Dimension),
	poff(default_scale), XtRImmediate, (XtPointer) PANNER_DEFAULT_SCALE },
    { XtNinternalSpace, XtCInternalSpace, XtRDimension, sizeof(Dimension),
	poff(internal_border), XtRImmediate, (XtPointer) 4 },
    { XtNcanvasWidth, XtCCanvasWidth, XtRDimension, sizeof(Dimension),
	poff(canvas_width), XtRImmediate, (XtPointer) 0 },
    { XtNcanvasHeight, XtCCanvasHeight, XtRDimension, sizeof(Dimension),
	poff(canvas_height), XtRImmediate, (XtPointer) 0 },
    { XtNsliderX, XtCSliderX, XtRPosition, sizeof(Position),
	poff(slider_x), XtRImmediate, (XtPointer) 0 },
    { XtNsliderY, XtCSliderY, XtRPosition, sizeof(Position),
	poff(slider_y), XtRImmediate, (XtPointer) 0 },
    { XtNsliderWidth, XtCSliderWidth, XtRDimension, sizeof(Dimension),
	poff(slider_width), XtRImmediate, (XtPointer) 0 },
    { XtNsliderHeight, XtCSliderHeight, XtRDimension, sizeof(Dimension),
	poff(slider_height), XtRImmediate, (XtPointer) 0 },
#undef poff
};


/*
 * widget class methods used below
 */
static void Initialize(Widget greq, Widget gnew, ArgList args, Cardinal *num_args);
static void Realize(Widget gw, XtValueMask *valuemaskp, XSetWindowAttributes *attr);
static void Destroy(Widget gw);
static void Resize(Widget gw);
static void Redisplay(Widget gw, XEvent *event, Region region);
static Boolean SetValues(Widget gcur, Widget greq, Widget gnew, ArgList args, Cardinal *num_args);
static void SetValuesAlmost(Widget gold, Widget gnew, XtWidgetGeometry *req, XtWidgetGeometry *reply);
static XtGeometryResult QueryGeometry(Widget gw, XtWidgetGeometry *intended, XtWidgetGeometry *pref);

PannerClassRec pannerClassRec = {
  { /* core fields */
    /* superclass		*/	(WidgetClass) &simpleClassRec,
    /* class_name		*/	"Panner",
    /* widget_size		*/	sizeof(PannerRec),
    /* class_initialize		*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	actions,
    /* num_actions		*/	XtNumber(actions),
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	XtExposeCompressSeries,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	SetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	defaultTranslations,
    /* query_geometry		*/	QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* simple fields */
    /* change_sensitive		*/	XtInheritChangeSensitive
  },
  { /* panner fields */
    /* ignore                   */	0
  }
};
/* *INDENT-ON* */

WidgetClass pannerWidgetClass = (WidgetClass) & pannerClassRec;

/*****************************************************************************
 *                                                                           *
 *			    panner utility routines                          *
 *                                                                           *
 *****************************************************************************/

static void
check_knob(
	      PannerWidget pw,
	      Boolean knob)
{
    Position pad = (Position) (pw->panner.internal_border * 2);
    Position maxx = (Position) (((Position) pw->core.width) - pad -
				((Position) pw->panner.knob_width));
    Position maxy = (Position) (((Position) pw->core.height) - pad -
				((Position) pw->panner.knob_height));
    Position *x = (knob ? &pw->panner.knob_x : &pw->panner.tmp.x);
    Position *y = (knob ? &pw->panner.knob_y : &pw->panner.tmp.y);

    /*
     * note that positions are already normalized (i.e. internal_border
     * has been subtracted out)
     */
    if (*x < 0)
	*x = 0;
    if (*x > maxx)
	*x = maxx;

    if (*y < 0)
	*y = 0;
    if (*y > maxy)
	*y = maxy;

    if (knob) {
	pw->panner.slider_x = (Position) (((double) pw->panner.knob_x) /
					  pw->panner.haspect + 0.5);
	pw->panner.slider_y = (Position) (((double) pw->panner.knob_y) /
					  pw->panner.vaspect + 0.5);
	pw->panner.last_x = pw->panner.last_y = PANNER_OUTOFRANGE;
    }
}

/* set knob size and/or loc */
static void
scale_knob(
	      PannerWidget pw,
	      Boolean location,
	      Boolean size)
{
    if (location) {
	pw->panner.knob_x = (Position) PANNER_HSCALE(pw, pw->panner.slider_x);
	pw->panner.knob_y = (Position) PANNER_VSCALE(pw, pw->panner.slider_y);
    }
    if (size) {
	Dimension width, height;

	if (pw->panner.slider_width < 1) {
	    pw->panner.slider_width = pw->panner.canvas_width;
	}
	if (pw->panner.slider_height < 1) {
	    pw->panner.slider_height = pw->panner.canvas_height;
	}
	width = Min(pw->panner.slider_width, pw->panner.canvas_width);
	height = Min(pw->panner.slider_height, pw->panner.canvas_height);

	pw->panner.knob_width = (Dimension) PANNER_HSCALE(pw, width);
	pw->panner.knob_height = (Dimension) PANNER_VSCALE(pw, height);
    }
    if (!pw->panner.allow_off)
	check_knob(pw, TRUE);
}

static void
rescale(PannerWidget pw)
{
    int hpad = pw->panner.internal_border * 2;
    int vpad = hpad;

    if (pw->panner.canvas_width < 1)
	pw->panner.canvas_width = pw->core.width;
    if (pw->panner.canvas_height < 1)
	pw->panner.canvas_height = pw->core.height;

    if ((int) pw->core.width <= hpad)
	hpad = 0;
    if ((int) pw->core.height <= vpad)
	vpad = 0;

    pw->panner.haspect = ((double) pw->core.width - hpad) /
	(double) pw->panner.canvas_width;
    pw->panner.vaspect = ((double) pw->core.height - vpad) /
	(double) pw->panner.canvas_height;
    scale_knob(pw, TRUE, TRUE);
}

static void
get_default_size(
		    PannerWidget pw,
		    Dimension *wp,
		    Dimension *hp)
{
    Dimension pad = pw->panner.internal_border * 2;
    *wp = PANNER_DSCALE(pw, pw->panner.canvas_width) + pad;
    *hp = PANNER_DSCALE(pw, pw->panner.canvas_height) + pad;
}

static Boolean
get_event_xy(
		PannerWidget pw,
		XEvent *event,
		int *x,
		int *y)
{
    int pad = pw->panner.internal_border;

    switch (event->type) {
    case ButtonPress:
    case ButtonRelease:
	*x = event->xbutton.x - pad;
	*y = event->xbutton.y - pad;
	return TRUE;

    case KeyPress:
    case KeyRelease:
	*x = event->xkey.x - pad;
	*y = event->xkey.y - pad;
	return TRUE;

    case EnterNotify:
    case LeaveNotify:
	*x = event->xcrossing.x - pad;
	*y = event->xcrossing.y - pad;
	return TRUE;

    case MotionNotify:
	*x = event->xmotion.x - pad;
	*y = event->xmotion.y - pad;
	return TRUE;
    }

    return FALSE;
}

static int
parse_page_string(
		     String s,
		     int pagesize,
		     int canvassize,
		     Boolean *relative)
{
    String cp;
    double val = 1.0;
    Boolean rel = FALSE;

    /*
     * syntax:    spaces [+-] number spaces [pc\0] spaces
     */

    for (; isascii(*s) && isspace(*s); s++) ;	/* skip white space */

    if (*s == '+' || *s == '-') {	/* deal with signs */
	rel = TRUE;
	if (*s == '-')
	    val = -1.0;
	s++;
    }
    if (!*s) {			/* if null then return nothing */
	*relative = TRUE;
	return 0;
    }

    /* skip over numbers */
    for (cp = s; isascii(*s) && (isdigit(*s) || *s == '.'); s++) ;
    val *= atof(cp);

    /* skip blanks */
    for (; isascii(*s) && isspace(*s); s++) ;

    if (*s) {			/* if units */
	switch (s[0]) {
	case 'p':
	case 'P':
	    val *= (double) pagesize;
	    break;

	case 'c':
	case 'C':
	    val *= (double) canvassize;
	    break;
	}
    }
    *relative = rel;
    return ((int) val);
}

/*****************************************************************************
 *                                                                           *
 * 			     panner class methods                            *
 *                                                                           *
 *****************************************************************************/

/* create gc's */
static void
Initialize(
	      Widget greq,
	      Widget gnew,
	      ArgList args GCC_UNUSED,
	      Cardinal *num_args GCC_UNUSED)
{
    PannerWidget req = (PannerWidget) greq, new = (PannerWidget) gnew;
    Dimension defwidth, defheight;
    XGCValues values;

    if (req->panner.canvas_width < 1)
	new->panner.canvas_width = 1;
    if (req->panner.canvas_height < 1)
	new->panner.canvas_height = 1;
    if (req->panner.default_scale < 1)
	new->panner.default_scale = PANNER_DEFAULT_SCALE;

    get_default_size(req, &defwidth, &defheight);
    if (req->core.width < 1)
	new->core.width = defwidth;
    if (req->core.height < 1)
	new->core.height = defheight;

    rescale(new);		/* does a position check */
    new->panner.tmp.doing = FALSE;
    values.foreground = new->core.background_pixel;
    new->panner.gc = XtGetGC(gnew, GCForeground, &values);
}

/* create window */
static void
Realize(
	   Widget gw,
	   XtValueMask * valuemaskp,
	   XSetWindowAttributes * attr)
{
    (*pannerWidgetClass->core_class.superclass->core_class.realize) (gw,
								     valuemaskp, attr);
}

/* clean up widget */
static void
Destroy(Widget gw)
{
    PannerWidget pw = (PannerWidget) gw;

    XtReleaseGC(gw, pw->panner.gc);
}

static void
Resize(Widget gw)
{
    rescale((PannerWidget) gw);
}

static void
Redisplay(
	     Widget gw,
	     XEvent *event GCC_UNUSED,
	     Region region GCC_UNUSED)
{
    PannerWidget pw = (PannerWidget) gw;
    int pad = pw->panner.internal_border;
    int kx = pw->panner.knob_x + pad, ky = pw->panner.knob_y + pad;

    pw->panner.last_x = pw->panner.knob_x;
    pw->panner.last_y = pw->panner.knob_y;
    XFillRectangle(XtDisplay(gw), pw->core.window, pw->panner.gc,
		   0, 0, pw->core.width, pw->core.height);
    XawRaisedRectangle(gw, kx, ky, pw->panner.knob_width - 1,
		       pw->panner.knob_height - 1);
}

/* ARGSUSED */
static Boolean
SetValues(
	     Widget gcur,
	     Widget greq GCC_UNUSED,
	     Widget gnew,
	     ArgList args GCC_UNUSED,
	     Cardinal *num_args GCC_UNUSED)
{
    PannerWidget cur = (PannerWidget) gcur;
    PannerWidget new = (PannerWidget) gnew;
    Boolean redisplay = FALSE;

    if (new->panner.resize_to_pref &&
	(cur->panner.canvas_width != new->panner.canvas_width ||
	 cur->panner.canvas_height != new->panner.canvas_height ||
	 cur->panner.resize_to_pref != new->panner.resize_to_pref)) {
	get_default_size(new, &new->core.width, &new->core.height);
	redisplay = TRUE;
    } else if (cur->panner.canvas_width != new->panner.canvas_width ||
	       cur->panner.canvas_height != new->panner.canvas_height ||
	       cur->panner.internal_border != new->panner.internal_border) {
	rescale(new);		/* does a scale_knob as well */
	redisplay = TRUE;
    } else {
	Boolean loc = (cur->panner.slider_x != new->panner.slider_x ||
		       cur->panner.slider_y != new->panner.slider_y);
	Boolean siz = (cur->panner.slider_width != new->panner.slider_width ||
		       cur->panner.slider_height != new->panner.slider_height);
	if (loc || siz ||
	    (cur->panner.allow_off != new->panner.allow_off &&
	     new->panner.allow_off)) {
	    scale_knob(new, loc, siz);
	    redisplay = TRUE;
	}
    }

    return redisplay;
}

static void
SetValuesAlmost(
		   Widget gold,
		   Widget gnew,
		   XtWidgetGeometry * req,
		   XtWidgetGeometry * reply)
{
    if (reply->request_mode == 0) {	/* got turned down, so cope */
	Resize(gnew);
    }
    (*pannerWidgetClass->core_class.superclass->core_class.set_values_almost)
	(gold, gnew, req, reply);
}

static XtGeometryResult
QueryGeometry(
		 Widget gw,
		 XtWidgetGeometry * intended,
		 XtWidgetGeometry * pref)
{
    PannerWidget pw = (PannerWidget) gw;

    pref->request_mode = (CWWidth | CWHeight);
    get_default_size(pw, &pref->width, &pref->height);

    if (((intended->request_mode & (CWWidth | CWHeight)) ==
	 (CWWidth | CWHeight)) &&
	intended->width == pref->width &&
	intended->height == pref->height)
	return XtGeometryYes;
    else if (pref->width == pw->core.width && pref->height == pw->core.height)
	return XtGeometryNo;
    else
	return XtGeometryAlmost;
}

/*****************************************************************************
 *                                                                           *
 * 			      panner action procs                            *
 *                                                                           *
 *****************************************************************************/

/* ARGSUSED */
static void
ActionStart(
	       Widget gw,
	       XEvent *event,
	       String *params GCC_UNUSED,
	       Cardinal *num_params GCC_UNUSED)
{
    PannerWidget pw = (PannerWidget) gw;
    int x, y;

    if (!get_event_xy(pw, event, &x, &y)) {
	XBell(XtDisplay(gw), 0);	/* should do error message */
	return;
    }

    pw->panner.tmp.doing = TRUE;
    pw->panner.tmp.startx = pw->panner.knob_x;
    pw->panner.tmp.starty = pw->panner.knob_y;
    pw->panner.tmp.dx = (((Position) x) - pw->panner.knob_x);
    pw->panner.tmp.dy = (((Position) y) - pw->panner.knob_y);
    pw->panner.tmp.x = pw->panner.knob_x;
    pw->panner.tmp.y = pw->panner.knob_y;
}

/* ARGSUSED */
static void
ActionStop(
	      Widget gw,
	      XEvent *event,
	      String *params GCC_UNUSED,
	      Cardinal *num_params GCC_UNUSED)
{
    PannerWidget pw = (PannerWidget) gw;
    int x, y;

    if (get_event_xy(pw, event, &x, &y)) {
	pw->panner.tmp.x = ((Position) x) - pw->panner.tmp.dx;
	pw->panner.tmp.y = ((Position) y) - pw->panner.tmp.dy;
	if (!pw->panner.allow_off)
	    check_knob(pw, FALSE);
    }
    pw->panner.tmp.doing = FALSE;
}

/* ARGSUSED */
static void
ActionAbort(
	       Widget gw,
	       XEvent *event,
	       String *params,
	       Cardinal *num_params)
{
    PannerWidget pw = (PannerWidget) gw;

    if (!pw->panner.tmp.doing)
	return;

    /* restore old position */

    pw->panner.tmp.x = pw->panner.tmp.startx;
    pw->panner.tmp.y = pw->panner.tmp.starty;
    ActionNotify(gw, event, params, num_params);

    pw->panner.tmp.doing = FALSE;
}

static void
ActionMove(
	      Widget gw,
	      XEvent *event,	/* must be a motion event */
	      String *params,
	      Cardinal *num_params)
{
    PannerWidget pw = (PannerWidget) gw;
    int x, y;

    if (!pw->panner.tmp.doing)
	return;

    if (!get_event_xy(pw, event, &x, &y)) {
	XBell(XtDisplay(gw), 0);	/* should do error message */
	return;
    }

    pw->panner.tmp.x = ((Position) x) - pw->panner.tmp.dx;
    pw->panner.tmp.y = ((Position) y) - pw->panner.tmp.dy;

    ActionNotify(gw, event, params, num_params);	/* does a check */
}

static void
ActionPage(
	      Widget gw,
	      XEvent *event,
	      String *params,
	      Cardinal *num_params)
{
    PannerWidget pw = (PannerWidget) gw;
    Cardinal zero = 0;
    Boolean isin = pw->panner.tmp.doing;
    int x, y;
    Boolean relx, rely;
    int pad = pw->panner.internal_border * 2;

    if (*num_params != 2) {
	XBell(XtDisplay(gw), 0);
	return;
    }

    x = parse_page_string(params[0], (int) pw->panner.knob_width,
			  ((int) pw->core.width) - pad, &relx);
    y = parse_page_string(params[1], (int) pw->panner.knob_height,
			  ((int) pw->core.height) - pad, &rely);

    if (relx)
	x += pw->panner.knob_x;
    if (rely)
	y += pw->panner.knob_y;

    if (isin) {			/* if in, then use move */
	XEvent ev;
	ev.xbutton.type = ButtonPress;
	ev.xbutton.x = x;
	ev.xbutton.y = y;
	ActionMove(gw, &ev, (String *) NULL, &zero);
    } else {			/* else just do it */
	pw->panner.tmp.doing = TRUE;
	pw->panner.tmp.x = (Position) x;
	pw->panner.tmp.y = (Position) y;
	ActionNotify(gw, event, (String *) NULL, &zero);
	pw->panner.tmp.doing = FALSE;
    }
}

static void
ActionNotify(
		Widget gw,
		XEvent *event GCC_UNUSED,
		String *params GCC_UNUSED,
		Cardinal *num_params GCC_UNUSED)
{
    PannerWidget pw = (PannerWidget) gw;

    if (!pw->panner.tmp.doing)
	return;

    if (!pw->panner.allow_off)
	check_knob(pw, FALSE);
    pw->panner.knob_x = pw->panner.tmp.x;
    pw->panner.knob_y = pw->panner.tmp.y;

    pw->panner.slider_x = (Position) (((double) pw->panner.knob_x) /
				      pw->panner.haspect + 0.5);
    pw->panner.slider_y = (Position) (((double) pw->panner.knob_y) /
				      pw->panner.vaspect + 0.5);
    if (!pw->panner.allow_off) {
	Position tmp;

	if (pw->panner.slider_x >
	    (tmp = (((Position) pw->panner.canvas_width) -
		    ((Position) pw->panner.slider_width))))
	    pw->panner.slider_x = tmp;
	if (pw->panner.slider_x < 0)
	    pw->panner.slider_x = 0;
	if (pw->panner.slider_y >
	    (tmp = (((Position) pw->panner.canvas_height) -
		    ((Position) pw->panner.slider_height))))
	    pw->panner.slider_y = tmp;
	if (pw->panner.slider_y < 0)
	    pw->panner.slider_y = 0;
    }

    if (pw->panner.last_x != pw->panner.knob_x ||
	pw->panner.last_y != pw->panner.knob_y) {
	XawPannerReport rep;

	Redisplay(gw, (XEvent *) NULL, (Region) NULL);
	rep.changed = (XawPRSliderX | XawPRSliderY);
	rep.slider_x = pw->panner.slider_x;
	rep.slider_y = pw->panner.slider_y;
	rep.slider_width = pw->panner.slider_width;
	rep.slider_height = pw->panner.slider_height;
	rep.canvas_width = pw->panner.canvas_width;
	rep.canvas_height = pw->panner.canvas_height;
	XtCallCallbackList(gw, pw->panner.report_callbacks, (XtPointer) &rep);
    }
}
