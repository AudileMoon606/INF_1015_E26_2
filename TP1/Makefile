CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

SRC_DIR := TP1
PROGRAMS := pb1 pb2 pb3 pb4 pb5

.PHONY: all clean run-all $(addprefix run-,$(PROGRAMS))

all: $(PROGRAMS)

pb1: $(SRC_DIR)/pb1.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

pb2: $(SRC_DIR)/pb2.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

pb3: $(SRC_DIR)/pb3.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

pb4: $(SRC_DIR)/pb4.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

pb5: $(SRC_DIR)/pb5.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run-all: all $(addprefix run-,$(PROGRAMS))

run-pb1: pb1
	./pb1

run-pb2: pb2
	./pb2

run-pb3: pb3
	./pb3

run-pb4: pb4
	./pb4

run-pb5: pb5
	./pb5

clean:
	rm -f $(PROGRAMS)
