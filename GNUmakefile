# $MirOS: contrib/hosted/tg/code/kwalletcli/GNUmakefile,v 1.4 2009/07/08 12:34:20 tg Exp $
#-
# Copyright © 2009
#	Thorsten Glaser <t.glaser@tarent.de>
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

SCRIPTS=	kwalletaskpass kwalletcli_getpin
BINDIR=		/usr/bin

BINMODE?=	755
INSTALL_STRIP?=	-s

PROG=		kwalletcli
SRCS=		main.c kwif.cc
OBJS=		main.o kwif.o

CPPFLAGS+=	-I/usr/include/qt3 -I/usr/include/kde -D_GNU_SOURCE
LDADD+=		-lkwalletclient

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

clean:
	-rm -f ${OBJS} ${PROG}

${PROG}: ${OBJS} ${DPADD}
	${CXX} ${LDFLAGS} -o $@ ${OBJS} ${LDADD}

.c.o:
	${CC} ${CPPFLAGS} ${CFLAGS} -c -o $@ $<

.cc.o:
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<
