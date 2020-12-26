##
# Organice
#
# @file
# @version 0.1
SRC = organice.c
OBJ = ${SRC:.c}

all: clean install

clean:
	rm -f organice

install:
	gcc organice.c -o organice
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f organice ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/organice
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" <organice.1 >${DESTDIR}${MANPREFIX}/man1/organice.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/organice.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/organice
	rm -f ${DESTDIR}${MANPREFIX}/man1/organice.1


# end
