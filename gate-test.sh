#!/bin/sh
BROKER="127.0.0.1"
PORT=29999
QOS=1
RUNTIME_DIR=$(realpath ./.runtime-files)


GATE="GATE-EG-99"

TOPIC=/transport/gate/${GATE}/event/validate
PAYLOAD='eyJjcmVhdGlvbl9kYXRlIjoxNzY4MDcxMDQxLCJsaW5lX251bWJlciI6MiwidGlja2V0X2lkIjoxLCJ2YWxpZGl0eV9pbl9kYXlzIjo5OTl9'
echo "Publishing to $TOPIC ..."
mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC" -q $QOS -m "$PAYLOAD"

