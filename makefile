C:=gcc
CFLAGS:=-o
PREFLAGS:=-E -P

all:
	$(C) $(CFLAGS) out out.c

pre:
	$(C) $(PREFLAGS) $(CFLAGS) out out.c

PHONY: .clean

clean:
	rm -rf *.o out
