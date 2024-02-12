# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
LIBS = -lglfw -lGLEW -lGL 
SRCDIR = src


INCLUDES = -I$(SRCDIR) -Iglm -Iimgui

SRC = $(wildcard $(SRCDIR)/*.cpp)  $(wildcard imgui/*.cpp) imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp

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

build/%.o: imgui/%.cpp | build
	$(CXX) $(INCLUDES) -c $< -o $@ $(CXXFLAGS)

build/imgui_impl_glfw.o: imgui/backends/imgui_impl_glfw.cpp | build
	$(CXX) $(INCLUDES) -c $< -o $@ $(CXXFLAGS)

build/imgui_impl_opengl3.o: imgui/backends/imgui_impl_opengl3.cpp | build
	$(CXX) $(INCLUDES) -c $< -o $@ $(CXXFLAGS)

# Cleaning up
clean:
	rm -rf build