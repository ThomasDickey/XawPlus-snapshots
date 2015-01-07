/* $Xorg: XawI18n.h,v 1.4 2001/02/09 02:03:47 xorgcvs Exp $ */

/************************************************************************

Copyright 2015 Thomas E. Dickey
Copyright 1993, 1994, 1998  The Open Group

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

This file contains modifications for an easier compilation under Linux,
Roland Krause 2002

**************************************************************************/

#include <stdlib.h>

#ifdef HAS_WCTYPE_H
#include <wctype.h>
#include <widec.h>
#define wcslen(c) wslen(c)
#define wcscpy(d,s) wscpy(d,s)
#define wcsncpy(d,s,l) wsncpy(d,s,l)
#endif

#ifdef HAS_WCHAR_H
#include <wchar.h>
#endif

#ifdef AIXV3
#include <ctype.h>
#endif

#ifdef NCR
#define iswspace(c) _Xaw_iswspace(c)
extern int _Xaw_iswspace(wchar_t);
#endif

#ifdef sony
#ifndef SVR4
#include <jctype.h>
#define iswspace(c) jisspace(c)
#endif
#endif

#ifdef USE_XWCHAR_STRING
#define wcslen(c) _Xwcslen(c)
#define wcscpy(d,s) _Xwcscpy(d,s)
#define wcsncpy(d,s,l) _Xwcsncpy(d,s,l)
#ifdef USE_XMBTOWC
#define mbtowc(wc,s,l) _Xmbtowc(wc,s,l)
#endif
#endif

extern wchar_t _Xaw_atowc (
    unsigned char	c
);

#ifndef HAS_ISW_FUNCS
#include <ctype.h>
#ifndef iswspace
#define iswspace(c) (isascii(c) && isspace(toascii(c)))
#endif
#endif
