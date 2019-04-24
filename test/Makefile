# Makefile for gtest examples

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
  CXXFLAGS += -std=c++11 -Wall
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
  CXXFLAGS += -std=c++11 -Wall -Wno-unknown-pragmas
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
  CXXFLAGS += -Wall -D__STDC_LIMIT_MACROS
  CXXFLAGS += -O3 -DNDEBUG
  CXXOPENMP = -fopenmp
  LDFLAGS +=
  LIBS +=

endif


GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include

G++ = g++
G++_FLAGS = -c -Wall -I $(GOOGLE_TEST_INCLUDE) -std=c++11
LD_FLAGS = -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread

SRC = ../src

OBJECTS = runAll.o example.o $(SRC)/core.o $(SRC)/bfs.o bfs-test.o
TARGET = runAll

DEPH = 


all: $(TARGET)
run: test

$(TARGET): $(SRC)/bfs.o $(SRC)/trim.o $(SRC)/pivot.o $(SRC)/core.o $(SNAP)/Snap.o $(OBJECTS) 
	$(G++) -o $(TARGET) $(OBJECTS) $(LD_FLAGS) $(CXXFLAGS) $(SNAP)/Snap.o -I$(SNAP) -I$(SNAPADV) -I$(GLIB) -I$(SNAPEXP) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(G++) $(G++_FLAGS) $(CXXFLAGS) $(SRC)/bfs.o $(SNAP)/Snap.o -I$(SNAP) -I$(SNAPADV) -I$(GLIB) -I$(SNAPEXP) -I$(SRC) $(LDFLAGS) $(LIBS) $<

$(SNAP)/Snap.o: 
	make -C $(SNAP)

$(SRC)/bfs.o:
	make -C $(SRC)

$(SRC)/pivot.o:
	make -C $(SRC)

$(SRC)/trim.o:
	make -C $(SRC)

$(SRC)/core.o:
	make -C $(SRC)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)
                    
.PHONY: all clean