# $MirOS: contrib/hosted/tg/code/kwalletcli/BSDmakefile,v 1.11 2010/01/11 18:34:11 tg Exp $
#-
# Copyright © 2009, 2010
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
SRCS=		main.c
SCRIPTS=	kwalletaskpass kwalletcli_getpin pinentry-kwallet
MAN=		${PROG}.1 ${SCRIPTS:=.1}
BINDIR?=	${BSD_PREFIX}/bin

KDE_VER?=	3
.if ${KDE_VER} == 3
KDE_INCS?=	-I/usr/include/qt3 -I/usr/include/kde
SRCS+=		kwif3.cc
LDADD+=		-lkwalletclient
.elif ${KDE_VER} == 4
KDE_INCS?=	-I/usr/include/qt4 -I/usr/include/qt4/QtCore
SRCS+=		kwif4.cc
LDADD+=		-lkdeui
.else
.  error unknown KDE_VER
.endif

CPPFLAGS+=	${KDE_INCS} -D_GNU_SOURCE

afterinstall:
.for _i in ${SCRIPTS}
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m ${BINMODE} \
	    ${.CURDIR}/${_i} ${DESTDIR}${BINDIR}/
.endfor

.include <bsd.prog.mk>

# HTML manpage generation code (currently assumes the basenames
# of all manpages to be generated are unique, unlike man(1, 7).
HTMANMODE?=	local		# one of local or sectioned
.for _p in ${MANALL}
HTMANS+=	${_p:R}.htm
${_p:R}.htm: ${_p}
	@(. ${BSDSRCDIR}/scripts/roff2htm; \
	    do_conversion_verbose ${_p:R} ${_p:E:S/cat//} ${_p} $@)
.endfor
.for _s _t in ${MLINKS}
HTMANS+=	${_t:R}.htm
${_t:R}.htm: ${_s:R}.htm
	@print -ru2 ${_t:R:Q}.htm ← ${_s:R:Q}.htm
.if ${HTMANMODE:L:Mlocal}
	@(print '/<title>/s#</title>#, ${_t:R}(${_t:E})&#'; \
	  print '/<h1>/s#</h1>#, <a href="$@">${_t:R}(${_t:E})</a>&#'; \
	  print wq) | ed -s ${_s:R}.htm
.else
	@(print '/<title>/s#</title>#, ${_t:R}(${_t:E})&#'; \
	  print '/<h1>/s#</h1>#, <a' \
	    'href="../man${_t:E}/$@">${_t:R}(${_t:E})</a>&#'; \
	  print wq) | ed -s ${_s:R}.htm
.endif
	@ln -f ${.ALLSRC} $@
.endfor

CLEANFILES+=	${HTMANS}
htman: .PHONY ${HTMANS}
