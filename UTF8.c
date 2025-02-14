/*
 * $XTermId: UTF8.c,v 1.7 2024/04/28 23:33:56 tom Exp $
 */

/**********************************************************************
 *
 * UTF8.c: Functions to handle UTF8/UCS2 coded strings.
 *
 * These function are used in the Label and SmeBSB widget to
 * deal with UTF8 coded UNICODE/UCS2 strings. These functions
 * work independent of the locale settings. Tis module contains
 * the following functions:
 *
 * int str16len()	A strlen() on a char16 string
 * char16 *str16chr()	A strchr() on a char16 string
 * void str16cpy()	A strcpy() on a char16 string
 * void str16ncpy()	A strncpy() on a char16 string
 * void str16cat()	A strcat() on a char16 string
 *
 * int mbCharLen()	Calc number of byte of the UTF8 character
 * int mbStrLen()	Calc # of characters in UTF8 string
 * char16 *UTF8toUCS2() Convert UTF8 string to UCS2/UNICODE
 *
 * Copyright 2022,2024  Thomas E. Dickey
 * Copyright (c) Roland Krause 2002, roland_krause@freenet.de
 *
 * This module is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 **********************************************************************/

#include "private.h"

#include "UTF8.h"

/* ********************************************************************
 *
 * String functions to deal with 16 bit characters.
 * These functions are used to handle strings with 16 bit encoding.
 *
 **********************************************************************/

/*	Function Name:	str16len
 *	Description: 	Determine the string length of a char16 string
 *			independent of the locale settings.
 *	Arguments:	str16	- A terminated string of char16's
 *	Returns:	Length in char16's
 */
int
str16len(const char16 * str16)
{
    int len = 0;

    while (*str16++)
	len++;
    return len;
}

/*	Function Name:	str16chr
 *	Description: 	Search an 8 bit character in a char16 string.
 *			The upper byte of *ch* is assumed as '0'!
 *	Arguments:	str16	- A terminated string of char16's
 *			ch	- An 8 bit character
 *	Returns:	Position of the leftmost occurance of *ch*
 *			in str16 or NULL.
 */
char16 *
str16chr(char16 *str16, char ch)
{
    char *p;

    while (*str16) {
	p = (char *) str16;
	if ((*p++ == '\0') && (*p == ch))
	    return (str16);
	str16++;
    }
    return NULL;
}

/*	Function Name:	str16cpy
 *	Description: 	Copy a string of char16's from *src* to *dest*
 *	Arguments:	dest	- Destination string
 *			src	- Source string
 *	Returns:	None
 */
void
str16cpy(char16 * dest, const char16 * src)
{
    while (*src)
	*dest++ = *src++;
    *dest = 0;			/* To terminate the string */
}

/*	Function Name:	str16ncpy
 *	Description: 	Copy *n* char16's from *src* to *dest* and
 *			terminate *dest*.
 *	Arguments:	dest	- Destination string
 *			src	- Source string
 *			n	- # of characters to copy
 *	Returns:	None
 */
void
str16ncpy(char16 * dest, const char16 * src, size_t n)
{
    while ((n > 0) && *src) {
	*dest++ = *src++;
	n--;
    }
    *dest = 0;			/* We always terminate the string here */
}

/*	Function Name:	str16cat
 *	Description: 	Concatenate the string of char16's in *src* with *dest*.
 *	Arguments:	dest	- Destination string
 *			src	- Source string
 *	Returns:	None
 */
void
str16cat(char16 * dest, const char16 * src)
{
    while (*dest)
	dest++;			/* search the end of the string */
    while (*src)
	*dest++ = *src++;	/* copy the other behind */
    *dest = 0;			/* and terminate the string */
}

/*	Function Name:	mbCharLen
 *	Description: 	Determine the length in byte of an UTF8 coded
 *			character.
 *	Arguments:	str	- Pointer into an UTF8 coded string
 *	Returns:	Number of byte of the next character in the string
 *			or 0 in case of an error.
 */
int
mbCharLen(const char *str)
{
    unsigned char c = (unsigned char) *str;
    unsigned char mask = 0x80;
    int count = 0;

    if (c) {
	while (c & mask) {
	    count++;
	    mask >>= 1;
	}

	switch (count) {
	case 0:
	    return 1;		/* Character is one byte long */
	case 1:
	    return 0;		/* No multibyte string or wrong position */
	default:
	    return count;
	}
    }
    return 0;			/* End of string reached */
}

/*	Function Name:	mbStrLen
 *	Description: 	Determine the string length of an UTF8 coded string
 *			in characters (not in byte!).
 *	Arguments:	str	- The UTF8 coded string
 *	Returns:	The length in characters, illegal coded bytes
 *			are counted as one character per byte.
 *			See UTF8toUCS2() for the reason!
 */
int
mbStrLen(const char *str)
{
    const char *p = str;
    int clen, len = 0;

    while ((clen = mbCharLen(p)) > 0) {
	len++;
	p += clen;
    }
    return len;
}

/*	Function Name:	UTF8toUCS2
 *	Description: 	Conversion of an UTF8 coded string into UCS2/UNICODE.
 *			If the encoding of the character is not representable
 *			in two bytes, the tilde sign ~ is written into the
 *			result string at this position.
 *			For an illegal UTF8 code an asterix * is stored in
 *			the result string.
 *	Arguments:	str	- The UTF8 coded string
 *	Returns:	The UCS2 coded result string. The allocated memory
 *			for this string has to be freed by the caller!
 *			The result string is stored independent of the
 *			architecture in the high byte/low byte order and is
 *			compatible to the XChar2b format! Type casting is valid.
 *			char16 is used to increase the performance.
 */
char16 *
UTF8toUCS2(const char *str)
{
    char16 *str16, *p16, testINTEL = 1, c16;
    int clen, cInString;
    const char *p;

    /* In the first step we try to determine the string
     * length in characters.
     */
    cInString = mbStrLen(str);
    cInString++;		/* For the terminating null */

    /* Now we need memory for our conversion result */

    str16 = (char16 *) malloc((size_t) cInString * sizeof(char16));
    if (str16) {
	/* Start the conversion: Determine the number of bytes
	 * for the next character, decode it and store the
	 * result in our result string
	 */
	p = str;
	p16 = str16;
	while ((clen = mbCharLen(p)) > 0) {
	    switch (clen) {
	    case 1:
		*p16 = (char16) * p;
		break;

	    case 2:
		c16 = (p[1] & 0x3f) + ((p[0] & 0x1f) << 6);
		*p16 = ((c16 > 0x7f) && ((p[1] & 0xC0) == 0x80)) ? c16 : '*';
		break;

	    case 3:
		c16 = ((p[2] & 0x3f) + ((p[1] & 0x3f) << 6) + ((p[0] & 0xf)
							       << 12));
		*p16 = ((c16 > 0x7ff) && ((p[1] & 0xC0) == 0x80) && ((p[2] &
								     0xC0)
								     ==
		    0x80)) ? c16 : '*';
		break;

	    default:
		*p16 = '~';	/* character code is greater than 0xffff */
	    }
	    p16++;		/* Jump to the next character */
	    p += clen;
	}
	*p16 = 0;		/* String termination */

	/* Swap the bytes, if we are on a machine with an INTEL architecture */

	if (*((char *) &testINTEL)) {
	    char *src, *dest, c;

	    src = dest = (char *) str16;
	    src++;
	    while (*src || *dest) {
		c = *dest;
		*dest = *src;
		*src = c;
		src += 2;
		dest += 2;
	    }
	}
	return str16;
    }
    return NULL;
}
