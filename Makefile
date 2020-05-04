SRCS := $(wildcard *.c)
CC := gcc

all: $(SRCS)
	$(CC) -O2 -Wall -o buddy.o $(CFLAGS) $(SRCS) -lpthread

clean:
	find ./ -name "*.o" | xargs rm -rf
