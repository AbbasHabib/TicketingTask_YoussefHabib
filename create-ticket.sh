#!/bin/bash

# check if the TVM and backoffice apps are running
APPS=("ticket-vending-machine" "back-office_app")

for APP in "${APPS[@]}"; do
    if pgrep -x "$APP" >/dev/null; then
        echo "$APP is running."
    else
        echo "$APP is NOT running."
        exit 1
    fi
done

if pgrep -x "$APP_NAME" >/dev/null; then
    echo "$APP_NAME is running."
else
    echo "$APP_NAME is NOT running."
fi

TOPIC="/transport/tvm/1/event/create"
PAYLOAD='{
  "validity_in_days": 999,
  "line_number": 2,
  "request_date": 1768071041
}'
echo "Publishing to $TOPIC ..."
mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC" -q $QOS -m "$PAYLOAD"

echo "Publish complete."
echo "Check folder .runtime-files"

echo "tickets:"
cat ./.runtime-files/tickets.json

