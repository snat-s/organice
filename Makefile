##
# Organice
# I literally learned from to use make from suckless. Keep it simple!
# See LICENSE file for copyright and license details.
# @version 0.1
SRC = organice.c
OBJ = ${SRC:.c}
PREFIX = /usr
MANPREFIX = /usr/local/share/man

CC = gcc

all: organice

organice:
	${CC} -o organice organice.o
	#gcc organice -o organice

install: all
	@echo installing executable to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f organice ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/organice
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@cp -f organice.1 ${DESTDIR}${MANPREFIX}/man1/organice.1
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/organice.1

uninstall:
	@echo uninstalling executable from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/organice
	@echo uninstalling manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/organice.1

.PHONY: all install uninstall
# end
