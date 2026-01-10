#!/bin/bash

# MQTT broker details
BROKER="127.0.0.1"
PORT=1883
QOS=1

# Topic 1 and JSON payload
TOPIC1="/transport/tvm/1/event/create"
PAYLOAD1='{
  "validity_in_days": 999,
  "line_number": 2,
  "request_date": 1768071041
}'

# Topic 2 and string payload
TOPIC2="/transport/gate/GATE-1/event/validate"
PAYLOAD2="ACBCCC"

# Publish the JSON payload
echo "Publishing to $TOPIC1 ..."
mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC1" -q $QOS -m "$PAYLOAD1"

# Publish the string payload
echo "Publishing to $TOPIC2 ..."
mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC2" -q $QOS -m "$PAYLOAD2"

echo "All messages published successfully."



