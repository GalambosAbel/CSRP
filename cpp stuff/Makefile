# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Source files
SRCS = $(wildcard *.cpp)

# Header files
HDRS = $(wildcard *.h)

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = program.exe

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	for file in $(OBJS) $(EXEC); do if [ -e $$file ]; then rm $$file; fi; done

# Phony targets
.PHONY: all clean