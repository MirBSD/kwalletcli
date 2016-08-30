# $MirOS: contrib/hosted/tg/code/kwalletcli/BSDmakefile,v 1.19 2016/08/30 19:33:50 tg Exp $
#-
# Copyright © 2009, 2010, 2011, 2012, 2016
#	mirabilos <m@mirbsd.org>
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
#-
# Note: Qt5 builds may require fPIC in CFLAGS, CXXFLAGS, and LDFLAGS
# (*not* CPPFLAGS!) to successfully build; PIE does not work.

PROG=		kwalletcli
SRCS=		charconv.c main.c
SCRIPTS=	kwalletaskpass kwalletcli_getpin pinentry-kwallet
MAN=		${PROG}.1 ${SCRIPTS:=.1}
BINDIR?=	${BSD_PREFIX}/bin

KDE_VER?=	3
.if ${KDE_VER} == 3
KDE_INCS?=	-I/usr/include/qt3 -I/usr/include/kde
SRCS+=		kwif3.cc
LDADD+=		-lkwalletclient -lkdecore -lqt-mt
.elif ${KDE_VER} == 4
KDE_INCS?=	-I/usr/include/qt4 -I/usr/include/qt4/QtCore
SRCS+=		kwif4.cc
LDADD+=		-lkdeui -lkdecore -lQtCore
.elif ${KDE_VER} == 5
.  ifndef KDE_INCS
QT5_INCS!=	pkg-config --cflags Qt5Gui
.  endif
KDE_INCS?=	${QT5_INCS} -I/usr/include/KF5/KI18n \
		-I/usr/include/KF5/KCoreAddons -I/usr/include/KF5/KWallet
SRCS+=		kwif5.cc
LDADD+=		-lKF5CoreAddons -lKF5Wallet -lKF5I18n -lQt5Core -lQt5Widgets
.else
.  error unknown KDE_VER; valid: 3 4 5
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

CLEANFILES+=	${HTMANS}
htman: .PHONY ${HTMANS}
