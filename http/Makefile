CC = gcc
srcdir = ./include
CFLAGS = -g -I. -I$(srcdir)

target = main

SRCS = ./src/client.c http_get.c
#For testing uncomment below
#SRCS = ./src/client.c client_test.c

OBJS = $(SRCS:.c=.o)

.PHONY: all
all: $(target)

#link object files to create executables
$(target): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 

#Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

		
.PHONY: clean
clean:
	rm $(OBJS) $(target)
