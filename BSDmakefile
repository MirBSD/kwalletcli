# $MirOS: contrib/hosted/tg/code/kwalletcli/BSDmakefile,v 1.6 2009/07/08 12:34:19 tg Exp $
#-
# Copyright © 2009
#	Thorsten Glaser <tg@mirbsd.org>
#
# Provided that these terms and disclaimer and all copyright notices
# are retained or reproduced in an accompanying document, permission
# is granted to deal in this work without restriction, including un‐
# limited rights to use, publicly perform, distribute, sell, modify,
# merge, give away, or sublicence.
#
# This work is provided “AS IS” and WITHOUT WARRANTY of any kind, to
# the utmost extent permitted by applicable law, neither express nor
# implied; without malicious intent or gross negligence. In no event
# may a licensor, author or contributor be held liable for indirect,
# direct, other damage, loss, or other issues arising in any way out
# of dealing in the work, even if advised of the possibility of such
# damage or existence of a defect, except proven that it results out
# of said person’s immediate fault when using the work as intended.

PROG=		kwalletcli
SRCS=		main.c kwif.cc
SCRIPTS=	kwalletaskpass kwalletcli_getpin # pinentry-kwallet
MAN=		${PROG}.1 ${SCRIPTS:=.1}

KDE_INCS?=	-I/usr/include/qt3 -I/usr/include/kde
CPPFLAGS+=	${KDE_INCS} -D_GNU_SOURCE
LDADD+=		-lkwalletclient

afterinstall:
.for _i in ${SCRIPTS}
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m ${BINMODE} \
	    ${.CURDIR}/${_i} ${DESTDIR}${BINDIR}/
.endfor

.include <bsd.prog.mk>
