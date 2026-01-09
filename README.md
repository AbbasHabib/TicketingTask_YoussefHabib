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
```