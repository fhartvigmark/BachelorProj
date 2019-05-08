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
  CXXFLAGS += -g -rdynamic
  CXXFLAGS += -ggdb
  LIBS += -lrt
else ifeq ($(UNAME), Darwin)
  # OS X flags
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
  CXXFLAGS += -Wall -D__STDC_LIMIT_MACROS -std=c++11
  CXXFLAGS += -O3 -DNDEBUG -fopenmp
  LIBS +=
endif

GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include

G++ = g++
G++_FLAGS = -c -Wall -I $(GOOGLE_TEST_INCLUDE) -std=c++11
LD_FLAGS = -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread

SRC = ../src

DEPH = $(SRC)/core.o $(SRC)/bfs.o $(SRC)/pivot.o $(SRC)/trim.o $(SRC)/fwbw.o
OBJECTS = $(DEPH) \
	runAll.o example.o \
	bfs-test.o \
	enhancedgraph-test.o \
	pivot-test.o \
	trim-test.o \
	fwbw-test.o
TARGET = runAll


all: $(TARGET)
run: test

$(TARGET): $(SNAP)/Snap.o $(OBJECTS) 
	$(G++) -o $(TARGET) $(OBJECTS) $(LD_FLAGS) $(CXXFLAGS) $(SNAP)/Snap.o -I$(SNAP) -I$(GLIB) $(LIBS)

%.o: %.cpp
	$(G++) $(G++_FLAGS) $(CXXFLAGS) $(DEPH) $(SNAP)/Snap.o -I$(SNAP) -I$(GLIB) -I$(SRC) $(LIBS) $<

$(SNAP)/Snap.o: 
	make -C $(SNAP)

$(SRC)/%.o: $(SRC)/%.cpp $(SRC)/%.h 
	make -C $(SRC)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)
                    
.PHONY: all run test clean