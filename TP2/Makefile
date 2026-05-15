CXX := g++
STD := -std=c++17
WARNFLAGS := -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion \
	-Wshadow -Wold-style-cast -Wcast-align -Wformat=2 -Wnull-dereference \
	-Wdouble-promotion -Wimplicit-fallthrough -Woverloaded-virtual \
	-Wnon-virtual-dtor -Wduplicated-cond -Wduplicated-branches -Wlogical-op \
	-Wuseless-cast -Wmisleading-indentation
DBGFLAGS := -g3 -ggdb -O0 -fno-omit-frame-pointer
ENABLE_SANITIZERS ?= 0
SANFLAGS :=
ifeq ($(ENABLE_SANITIZERS),1)
SANFLAGS := -fsanitize=address,undefined -fno-sanitize-recover=all
endif
CPPFLAGS := -Iinclude
CXXFLAGS := $(STD) $(WARNFLAGS) $(DBGFLAGS) $(SANFLAGS)
LDFLAGS := $(SANFLAGS)

EXEEXT ?=
PROGRAM_BASENAME := td2
PROGRAM := $(PROGRAM_BASENAME)$(EXEEXT)

SOURCES := td2.cpp \
	bibliotheque_cours/bibliotheque_cours.cpp \
	bibliotheque_cours/verification_allocation.cpp

HEADERS := structures.hpp \
	include/bibliotheque_cours.hpp \
	include/verification_allocation.hpp \
	include/debogage_memoire.hpp

.PHONY: all clean run remise

all: $(PROGRAM)

$(PROGRAM): $(SOURCES) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SOURCES) -o $@ $(LDFLAGS)

ifeq ($(EXEEXT),.exe)
td2: td2.exe
else
td2.exe: td2
endif

run: $(PROGRAM)
	./$<

clean:
	rm -f td2 td2.exe

remise:
	zip TD2_2336883_2506190.zip *.cpp *.hpp Makefile
