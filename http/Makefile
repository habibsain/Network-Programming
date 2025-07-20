CC = gcc
CFLAGS = -g -I./include

target = main

SRCS = ./src/client.c main.c
OBJS = $(SRCS:.c=.o)

all: $(target)

$(target): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

		
.PHONY: clean
clean:
	rm $(OBJS) $(target)
