CC = gcc
BIN = part1
CFLAGS= -Wall
LDFLAGS=

all: $(BIN)

part1: functions.o xml.o dtd.o main.o

	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c

	$(CC) -o $@  -c $< $(CFLAGS)


.PHONY: clean mrproper

clean:

	rm -rf *.o

mrproper: clean

	rm -rf $(BIN)