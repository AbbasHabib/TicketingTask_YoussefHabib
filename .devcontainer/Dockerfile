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

# install cppcodec
# RUN wget https://github.com/tplgy/cppcodec/archive/refs/tags/v0.2.tar.gz && \
#     tar -xzf ./v0.2.tar.gz && \
#     cd ./cppcodec-0.2/ && \
#     mkdir build && cd build && \
#     cmake .. -DBUILD_TESTING=OFF && \
#     make install
# RUN rm -rf ./v0.2.tar.gz /cppcodec-0.2/


# install grpc
RUN apt-get install -y \
    protobuf-compiler=3.12.4-1+deb11u1 \
    libprotobuf-dev=3.12.4-1+deb11u1 \
    grpc-proto=0.0~git20200526.dd2dca3-1.1 \
    libgrpc++-dev=1.30.2-3

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


