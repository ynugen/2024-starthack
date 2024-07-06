CC = gcc

CFLAGS = -Wall -g

TARGET = main

SRCS = main.c neural.c

HEADERS = neural.h

OBJS = $(SRCS:.c=.o)

OUT = test.txt results.csv

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(OUT)

.PHONY: all clean
