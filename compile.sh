#!/usr/bin/env sh

# Move to the app directory
cd "$(dirname "$(readlink -f "$0")")" || exit

# Source files
SOURCE_FILES="src/main-linux.cpp src/dbe16.cpp"
OUTPUT_FILE="SmartReminderLinux"

# Include paths
INCLUDE_DIR="-I/usr/include -I/usr/local/include"
LIB_DIR="-L/usr/lib -L/usr/local/lib"

# Requirements
LIBS="-lglfw -ldl -lGL"

# C++ Version
CXXVERSION="17"

# Compile the app
exec g++ $SOURCE_FILES -o $OUTPUT_FILE --std=c++$CXXVERSION $INCLUDE_DIR $LIB_DIR $LIBS "$@"