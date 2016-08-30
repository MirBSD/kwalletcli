# $MirOS: contrib/hosted/tg/code/kwalletcli/GNUmakefile,v 1.15 2016/08/30 19:33:50 tg Exp $
#-
# Copyright © 2009, 2011, 2016
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

SCRIPTS=	kwalletaskpass kwalletcli_getpin pinentry-kwallet
BINDIR?=	/usr/bin
MANDIR?=	/usr/share/man/man

BINMODE?=	755
MANMODE?=	444
INSTALL_STRIP?=	-s

PROG=		kwalletcli
SRCS=		charconv.c main.c
OBJS=		charconv.o main.o

KDE_VER:=	3
ifeq (${KDE_VER},3)
KDE_INCS?=	-I/usr/include/qt3 -I/usr/include/kde
SRCS+=		kwif3.cc
OBJS+=		kwif3.o
LDADD+=		-lkwalletclient -lkdecore -lqt-mt
else
ifeq (${KDE_VER},4)
KDE_INCS?=	-I/usr/include/qt4 -I/usr/include/qt4/QtCore
SRCS+=		kwif4.cc
OBJS+=		kwif4.o
LDADD+=		-lkdeui -lkdecore -lQtCore
else
ifeq (${KDE_VER},5)
KDE_INCS?=	$(shell pkg-config --cflags Qt5Gui) -I/usr/include/KF5/KI18n \
		-I/usr/include/KF5/KCoreAddons -I/usr/include/KF5/KWallet
SRCS+=		kwif5.cc
OBJS+=		kwif5.o
LDADD+=		-lKF5CoreAddons -lKF5Wallet -lKF5I18n -lQt5Core -lQt5Widgets
else
$(error unknown KDE_VER)
endif
endif
endif

CPPFLAGS+=	${KDE_INCS} -D_GNU_SOURCE

CC?=		gcc
CXX?=		g++

CFLAGS?=	-O2
CXXFLAGS?=	${CFLAGS}

all: ${PROG}

install:
	install -c ${INSTALL_STRIP} -m ${BINMODE} \
	    ${PROG} ${DESTDIR}${BINDIR}/
	install -c -m ${BINMODE} \
	    ${SCRIPTS} ${DESTDIR}${BINDIR}/
	for f in ${PROG} ${SCRIPTS}; do \
		install -c -m ${MANMODE} $$f.1 ${DESTDIR}${MANDIR}1/; \
	done

uninstall:
	for f in ${PROG} ${SCRIPTS}; do \
		rm -f ${DESTDIR}${BINDIR}/$$f ${DESTDIR}${MANDIR}1/$$f.1; \
	done

clean:
	-rm -f ${OBJS} ${PROG}

${PROG}: ${OBJS} ${DPADD}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ ${OBJS} ${LDADD}

.c.o:
	${CC} ${CPPFLAGS} ${CFLAGS} -c -o $@ $<

.cc.o:
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<
