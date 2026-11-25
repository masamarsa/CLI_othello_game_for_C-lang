CC = cc
CFLAGS = -std=gnu23 -O2 -Wall -Wextra -Wpedantic
OBJECTS = othello.o board.o console.o

all: othello

othello: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

othello.o: othello.c board.h console.h
board.o: board.c board.h
console.o: console.c board.h console.h

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f othello $(OBJECTS)

.PHONY: all clean
