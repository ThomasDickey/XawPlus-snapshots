/*
 * $Xorg: PannerP.h,v 1.4 2001/02/09 02:03:45 xorgcvs Exp $
 *
Copyright 1989, 1998 The Open Group

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
 * This file contains modifications for XawPlus, Roland Krause 2000
 */

#ifndef _XawPannerP_h
#define _XawPannerP_h

#include <X11/XawPlus/Panner.h>
#include <X11/XawPlus/SimpleP.h>	/* parent */

typedef struct {			/* new fields in widget class */
    int dummy;
} PannerClassPart;

typedef struct _PannerClassRec {	/* Panner widget class */
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    PannerClassPart	panner_class;
} PannerClassRec;

typedef struct {			/* new fields in widget */
    /* resources... */
    XtCallbackList report_callbacks;	/* callback/Callback */
    Boolean allow_off;			/* allowOff/AllowOff */
    Boolean resize_to_pref;		/* resizeToPreferred/Boolean */
    Dimension default_scale;		/* defaultScale/DefaultScale */
    Dimension canvas_width;		/* canvasWidth/CanvasWidth */
    Dimension canvas_height;		/* canvasHeight/CanvasHeight */
    Position slider_x;			/* sliderX/SliderX */
    Position slider_y;			/* sliderY/SliderY */
    Dimension slider_width;		/* sliderWidth/SliderWidth */
    Dimension slider_height;		/* sliderHeight/SliderHeight */
    Dimension internal_border;		/* internalBorderWidth/BorderWidth */

    /* private data... */
    double haspect, vaspect;		/* aspect ratio of core to canvas */
    struct {
	Boolean doing;			/* tmp graphics in progress */
	Boolean showing;		/* true if tmp graphics displayed */
	Position startx, starty;	/* initial position of slider */
	Position dx, dy;		/* offset loc for tmp graphics */
	Position x, y;			/* location for tmp graphics */
    } tmp;
    Position knob_x, knob_y;		/* real upper left of knob in canvas */
    Dimension knob_width, knob_height;	/* real size of knob in canvas */
    Position last_x, last_y;		/* previous location of knob */
    GC gc;				/* GC for paintings */
} PannerPart;

typedef struct _PannerRec {
    CorePart	 core;
    SimplePart	 simple;
    PannerPart	 panner;
} PannerRec;

#define PANNER_HSCALE(pw,val) ((pw)->panner.haspect * ((double) (val)))
#define PANNER_VSCALE(pw,val) ((pw)->panner.vaspect * ((double) (val)))

#define PANNER_DSCALE(pw,val) (Dimension)  \
  ((((unsigned long) (val)) * (unsigned long) pw->panner.default_scale) / 100L)
#define PANNER_DEFAULT_SCALE 8		/* percent */

#define PANNER_OUTOFRANGE -30000

/*
 * external declarations
 */
extern PannerClassRec pannerClassRec;

#endif /* _XawPannerP_h */
