# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
LIBS = -lglfw -lGLEW -lGL
SRCDIR = src
IMGUI_DIR = imgui
INCLUDES = -I$(SRCDIR) -I$(IMGUI_DIR) -Iglm 

SRC = $(wildcard $(SRCDIR)/*.cpp)

OBJ = $(addprefix build/, $(notdir $(SRC:.cpp=.o)))
EXEC = build/rayTracingAssignment1

# Default target
all: $(EXEC)

# Create the build directory
build:
	mkdir -p build

# Linking
$(EXEC): $(OBJ)
	$(CXX) $^ -o $@ $(LIBS)

# Compiling
build/%.o: $(SRCDIR)/%.cpp | build
	$(CXX) $(INCLUDES) -c $< -o $@ $(CXXFLAGS)

build/%.o: $(IMGUI_DIR)/%.cpp | build
	$(CXX) $(INCLUDES) -c $< -o $@ $(CXXFLAGS)

# Cleaning up
clean:
	rm -rf build