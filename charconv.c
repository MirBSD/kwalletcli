/*-
 * Copyright © 2011
 *	mirabilos <m@mirbsd.org>
 *
 * Provided that these terms and disclaimer and all copyright notices
 * are retained or reproduced in an accompanying document, permission
 * is granted to deal in this work without restriction, including un‐
 * limited rights to use, publicly perform, distribute, sell, modify,
 * merge, give away, or sublicence.
 *
 * This work is provided “AS IS” and WITHOUT WARRANTY of any kind, to
 * the utmost extent permitted by applicable law, neither express nor
 * implied; without malicious intent or gross negligence. In no event
 * may a licensor, author or contributor be held liable for indirect,
 * direct, other damage, loss, or other issues arising in any way out
 * of dealing in the work, even if advised of the possibility of such
 * damage or existence of a defect, except proven that it results out
 * of said person’s immediate fault when using the work as intended.
 */

#include <stdlib.h>

#include "kwalletcli.h"

const char __rcsid_charconv_c[] =
    "$MirOS: contrib/hosted/tg/code/kwalletcli/charconv.c,v 1.4 2016/08/30 17:37:56 tg Exp $";

/* From MirOS: contrib/hosted/tg/code/any2utf8/wide.c,v 1.1 2009/08/02 17:12:07 tg Exp */


size_t
utf_32to8(char *dst, unsigned int wc)
{
	unsigned char *cp = (unsigned char *)dst;
	unsigned int count;

	if (wc > 0x7FFFFFFF)
		/* beyond UTF-8 */
		abort();

	if (wc < 0x80) {
		count = 0;
		*cp++ = wc;
	} else if (wc < 0x0800) {
		count = 1;
		*cp++ = (wc >> 6) | 0xC0;
	} else if (wc < 0x00010000) {
		count = 2;
		*cp++ = (wc >> 12) | 0xE0;
	} else if (wc < 0x00200000) {
		count = 3;
		*cp++ = (wc >> 18) | 0xF0;
	} else if (wc < 0x04000000) {
		count = 4;
		*cp++ = (wc >> 24) | 0xFC;
	} else {
		count = 5;
		*cp++ = (wc >> 30) | 0xFE;
	}

	while (count)
		*cp++ = ((wc >> (6 * --count)) & 0x3F) | 0x80;

	return ((size_t)((char *)cp - dst));
}

size_t
utf_8to32(const char *src, unsigned int *dst)
{
	const unsigned char *s = (const unsigned char *)src;
	unsigned int wc, count = 0;
	unsigned char c;

	wc = *s++;
	if (wc < 0xC2 || wc >= 0xFE) {
		if (wc >= 0x80)
			return (UTFCONV_ERROR);
	} else if (wc < 0xE0) {
		count = 1; /* one byte follows */
		wc = (wc & 0x1F) << 6;
	} else if (wc < 0xF0) {
		count = 2; /* two bytes follow */
		wc = (wc & 0x0F) << 12;
	} else if (wc < 0xF8) {
		count = 3; /* three bytes follow */
		wc = (wc & 0x07) << 18;
	} else if (wc < 0xFC) {
		count = 4; /* four bytes follow */
		wc = (wc & 0x03) << 24;
	} else /* (wc < 0xFE) */ {
		count = 5; /* five bytes follow */
		wc = (wc & 0x01) << 30;
	}

	while (count) {
		if (((c = *s++) & 0xC0) != 0x80)
			return (UTFCONV_ERROR);
		wc |= (c & 0x3F) << (6 * --count);
		if (!count)
			break;
		if (wc < (1U << (5 * count + 6)))
			return (UTFCONV_ERROR);
	}

	if (wc == 0xFFFE || wc == 0xFFFF || wc > 0x7FFFFFFF ||
	    (wc >= 0xD800 && wc <= 0xDFFF))
		return (UTFCONV_ERROR);
	*dst = wc;
	return ((size_t)((const char *)s - src));
}
