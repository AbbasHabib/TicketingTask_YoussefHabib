# transport-ticketing-sim-cpp
C++ server-client transport ticketing simulator


## System Bird's-Eye View
![Alt text here](.diagrams/transport-ticketing-sys-sim.drawio.png)

## Getting Started

### Developement Environment Preperation
#### Using VScode .devcontainer
This repository includes a VS Code Dev Container for a fully reproducible development environment with all required tools preinstalled.


Required Tools
- [Docker](https://docs.docker.com/engine/install/)
- [VS Code](https://code.visualstudio.com/Download)
- [VS Code Remote – Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)


Inside Vscode Press `F1` then write ` >dev containers: Rebuild and reopen in Container` 

#
Having that the development environment is established successfully you can proceed with the following sections

#### Build the applications
```bash
# at repo top dir
mkdir build
cd build
cmake ..
make
```
Result
```
build/
 |/back-office/
 |/gate/
 |/ticket-vending-machine/
 |/proto
```

after build the proto folder contains the in repo top dir will contain generated protobuf grpc c++ code to compile into a lib and be used by the `back-office` service and `gate` client

### Use docker compose to fire up all the apps
docker compose up will start
- MQTT Broker : TCP Port `55555` 
- Back-Office HTTP Server : TCP Port `55556` 
- Back-Office gRPC Server : TCP Port `55557` 
- client gate application
- client ticket-vending-machine application

### User can use postman to test HTTP and gRPC server (back-office)
Examples can be found in the [back-office](back-office)

### Apps logs and Analytics
the .devcontainer has bind to the repo dir `.runtime-files`
apps will create it's Persistent files into it
example:
```bash
GATE-EG-3.xml
# <GateReport>
#     <GateId>GATE-EG-3</GateId>
#     <Summary>
#         <TotalValidations>3</TotalValidations>
#         <ValidCount>3</ValidCount>
#         <InvalidCount>0</InvalidCount>
#         <OnlineCount>0</OnlineCount>
#         <OfflineCount>3</OfflineCount>
#     </Summary>
#     <Validations>
#         <Validation>
#             <TicketBase64>eyJjcmVhdGlvbl9kYXRlIjoxNzY3OTgzNDg5LCJsaW5lX251bWJlciI6MSwidGlja2V0X2lkIjowLCJ2YWxpZGl0eV9pbl9kYXlzIjoxfQ==</TicketBase64>
#             <Strategy>OFFLINE</Strategy>
#             <Result>VALID</Result>
#             <Timestamp>1768066912</Timestamp>
#         </Validation>...
tickets.json
# {
#     "tickets": [
#         {
#             "creation_date": 1768005495,
#             "line_number": 1,
#             "ticket_id": 1,
#             "validity_in_days": 1
#         },
#         {
#             "creation_date": 1768005502,
#             "line_number": 1,
#             "ticket_id": 2,
#             "validity_in_days": 1
#         },
#         {
#             "creation_date": 1768005509,
#             "line_number": 1,
#             "ticket_id": 3,
#             "validity_in_days": 1
#         }, ...
```
