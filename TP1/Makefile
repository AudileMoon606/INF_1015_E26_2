CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

SRC_DIR := TP1
PROGRAMS := pb1 pb2 pb3 pb4 pb5

.PHONY: all clean run-all $(addprefix run-,$(PROGRAMS))

all: $(PROGRAMS)

pb1: pb1.cpp fonctions.cpp
	$(CXX) $(CXXFLAGS) pb1.cpp fonctions.cpp -o $@


pb2: pb2.cpp fonctions.cpp
	$(CXX) $(CXXFLAGS) pb2.cpp fonctions.cpp -o $@


pb3: pb3.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

pb4: pb4.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

pb5: pb5.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run-all: all $(addprefix run-,$(PROGRAMS))

run-pb1: pb1
	pb1.exe
	

run-pb2: pb2
	pb2.exe
	

run-pb3: pb3
	pb3.exe
	

run-pb4: pb4
	pb4.exe
	

run-pb5: pb5
	pb5.exe
	

clean:
	rm -f $(PROGRAMS)
