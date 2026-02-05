# Makefile for ECE420 Lab 2 - Multithreaded Server

CC = gcc
CFLAGS = -Wall -pthread
TARGET = main
SOURCES = main.c
HEADERS = common.h timer.h

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Clean up compiled files
clean:
	rm -f $(TARGET) server_output_time_aggregated

# Phony targets
.PHONY: all clean
