# Define the compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g

# Define the directories
OBJDIR = obj
BINDIR = bin

# Define the executable and object files
EXEC = $(BINDIR)/bplus_tree
OBJS = $(OBJDIR)/BPlusTree.o $(OBJDIR)/main.o

# Create the output directory if it doesn't exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Default target to build the executable
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to build object files
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
