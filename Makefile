CC     = gcc
CFLAGS = -Wall -g

APPS  = a1 a2 a3 a4 a5 a6
PROGS = kernelSim interControllerSim $(APPS)

all: $(PROGS)

kernelSim: kernelSim.c constants.h
	$(CC) $(CFLAGS) -o $@ $<

interControllerSim: interControllerSim.c constants.h
	$(CC) $(CFLAGS) -o $@ $<

a%: a%.c constants.h
	$(CC) $(CFLAGS) -o $@ $<

run: all
	./kernelSim

clean:
	rm -f $(PROGS)

.PHONY: all run clean