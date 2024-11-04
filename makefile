# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall

# Linker flags for OpenSSL
LDFLAGS = -lssl -lcrypto

# Target executable
TARGET = myvcs

# Source files
SRCS = main.cpp add.cpp init.cpp commit.cpp diff.cpp delta.cpp log.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	@echo "Linking $@ with $(OBJS)"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.cpp
	@echo "Compiling $< into $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean
