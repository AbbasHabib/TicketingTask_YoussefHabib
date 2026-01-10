#!/bin/bash

####
# Here we test end to end scenario ( create ticket at TVM, Validate it at Gate, having that the back-office & MQTT Broker is up and running
# 
###

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color


# MQTT broker
BROKER="127.0.0.1"
PORT=29999
QOS=1
RUNTIME_DIR=$(realpath ./.runtime-files)
GATE="GATE-1"

APPS=("ticket-vending-machine_app" "gate_app" "back-office_app")

for APP in "${APPS[@]}"; do
    if pgrep -f "$APP" >/dev/null; then
        echo "$APP is running."
    else
        echo "$APP is NOT running."
        exit 1
    fi
done


if ! mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC" -m "ping" -q 0 -d >/dev/null 2>&1; then
    echo "Mosquitto broker is not reachable. Exiting."
    exit 1
fi

# preparing env

read -p "Script need to to empty $RUNTIME_DIR? (yes/no): " answer

if [[ "${answer,,}" == "yes" ]]; then  # convert to lowercase and check
  echo "Emptying $RUNTIME_DIR..."
  rm -rf "$RUNTIME_DIR"/*
  echo "Done."
else
    echo "Have to delete cannot proceed exiting!"
    exit 0
fi

echo ">>>>>> getting new ticket from TVM"
# request create ticket
TOPIC="/transport/tvm/1/event/create"
PAYLOAD='{
  "validity_in_days": 999,
  "line_number": 2,
  "request_date": 1768071041
}'
echo "Publishing to $TOPIC ..."
mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC" -q $QOS -m "$PAYLOAD"

echo "published successfully !"

### check if the .runtime-files contains it
## TODO:




echo ">>>>>>  Checking ticket at the gate"
# check ticket validity
TOPIC=/transport/gate/${GATE}/event/validate
PAYLOAD='eyJjcmVhdGlvbl9kYXRlIjoxNzY4MDcxMDQxLCJsaW5lX251bWJlciI6MiwidGlja2V0X2lkIjoxLCJ2YWxpZGl0eV9pbl9kYXlzIjo5OTl9'
echo "Publishing to $TOPIC ..."
mosquitto_pub -h "$BROKER" -p "$PORT" -t "$TOPIC" -q $QOS -m "$PAYLOAD"

EXPECTED_VALIDATION='<Validation>
    <TicketBase64>eyJjcmVhdGlvbl9kYXRlIjoxNzY4MDcxMDQxLCJsaW5lX251bWJlciI6MiwidGlja2V0X2lkIjoxLCJ2YWxpZGl0eV9pbl9kYXlzIjo5OTl9</TicketBase64>
    <Strategy>ONLINE</Strategy>
    <Result>VALID</Result>'


XML_FILE=${RUNTIME_DIR}/${GATE}.xml

NORMALIZED_EXPECTED=$(echo "$EXPECTED_VALIDATION" | tr -d '\n\r' | tr -d '[:space:]')
NORMALIZED_FILE=$(tr -d '\n\r' < "$XML_FILE" | tr -d '[:space:]')

if grep -qF "$NORMALIZED_EXPECTED" <<< "$NORMALIZED_FILE"; then
    echo -e "${GREEN}Validation record exists in XML.${NC}"
    echo -e "${GREEN}OK ${NC}"
else
    echo -e "${RED}Validation record NOT found in XML.${NC}"
    echo -e "${RED}NOK ${NC}"
fi

EXPECTED_TICKET='{
    "creation_date": 1768071041,
    "line_number": 2,
    "ticket_id": 1,
    "validity_in_days": 999
  }'

JSON_FILE=${RUNTIME_DIR}/tickets.json


NORMALIZED_EXPECTED=$(echo "$EXPECTED_TICKET" | tr -d '\n\r' | tr -d '[:space:]')
NORMALIZED_FILE=$(tr -d '\n\r' < "$JSON_FILE" | tr -d '[:space:]')


if grep -qF "$NORMALIZED_EXPECTED" <<< "$NORMALIZED_FILE"; then
    echo -e "${GREEN}Validation record exists in XML.${NC}"
    echo -e "${GREEN}OK ${NC}"
else
    echo -e "${RED}Validation record NOT found in XML.${NC}"
    echo -e "${RED}NOK ${NC}"
fi


