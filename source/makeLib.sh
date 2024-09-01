#!/bin/bash

# Ensure a program is provided as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <program>"
    exit 1
fi

PROGRAM="$1"

# Create the 'libs' directory if it doesn't exist
mkdir -p libs

# Function to recursively find and copy libraries
copy_libs() {
    local lib="$1"
    local lib_dir="$(dirname "$lib")"
    local lib_name="$(basename "$lib")"

    # Check if the library already exists in the 'libs' directory
    if [ ! -f "libs/$lib_name" ]; then
        echo "Copying $lib_name to libs/"
        cp "$lib" libs/

        # Find dependencies of the library
        local deps
        deps=$(ldd "$lib" | awk '{print $3}' | grep -v '^(')
        
        for dep in $deps; do
            if [ -n "$dep" ] && [ -f "$dep" ]; then
                copy_libs "$dep"
            fi
        done
    fi
}

# Find and copy dependencies of the main program
libs_to_check=$(ldd "$PROGRAM" | awk '{print $3}' | grep -v '^(')

for lib in $libs_to_check; do
    if [ -n "$lib" ] && [ -f "$lib" ]; then
        copy_libs "$lib"
    fi
done

echo "Libraries copied to 'libs/' directory."
