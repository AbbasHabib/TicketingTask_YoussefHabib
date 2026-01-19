FROM mcr.microsoft.com/devcontainers/cpp:1-debian-11

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.22.2"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh


RUN apt-get update

# install clangd
RUN apt-get install -y clangd-19

# install mosquitto
RUN apt-get install -y \
    mosquitto=2.0.11-1+deb11u2 \
    mosquitto-clients=2.0.11-1+deb11u2 \
    libmosquitto-dev=2.0.11-1+deb11u2

# install rapidjson
RUN apt-get install -y \
    rapidjson-dev=1.1.0+dfsg2-7

# install nlohmann/json
RUN wget https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz && \
    tar -xf json.tar.xz && \
    cp -r ./json/include/* /usr/include/ && \
    rm -rf ./json ./json.tar.xz

# install grpc
RUN apt-get install -y \
    protobuf-compiler \
    libprotobuf-dev \
    grpc-proto \
    libgrpc++-dev \
    protobuf-compiler-grpc \
    libgrpc-dev


# install libasio
RUN apt-get install -y libasio-dev

# install Crow
RUN wget https://github.com/CrowCpp/Crow/archive/refs/tags/v1.3.0.tar.gz && \
    tar -xzf ./v1.3.0.tar.gz && \
    cd ./Crow-1.3.0 && \
    mkdir build && cd build && \
    cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF -DCROW_ENABLE_SSL=OFF && \
    make install
RUN rm -rf ./v1.3.0.tar.gz ./Crow-1.3.0

# install libcpr
RUN wget https://github.com/libcpr/cpr/archive/refs/tags/1.12.0.tar.gz && \
    tar -xzf ./1.12.0.tar.gz && \
    cd ./cpr-1.12.0/ && mkdir build && cd build && \
    cmake -DCPR_ENABLE_SSL=OFF -DCPR_USE_SYSTEM_CURL=ON .. && \
    cmake --build . --parallel && \
    cmake --install .
RUN rm -rf ./1.12.0.tar.gz ./cpr-1.12.0/


# install paho.mqtt.c and paho.mqtt.cpp
RUN git clone https://github.com/eclipse/paho.mqtt.cpp && \
    cd paho.mqtt.cpp && \
    git checkout v1.5.3 && \
    git submodule init && \
    git submodule update && \
    cmake -Bbuild -H. -DPAHO_WITH_MQTT_C=ON -DPAHO_BUILD_EXAMPLES=OFF -DPAHO_WITH_SSL=OFF && \
    cmake --build build/ --target install
RUN rm -rf ./paho.mqtt.cpp

# install libtinyxml2
RUN apt-get install -y libtinyxml2-dev=8.0.0+dfsg-2

# install google test
RUN wget https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz && \
    tar -xzf ./googletest-1.17.0.tar.gz && \
    cd ./googletest-1.17.0/ && \
    mkdir build && cd build && \
    cmake .. -DBUILD_SHARED_LIBS=ON -DINSTALL_GTEST=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr && \
    make -j4 && \
    make install && ldconfig
RUN rm -rf ./googletest-1.17.0.tar.gz ./googletest-1.17.0/

# install lcov
RUN apt-get install -y lcov
