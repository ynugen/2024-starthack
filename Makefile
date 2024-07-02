# Compiler flags
CC = gcc
CFLAGS = -Wall -g
LDLIBS =

# Source and object files
SRCS = (*.c)
OBJS = $(SRCS:.c=.o)

# Executables
EXECS = $(SRCS:%.c=%)

# Targets
default: all

all: $(EXE)

$(EXES): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXES) $(OBS) $(LDLIBS)

clean:
	rm -f $(OBJS) $(EXECS)


