#!/bin/sh
# MQTT broker
BROKER="127.0.0.1"
PORT=29999
QOS=1
RUNTIME_DIR=$(realpath ./.runtime-files)
GATE="GATE-1"



TOPIC="/transport/tvm/1/event/create"
PAYLOAD='{
  "validity_in_days": 999,
  "line_number": 2,
  "request_date": 1768071041
}'
echo "Publishing to $TOPIC ..."
mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC" -q $QOS -m "$PAYLOAD"