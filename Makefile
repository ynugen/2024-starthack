# Compiler flags
CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lm

# Source and object files
SRCS = $(wildcard *.c)
# SRCS = main.c neural.c
OBJS = $(SRCS:.c=.o)

# Executables
# EXECS = $(SRCS:%.c=%)
EXECS = speed_gpu

# Targets
default: all

all: $(EXECS)

$(EXECS): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECS) $(OBJS) $(LDLIBS)

clean:
	rm -f $(OBJS) $(EXECS)


