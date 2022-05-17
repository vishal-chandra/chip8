EXECUTABLE = chip8
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)


CXX = g++
CXXFLAGS = -std=c++11 -F/Library/Frameworks -framework SDL2 -Wall -Wextra -Werror -pedantic

all: release debug

release: CXXFLAGS += -O3
release: 
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

debug: CXXFLAGS += -g3
debug: 
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)_debug

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

