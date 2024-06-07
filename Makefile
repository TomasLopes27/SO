CC = gcc
CFLAGS = -Wall -g

all: client orchestrator

client: client.c

orchestrator: orchestrator.c


clean:
	rm -rf client orchestrator *.dSYM

