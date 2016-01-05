C:=gcc
CFLAGS:=-o

all:
	$(C) $(CFLAGS) out out.c

PHONY: .clean

clean:
	rm -rf *.o example
