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
  CXXFLAGS += -std=c++11 -Wall
  CXXFLAGS += -O3 -DNDEBUG -fopenmp
  # turn on for crash debugging, get symbols with <prog> 2>&1 | c++filt
  CXXFLAGS += -g -rdynamic
  CXXFLAGS += -ggdb
  LIBS += -lrt

else ifeq ($(UNAME), Darwin)
  # OS X flags
  CC = g++
  CXXFLAGS += -std=c++11 -Wall -Wno-unknown-pragmas
  CXXFLAGS += -O3 -DNDEBUG
  CLANG := $(shell g++ -v 2>&1 | grep clang | cut -d " " -f 2)
  ifneq ($(CLANG), LLVM)
    CXXFLAGS += -fopenmp
  else
    CXXFLAGS += -DNOMP
  endif
  LIBS +=

else ifeq ($(shell uname -o), Cygwin)
  # Cygwin flags
  CC = g++
  CXXFLAGS += -Wall -D__STDC_LIMIT_MACROS
  CXXFLAGS += -O3 -DNDEBUG -fopenmp
  LIBS +=

endif


MAIN = main
DEPH = core.h
DEPCPP = core.cpp pivot.cpp trim.cpp bfs.cpp scc.cpp fwbw.cpp
TESTDEP = core.o pivot.o trim.o bfs.o fwbw.o

all: $(MAIN)
run:
	./$(MAIN)

# COMPILE
$(MAIN): $(MAIN).cpp $(SNAP)/Snap.o $(DEPH) $(DEPCPP) $(TESTDEP)
	$(CC) $(CXXFLAGS) -o $(MAIN) $(MAIN).cpp $(DEPCPP) $(SNAP)/Snap.o -I$(SNAP) -I$(GLIB) $(LIBS)

$(SNAP)/Snap.o: 
	make -C $(SNAP)

%.o: %.cpp $(DEPH) 
	$(CC) $(CXXFLAGS) -c $< $(SNAP)/Snap.o -I$(SNAP) -I$(GLIB) $(LIBS)

clean:
	rm -f *.o  $(MAIN)  $(MAIN).exe
	rm -rf Debug Release