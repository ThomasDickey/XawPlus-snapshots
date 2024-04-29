/*
 * $XTermId: LocPixmap.c,v 1.8 2024/04/29 00:00:19 tom Exp $
 */

/*
 * Copyright 2022,2024  Thomas E. Dickey
 *
 * I hacked this up from LocBitmap.c.  Since they (MIT X Consortium) did
 * most of the work and I only hacked it a little bit, I'm including
 * their copyright, HOWEVER, the hacks (specifically the diffs) are
 * Copyright 1990 Robert H. Forsman Jr., distributable under the GNU
 * General Public License.
 *
 * <thoth@cis.ufl.edu>
 *
 * Adapted to XawPlus: Roland Krause 1996.
 * Some bugfixes : Roland Krause 1999
 *
 * roland_krause@freenet.de
 */

/*
 * $XConsortium: LocPixmap.c,v 1.4 89/12/10 10:35:21 rws Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include "private.h"

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/StringDefs.h>
#include <sys/param.h>		/* get MAXPATHLEN if possible */
#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif
#include <X11/Xmu/CvtCache.h>
#include <X11/xpm.h>

#include <ctype.h>
#include <string.h>

#include <X11/XawPlus/StrToPmap.h>

static char **PIXMAPFILEPATHCACHE = NULL;

/* This is the default pixmap path used for pixmaps, which are defined
 * in resouce files of XawPlus applications.
 */
#ifndef PIXMAPDIR
#define PIXMAPDIR "/usr/X11/include/X11/pixmaps"
#endif

/*
 * split_path_string - split a colon-separated list into its constituent
 * parts; to release, free list[0] and list.
 */
static char **
split_path_string(char *src)
{
    int nelems = 1;
    register char *dst;
    char **elemlist, **elem;

    /* count the number of elements */
    for (dst = src; *dst; dst++)
	if (*dst == ':')
	    nelems++;

    /* get memory for everything */
    dst = (char *) malloc((size_t) (dst - src + 1));
    if (!dst)
	return NULL;
    elemlist = (char **) calloc((size_t) (nelems + 1), sizeof(char *));
    if (!elemlist) {
	free(dst);
	return NULL;
    }

    /* copy to new list and walk up nulling colons and setting list pointers */
    strcpy(dst, src);
    for (elem = elemlist, src = dst; *src; src++) {
	if (*src == ':') {
	    *elem++ = dst;
	    *src = '\0';
	    dst = src + 1;
	}
    }
    *elem = dst;

    return elemlist;
}

static XpmColorSymbol *
ExtractXPMColorOverrides(
			    char *string,	/* !CONTENTS ARE MODIFIED! */
			    int *numsymbols)
{
    XpmColorSymbol *colorsymbols = NULL;
    char *s;			/* string scanning pointer */
    int asize;			/* allocated size for colorsymbols */

    asize = 4;			/* allocate 4 at first, expand as necessary */
    colorsymbols = (XpmColorSymbol *) malloc(sizeof(XpmColorSymbol) *
					     (size_t) asize);

    *numsymbols = 0;		/* how many color replacements have we read */

    /* I'm not using any of the more advanced string utilities, because I
       don't know what OS's they are available in.  Sue me */

    /* scan past filename and terminate with \0 */
    for (s = string; *s && !isspace(*s); s++) ;
    if (*s) {
	char *p;		/* placeholder */
	int len;		/* length of color or symbol name */

	*s = '\0';		/* filename is now separated from the rest of the string */
	s++;

	while (*s) {

	    for (; *s && isspace(*s); s++) ;	/* skip over whitespace */
	    if (*s == '\0')
		break;		/* bail if no symbol name */

	    p = s;		/* stash the beginning of the symbol */
	    for (; *s && !isspace(*s); s++) ;	/* scan past symbol */
	    len = (int) (s - p);	/* store length */
	    for (; *s && isspace(*s); s++) ;	/* skip whitespace between symbol and color name */

	    if (*s == '\0') {
		/* complain. I should find a better mechanism than this. */
		fprintf(stderr,
			"Warning: ExtractXPMColorOverrides: symbol name without replacement color name. (%s, %s)\n",
			string, p);
		break;
	    }

	    if (*numsymbols >= asize) {
		colorsymbols = (XpmColorSymbol *) realloc(colorsymbols,
							  sizeof(XpmColorSymbol)
							  * (size_t) (asize
								      *= 2));
		if (colorsymbols == NULL) {
		    fprintf(stderr,
			    "ERROR: ExtractXPMColorOverrides: Out of space for colorsymbols table (%s)\n", string);
		    break;
		}
	    }
	    /* store symbol name */

	    colorsymbols[*numsymbols].name = (char *) malloc((size_t) (len + 1));
	    strncpy(colorsymbols[*numsymbols].name, p, (size_t) len);
	    colorsymbols[*numsymbols].name[len] = '\0';

	    p = s;		/* we are at the beginning of the replacement color name */
	    for (; *s && !isspace(*s); s++) ;	/* scan to the end of the color name */
	    len = (int) (s - p);

	    /* store the color name */
	    colorsymbols[*numsymbols].value = (char *) malloc((size_t) (len
									+ 1));
	    strncpy(colorsymbols[*numsymbols].value, p, (size_t) len);
	    colorsymbols[*numsymbols].value[len] = '\0';

	    (*numsymbols)++;	/* one more replacement in the array */

	    /* go around for another shot. */
	}			/* while */

    }				/* if */
    return colorsymbols;
}

/*
 * XawLocatePixmapFile - read a pixmap file using the normal defaults
 */

Pixmap
XawLocatePixmapFile(
		       Screen *screen,
		       Colormap colormap,
		       char *PixmapName,
		       char *srcname,	/* RETURN */
		       int srcnamelen,
		       int *widthp,
		       int *heightp,	/* RETURN */
		       Pixmap * clipmask)	/* RETURN */
{
    Display *dpy = DisplayOfScreen(screen);
    Window root = RootWindowOfScreen(screen);
    Bool try_plain_name = True;
    XmuCvtCache *cache = _XmuCCLookupDisplay(dpy);
    char **file_paths = NULL;
    char filename[MAXPATHLEN];
    int i;
    char *name;

    XpmAttributes attributes;
    XpmColorSymbol *colorsymbols;
    int numsymbols;

    attributes.visual = screen->root_visual;
    attributes.depth = (unsigned) screen->root_depth;
    attributes.colormap = colormap;
    attributes.valuemask = XpmColormap | XpmDepth | XpmVisual;

    name = (char *) malloc(strlen(PixmapName) + 1);
    (void) strcpy(name, PixmapName);
    colorsymbols = ExtractXPMColorOverrides(name, &numsymbols);

    if (numsymbols) {
	attributes.numsymbols = (unsigned) numsymbols;
	attributes.colorsymbols = colorsymbols;
	attributes.valuemask |= XpmColorSymbols;
    }
    /* name is modified by call */

    /*
     * look in cache for pixmap path
     */
    if (cache) {
	if (!PIXMAPFILEPATHCACHE) {
	    XrmName xrm_name[2];
	    XrmClass xrm_class[2];
	    XrmRepresentation rep_type;
	    XrmValue value;

	    xrm_name[0] = XrmPermStringToQuark("pixmapFilePath");
	    xrm_name[1] = NULLQUARK;
	    xrm_class[0] = XrmPermStringToQuark("PixmapFilePath");
	    xrm_class[1] = NULLQUARK;

	    /*
	     * XXX - warning, derefing Display * until XDisplayDatabase
	     */
	    if (!XrmGetDatabase(dpy)) {
		/* what a hack; need to initialize dpy->db */
		(void) XGetDefault(dpy, "", "");
	    }
	    if (XrmQGetResource(XrmGetDatabase(dpy), xrm_name, xrm_class,
				&rep_type, &value) &&
		rep_type == XrmPermStringToQuark("String")) {
		PIXMAPFILEPATHCACHE = split_path_string(value.addr);
	    }
	}
	file_paths = PIXMAPFILEPATHCACHE;
    }

    /*
     * Search order:
     *    1.  name if it begins with / or ./
     *    2.  "each prefix in file_paths"/name
     *    3.  PIXMAPDIR/name
     *    4.  name if didn't begin with / or .
     */

    for (i = 1; i <= 4; i++) {
	char *fn = filename;
	Pixmap pixmap;

	switch (i) {
	case 1:
	    if (!((name[0] == '/') || ((name[0] == '.') && (name[1] == '/'))))
		continue;
	    fn = name;
	    try_plain_name = False;
	    break;
	case 2:
	    if (file_paths && *file_paths) {
		sprintf(filename, "%s/%s", *file_paths, name);
		file_paths++;
		i--;
		break;
	    }
	    continue;
	case 3:
	    sprintf(filename, "%s/%s", PIXMAPDIR, name);
	    break;
	case 4:
	    if (!try_plain_name)
		continue;
	    fn = name;
	    break;
	}

	if (XpmReadPixmapFile(dpy, root, fn,
			      &pixmap, clipmask, &attributes) == XpmSuccess) {
	    if (widthp)
		*widthp = (int) attributes.width;
	    if (heightp)
		*heightp = (int) attributes.height;
	    if (srcname && srcnamelen > 0) {
		strncpy(srcname, fn, (size_t) (srcnamelen - 1));
		srcname[srcnamelen - 1] = '\0';
	    }
	    /*free up colorsymbol info */
	    free(name);
	    if (numsymbols)
		free(colorsymbols);
	    return pixmap;
	}
    }
    /*free up colorsymbol info */

    free(name);
    if (numsymbols)
	free(colorsymbols);
    return None;
}
