/*-
 * Copyright © 2009, 2011
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

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "kwalletcli.h"

const char __rcsid_main_c[] =
    "$MirOS: contrib/hosted/tg/code/kwalletcli/main.c,v 1.11 2016/08/30 17:37:58 tg Exp $";

#define WOCTET_MASK	(0x7FFFFF80)
#define WOCTET_VALUE	(0x0000EF80)
#define iswoctet(wc)	(((wc) & WOCTET_MASK) == WOCTET_VALUE)


int
main(int argc, char *argv[])
{
	int ch, rv, quiet = 0;
	const char *kw_folder = NULL, *kw_entry = NULL, *kw_pass = NULL, *fmts;
	char *vers;

	while ((ch = getopt(argc, argv, "e:f:hPp:qV")) != -1) {
		switch (ch) {
		case 'e':
			kw_entry = optarg;
			break;
		case 'f':
			kw_folder = optarg;
			break;
		case 'P': {
			char *cp;
			size_t n = 65536;
			ssize_t cnt;

			if ((kw_pass = cp = malloc(n--)) == NULL)
				abort();
			do {
				cnt = read(STDIN_FILENO, cp, n);
				if (cnt == -1) {
					if (errno == EINTR)
						continue;
					break;
				} else if (cnt == 0)
					break;
				n -= cnt;
				cp += cnt;
			} while (n);
			*cp = '\0';
			break;
		}
		case 'p':
			kw_pass = optarg;
			break;
		case 'q':
			quiet = 1;
			break;
		case 'V':
			if (!quiet)
				fprintf(stderr, "%s\n%s\n%s\n",
				    __rcsid_main_c, __rcsid_kwif,
				    KWALLETCLI_H);
			return (0);
		case 'h':
		default:
 usage:
			fprintf(stderr, "Usage: kwalletcli -f folder"
			    " -e entry [-P | -p writepassword]\n");
			return (2);
		}
	}
	if ((argc - optind) || !kw_folder || !kw_entry)
		goto usage;

	if (asprintf(&vers, "%s %s %s", __rcsid_main_c, __rcsid_kwif,
	    KWALLETCLI_H) == -1)
		vers = NULL;

	if (kw_pass) {
		unsigned int wc;
		size_t n;
		char *dst, *cp;
		const char *src = kw_pass;

		/* recode kw_pass from binary/utf-8 to safe utf-8 */
		if ((dst = cp = malloc(strlen(kw_pass) * 3 + 1)) == NULL)
			abort();

		do {
			n = utf_8to32(src, &wc);
			if (n == UTFCONV_ERROR || iswoctet(wc)) {
				/* assert: 0x80 <= *src <= 0xFF */
				wc = *((const unsigned char *)src);
				wc |= WOCTET_VALUE;
				n = 1;
			}
			src += n;
			n = utf_32to8(dst, wc);
			dst += n;
		} while (wc);

		kw_pass = cp;
	}

	if (quiet)
		fclose(stderr);
	rv = kw_io(kw_folder, kw_entry, &kw_pass, vers ? vers : "");
	switch (rv) {
	case KWE_OK_GET: {
		unsigned int wc;
		size_t n;
		char *dst, *cp;
		const char *src = kw_pass;

		/* recode kw_pass from safe utf-8 to binary/utf-8 */
		if ((dst = cp = malloc(strlen(kw_pass) + 1)) == NULL)
			abort();

		do {
			n = utf_8to32(src, &wc);
			if (n == UTFCONV_ERROR)
				/* should never happen */
				goto print_kw_pass;
			src += n;
			if (iswoctet(wc)) {
				wc &= 0xFF;
				*((unsigned char *)dst++) = wc;
			} else {
				n = utf_32to8(dst, wc);
				dst += n;
			}
		} while (wc);

		kw_pass = cp;
 print_kw_pass:
		printf("%s", kw_pass);
		break;
	}
	case KWE_NOWALLET:
		if (!quiet)
			fprintf(stderr, "cannot open wallet\n");
		break;
	case KWE_NOFOLDER:
		fmts = "folder '%s' does not exist\n";
		if (0)
		/* FALLTHROUGH */
	case KWE_ERRFOLDER:
		fmts = "cannot open folder '%s'\n";
		if (!quiet)
			fprintf(stderr, fmts, kw_folder);
		break;
	case KWE_NOENTRY:
		fmts = "entry '%s' does not exist in folder '%s'\n";
		if (0)
		/* FALLTHROUGH */
	case KWE_ERRENTRY:
		fmts = "error reading entry '%s' from folder '%s'\n";
		if (0)
		/* FALLTHROUGH */
	case KWE_ERR_SET:
		fmts = "error writing entry '%s' to folder '%s'\n";
		if (!quiet)
			fprintf(stderr, fmts, kw_entry, kw_folder);
		break;
	case KWE_OK_SET:
		rv = 0;
		break;
	case KWE_ABORT:
		if (!quiet)
			fprintf(stderr, "internal error\n");
	default:
		fflush(NULL);
		abort();
	}
	return (rv);
}
