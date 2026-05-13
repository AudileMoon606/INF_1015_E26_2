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
CXXFLAGS := $(STD) $(WARNFLAGS) $(DBGFLAGS) $(SANFLAGS)
LDFLAGS := $(SANFLAGS)

EXEEXT ?=
PROGRAM_BASENAMES := pb1 pb2 pb3 pb4 pb5
PROGRAMS := $(addsuffix $(EXEEXT),$(PROGRAM_BASENAMES))

.PHONY: all clean run-all $(addprefix run-,$(PROGRAM_BASENAMES))

all: $(PROGRAMS)

%$(EXEEXT): %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

ifeq ($(EXEEXT),.exe)
pb1: pb1.exe
pb2: pb2.exe
pb3: pb3.exe
pb4: pb4.exe
pb5: pb5.exe
else
pb1.exe: pb1
pb2.exe: pb2
pb3.exe: pb3
pb4.exe: pb4
pb5.exe: pb5
endif

run-all: all $(addprefix run-,$(PROGRAM_BASENAMES))

$(addprefix run-,$(PROGRAM_BASENAMES)): run-%: %$(EXEEXT)
	./$<

clean:
	rm -f $(PROGRAM_BASENAMES) $(addsuffix .exe,$(PROGRAM_BASENAMES))
