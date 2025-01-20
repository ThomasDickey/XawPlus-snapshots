/*
 * $XTermId: XawI18n.c,v 1.5 2025/01/19 21:28:27 tom Exp $
 * $Xorg: XawI18n.c,v 1.4 2001/02/09 02:03:47 xorgcvs Exp $
 */

/*

Copyright 2015, 2022  Thomas E. Dickey
Copyright 1991, 1994, 1998  The Open Group

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

*/

#include "private.h"

#include <X11/IntrinsicP.h>

#include "XawI18n.h"

wchar_t _Xaw_atowc(unsigned char c)
{
    wchar_t  wc;
    char str[2];

    str[0] = (char) c;
    str[1] = '\0';

    mbtowc(&wc, str, 1);
    return wc;
}

#ifdef NCR
int _Xaw_iswspace(wchar_t w)
{
    int ret = 0;
    wchar_t s = _Xaw_atowc(' ');
    if (s == w)
	ret = 1;
    return ret;
}
#endif
