UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  # Linux flags
  CC = g++
  CXXFLAGS += -std=c++14 -Wall -pipe -w
  CXXFLAGS += -O3 -DNDEBUG -fopenmp
  # turn on for crash debugging, get symbols with <prog> 2>&1 | c++filt
  #CXXFLAGS += -g -rdynamic
  #CXXFLAGS += -ggdb
  LIBS += -lrt

else ifeq ($(UNAME), Darwin)
  # OS X flags
  CC = g++
  CXXFLAGS += -std=c++14 -Wall -Wno-unknown-pragmas -pipe -w
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
  CXXFLAGS += -Wall -D__STDC_LIMIT_MACROS -pipe -w
  CXXFLAGS += -O3 -DNDEBUG -fopenmp
  LIBS +=

endif

NUM_CORES ?= $(shell grep -c "vendor_id" /proc/cpuinfo)
MAKEFLAGS = " -j$(NUM_CORES) -l$(NUM_CORES) "

MAIN = scc_checker graph_converter pivot_extract

all: $(MAIN)
run:
	./$(MAIN)

# COMPILE
%: %.cpp
	$(CC) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f *.o  $(MAIN)  $(MAIN).exe
	rm -rf Debug Release

.PHONY: all run clean