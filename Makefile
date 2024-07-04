# Define the compiler
CC = gcc

# Define compiler flags
CFLAGS = -Wall -g

# Define the target executable
TARGET = main

# Define the source files
SRCS = main.c neural.c

# Define the header files
HEADERS = neural.h

# Define the object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Define output files
OUT = test.txt

# Default target to build the executable
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove generated files
clean:
	rm -f $(OBJS) $(TARGET) $(OUT)

# Phony targets (not actual files)
.PHONY: all clean
