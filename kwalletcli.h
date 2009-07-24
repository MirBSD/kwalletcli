/*-
 * Copyright (c) 2009
 *	Thorsten Glaser <tg@mirbsd.org>
 *
 * Provided that these terms and disclaimer and all copyright notices
 * are retained or reproduced in an accompanying document, permission
 * is granted to deal in this work without restriction, including un-
 * limited rights to use, publicly perform, distribute, sell, modify,
 * merge, give away, or sublicence.
 *
 * This work is provided "AS IS" and WITHOUT WARRANTY of any kind, to
 * the utmost extent permitted by applicable law, neither express nor
 * implied; without malicious intent or gross negligence. In no event
 * may a licensor, author or contributor be held liable for indirect,
 * direct, other damage, loss, or other issues arising in any way out
 * of dealing in the work, even if advised of the possibility of such
 * damage or existence of a defect, except proven that it results out
 * of said person's immediate fault when using the work as intended.
 */

#ifndef KWALLETCLI_H
#define KWALLETCLI_H	"$MirOS: contrib/hosted/tg/code/kwalletcli/kwalletcli.h,v 1.5 2009/07/24 12:21:14 tg Exp $"


#ifdef __cplusplus
extern "C" {
#endif


/* exported by kwif*.cc */

extern const char __rcsid_kwif[];

/* this function MUST NOT fail if stderr is closed */
int kw_io(const char *, const char *, const char **, const char *);


/* these are (except abort/usage and OK) the same as exit codes */
#define KWE_OK_GET	0
#define KWE_NOENTRY	1
#define KWE_ABORT	2	/* errorlevel 2 == usage */
#define KWE_NOWALLET	3
#define KWE_NOFOLDER	4
#define KWE_ERRFOLDER	5
#define KWE_ERRENTRY	6
#define KWE_OK_SET	7	/* uses errorlevel 0 */
#define KWE_ERR_SET	8

#ifdef __cplusplus
}
#endif

#endif
