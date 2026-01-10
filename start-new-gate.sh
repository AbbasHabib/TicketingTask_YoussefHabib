#!/bin/bash


if [ $# -lt 1 ]; then
    echo "Usage: $0 <param1> "
    exit 1
fi


APP_PATH="./build/gate/gate_app"

# Start the app with all parameters passed to the script
GATE_ID=$1 "$APP_PATH" 
