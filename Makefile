
#
# WARNING : g++ and root-system are requiered to compile this example
#


ROOT_CFLAGS     = $(shell root-config --cflags)
ROOT_LIBS       = $(shell root-config --libs)


CC        = g++
CFLAGS    = ${ROOT_CFLAGS}
LIBS      = ${ROOT_LIBS}

SRCEXE    = $(shell ls *.cxx)
EXE       = $(SRCEXE:.cxx=)

all : $(EXE)

$(EXE): $(SRCEXE)
	${CC} $@.cxx -o $@ ${CFLAGS} ${LIBS}

clean :
	rm -f *.o
	rm -f $(EXE)


