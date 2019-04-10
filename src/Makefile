#
#	Makefile for this SNAP example
#	- modify Makefile.ex when creating a new SNAP example
#
#	implements:
#		all (default), clean
#

#include ../../Makefile.config
aGLIB = glib-core
aSNAP = snap-core
aGLIBADV = glib-adv
aSNAPADV = snap-adv
aSNAPEXP = snap-exp

GLIB = ../Snap-4.1/$(aGLIB)
SNAP = ../Snap-4.1/$(aSNAP)
GLIBADV = ../Snap-4.1/$(aGLIBADV)
SNAPADV = ../Snap-4.1/$(aSNAPADV)
SNAPEXP = ../Snap-4.1/$(aSNAPEXP)

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  # Linux flags
  CC = g++
  CXXFLAGS += -std=c++98 -Wall
  CXXFLAGS += -O3 -DNDEBUG -fopenmp
  # turn on for crash debugging, get symbols with <prog> 2>&1 | c++filt
  #CXXFLAGS += -g -rdynamic
  #CXXFLAGS += -ggdb
  # turn on for OpenMP
  CXXOPENMP = 
  LDFLAGS +=
  LIBS += -lrt

else ifeq ($(UNAME), Darwin)
  # OS X flags
  CC = g++
  CXXFLAGS += -std=c++98 -Wall -Wno-unknown-pragmas
  CXXFLAGS += -O3 -DNDEBUG
  CLANG := $(shell g++ -v 2>&1 | grep clang | cut -d " " -f 2)
  ifneq ($(CLANG), LLVM)
    CXXFLAGS += -fopenmp
    #CXXOPENMP += -fopenmp
  else
    CXXFLAGS += -DNOMP
    CXXOPENMP =
  endif
  LDFLAGS +=
  LIBS +=

else ifeq ($(shell uname -o), Cygwin)
  # Cygwin flags
  CC = g++
  CXXFLAGS += -Wall -D__STDC_LIMIT_MACROS
  CXXFLAGS += -O3 -DNDEBUG
  CXXOPENMP = -fopenmp
  LDFLAGS +=
  LIBS +=

endif


#include Makefile.ex
MAIN = main
DEPH = core.h
DEPCPP = fwbw.cpp pivot.cpp bfs.cpp scc.cpp trim.cpp

#include ../Makefile.exmain
all: $(MAIN)
# COMPILE
$(MAIN): $(MAIN).cpp $(DEPH) $(DEPCPP) $(SNAP)/Snap.o
	$(CC) $(CXXFLAGS) -o $(MAIN) $(MAIN).cpp $(DEPCPP) $(SNAP)/Snap.o -I$(SNAP) -I$(SNAPADV) -I$(GLIB) -I$(SNAPEXP) $(LDFLAGS) $(LIBS)

$(SNAP)/Snap.o: 
	make -C $(SNAP)

clean:
	rm -f *.o  $(MAIN)  $(MAIN).exe
	rm -rf Debug Release