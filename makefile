C:=gcc
CFLAGS:=-o

all:
	$(C) $(CFLAGS) example example.c

PHONY: .clean

clean:
	rm -rf *.o example
