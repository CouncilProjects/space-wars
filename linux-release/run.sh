#!/bin/bash

# Directory where your application and libraries are located
DIR="$(cd "$(dirname "$0")" && pwd)"

# Export LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$DIR/libs:$LD_LIBRARY_PATH"

# Run the application but make it so that you can close the terminal
nohup "$DIR/Space_Wars" > /dev/null 2>&1

rm nohup.out


