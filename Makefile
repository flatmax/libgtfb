SBINDIR=usr/sbin
MANDIR=usr/man
INCLUDEDIR=usr/include
DOCDIR=usr/share/doc/libGTFB
LIBDIR=usr/lib

CC=g++ -O3 -Wall
.cc.o:
	$(CC) -fPIC -c $<
.C.o:
	$(CC) -fPIC -c $<
#MAKELIB=$(CC) -Wl,-shared,-soname
MAKELIB=$(CC) -shared -Wl,-soname

SRC=GT.cc GTFB.cc
OBJ=$(SRC:.cc=.o)

all: $(OBJ)\
	ex\
	libs

ex:
#	$(CC) $(OBJ) -o GTTest GTTest.cc
#	$(CC) $(OBJ) -o GTFBTest GTFBTest.cc
#	$(CC) -o GTSensitivityTest GTSensitivityTest.cc

libs:
	$(MAKELIB),libGTFB.so.0.1.0 -o libGTFB.so.0.1.0 $(OBJ)
	ln -s libGTFB.so.0.1.0 libGTFB.so.0.1
	ln -s libGTFB.so.0.1.0 libGTFB.so.0
	ln -s libGTFB.so.0.1.0 libGTFB.so

install:
	mkdir -p ${DESTDIR}
	mkdir -p ${DESTDIR}/${LIBDIR}
	mkdir -p ${DESTDIR}/${DOCDIR}
	mkdir -p ${DESTDIR}/${INCLUDEDIR}
	install -m 0755 *.so*  ${DESTDIR}/${LIBDIR}
	install -m 0666 *Test.cc ${DESTDIR}/${DOCDIR}
	install -m 0660 *.H ${DESTDIR}/${INCLUDEDIR}

clean:
	rm -f *~ *.o *.dat GTFBTest lib* *Test
	rm -f *.[roh] *.cxx *.rsp HowToUseGCFB *.plt *.txt

tar:
	tar cf gammatone.tar *.H *.cc Makefile

