#!/bin/bash


APPS=("back-office_app" "ticket-vending-machine_app" "gate_app" "mosquitto")

for APP in "${APPS[@]}"; do

    PIDS=$(pgrep -x "$APP")
    
    if [ -n "$PIDS" ]; then
        echo "Killing $APP with PID(s): $PIDS"
        kill -9 $PIDS
    else
        echo "$APP is not running."
    fi
done

echo "killed the apps"