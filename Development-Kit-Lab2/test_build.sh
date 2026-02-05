#!/bin/bash
# Test script for ECE420 Lab 2

# Build the server
echo "Building server..."
make clean
make

if [ ! -f main ]; then
    echo "Build failed!"
    exit 1
fi

echo "Server built successfully!"
echo ""
echo "To run the server:"
echo "  ./main <n_positions> <server_ip> <server_port>"
echo "Example:"
echo "  ./main 1024 127.0.0.1 3000 &"
echo ""
echo "To run the client (in another terminal):"
echo "  gcc -pthread -o client client.c"
echo "  ./client 1024 127.0.0.1 3000"
