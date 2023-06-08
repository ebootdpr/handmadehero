#!/bin/bash

# Set the desired compiler (Clang in this case)
CC=clang

# Set additional compiler flags if needed
# For example:
# CFLAGS="-Wall -Wextra -O2"

# Set the name of your source file(s)
SOURCE_FILES="./code/unix64_handmade.cpp"

# Set the name of the output executable
OUTPUT_FILE="./handmade"

# Build the project
# bear -- $CC $CFLAGS $SOURCE_FILES -o $OUTPUT_FILE `pkg-config --cflags --libs gtk+-3.0`
bear -- $CC $CFLAGS -g -include sys/syscall.h -D_GNU_SOURCE $SOURCE_FILES -o $OUTPUT_FILE `pkg-config --cflags --libs x11`
