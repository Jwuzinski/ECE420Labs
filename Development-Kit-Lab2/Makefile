# Makefile for ECE420 Lab 2 - Multithreaded Server

CC = gcc
CFLAGS = -Wall -pthread
TARGETS = main client attacker
HEADERS = common.h timer.h

# Default target
all: $(TARGETS)

# Build the main executable
main: main.c $(HEADERS)
	$(CC) $(CFLAGS) -o main main.c

# Build the client executable
client: client.c $(HEADERS)
	$(CC) $(CFLAGS) -o client client.c

# Build the attacker executable
attacker: attacker.c $(HEADERS)
	$(CC) $(CFLAGS) -o attacker attacker.c -lm

# Clean up compiled files
clean:
	rm -f $(TARGETS) server_output_time_aggregated

# Phony targets
.PHONY: all clean
