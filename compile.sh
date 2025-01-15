#!/usr/bin/env sh

# Move to the app directory
cd "$(dirname "$(readlink -f "$0")")" || exit

# Source files
SOURCE_FILES="src/main.cpp"
OUTPUT_FILE="SmartReminderLinux"

# Requirements
CONFIG="`pkg-config --cflags --libs gtk+-3.0` --std=c++20"

# Compile the app
g++ $SOURCE_FILES -o $OUTPUT_FILE $CONFIG "$@"