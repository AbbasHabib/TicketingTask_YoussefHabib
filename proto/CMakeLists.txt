cmake_minimum_required(VERSION 3.22)
project(ticketing-sys-proto)

# enforce -std=c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(PkgConfig REQUIRED)

pkg_check_modules(GRPC REQUIRED grpc++)
pkg_check_modules(PROTOBUF REQUIRED protobuf)

message("GRPC_INCLUDE_DIRS=${GRPC_INCLUDE_DIRS} , PROTOBUF_INCLUDE_DIRS=${PROTOBUF_INCLUDE_DIRS}")
message("GRPC_LIBRARY_DIRS=${GRPC_LIBRARY_DIRS} , PROTOBUF_LIBRARY_DIRS=${PROTOBUF_LIBRARY_DIRS}")
message("GRPC_LIBRARIES=${GRPC_LIBRARIES} , PROTOBUF_LIBRARIES=${PROTOBUF_LIBRARIES}")

## PROTO NAME 
set(PROTO_NAME transaction_tracking)

set(PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_NAME}.proto)

message("PROTO_FILE=${PROTO_FILE}")

# Generate sources from proto manually
execute_process(
    COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/generated
)

execute_process(
    COMMAND protoc -I=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/generated ${PROTO_FILE}
)
execute_process(
    COMMAND protoc -I=${CMAKE_CURRENT_SOURCE_DIR} --grpc_out=${CMAKE_CURRENT_SOURCE_DIR}/generated --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin ${PROTO_FILE}
)

add_library(${PROTO_NAME} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/${PROTO_NAME}.grpc.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/${PROTO_NAME}.grpc.pb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/${PROTO_NAME}.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/${PROTO_NAME}.pb.h
)

target_include_directories(${PROTO_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
    ${GRPC_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(${PROTO_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
    ${GRPC_LIBRARY_DIRS}
    ${PROTOBUF_LIBRARY_DIRS}
)


target_link_libraries(${PROTO_NAME} PUBLIC
    ${GRPC_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)
