/*-
 * Copyright © 2009
 *	Thorsten Glaser <t.glaser@tarent.de>
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
#include <unistd.h>

#include "kwalletcli.h"

extern const char __rcsid_kwif_cc[];
const char __rcsid_main_c[] =
    "$MirOS: contrib/hosted/tg/code/kwalletcli/main.c,v 1.2 2009/06/10 18:36:42 tg Exp $";

int
main(int argc, char *argv[])
{
	int ch, rv;
	const char *kw_folder = NULL, *kw_entry = NULL, *kw_pass = NULL, *fmts;
	char *vers;

	while ((ch = getopt(argc, argv, "e:f:hPp:V")) != -1) {
		switch (ch) {
		case 'e':
			kw_entry = optarg;
			break;
		case 'f':
			kw_folder = optarg;
			break;
		case 'P': {
			char *cp;
			size_t n = 2047;
			ssize_t cnt;

			if ((kw_pass = cp = malloc(2048)) == NULL)
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
		case 'V':
			fprintf(stderr, "%s\n%s\n", __rcsid_main_c,
			    __rcsid_kwif_cc);
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

	if (asprintf(&vers, "%s %s", __rcsid_main_c, __rcsid_kwif_cc) == -1)
		vers = NULL;

	rv = kw_io(kw_folder, kw_entry, &kw_pass, vers ? vers : "");
	switch (rv) {
	case KWE_OK_GET:
		printf("%s", kw_pass);
		break;
	case KWE_NOWALLET:
		fprintf(stderr, "cannot open wallet\n");
		break;
	case KWE_NOFOLDER:
		fmts = "folder '%s' does not exist\n";
		if (0)
		/* FALLTHROUGH */
	case KWE_ERRFOLDER:
		fmts = "cannot open folder '%s'\n";
		fprintf(stderr, fmts, kw_folder);
		break;
	case KWE_NOENTRY:
		fmts = "entry '%s' does not exist in folder '%s'\n";
		if (0)
	case KWE_ERRENTRY:
		fmts = "error reading entry '%s' from folder '%s'\n";
		if (0)
	case KWE_ERR_SET:
		fmts = "error writing entry '%s' to folder '%s'\n";
		fprintf(stderr, fmts, kw_entry, kw_folder);
		break;
	case KWE_OK_SET:
		rv = 0;
		break;
	case KWE_ABORT:
		fprintf(stderr, "internal error\n");
	default:
		fflush(NULL);
		abort();
	}
	return (rv);
}
