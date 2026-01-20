cmake_minimum_required(VERSION 3.22)
project(gate_app)

# enforce -std=c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PahoMqttCpp REQUIRED)
find_package(cpr REQUIRED)

find_path(TINYXML2_INCLUDE_DIR tinyxml2.h)
find_library(TINYXML2_LIBRARY tinyxml2)

message("TINYXML2_INCLUDE_DIR=${TINYXML2_INCLUDE_DIR}")
message("TINYXML2_LIBRARY=${TINYXML2_LIBRARY}")

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/CprHttpClient.cpp
    src/Gate.cpp
    src/PahoMqttClient.cpp
    src/TicketProcessor.cpp
    src/MqttTicketReceiver.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    PahoMqttCpp::paho-mqttpp3
    cpr::cpr
    ${TINYXML2_LIBRARY}
    transaction_tracking
)

target_compile_definitions(${PROJECT_NAME} PRIVATE MQTT_SERVER_URI="mqtt://127.0.0.1:29999")
target_compile_definitions(${PROJECT_NAME} PRIVATE HTTP_SERVER_URI="http://127.0.0.1:55556")
target_compile_definitions(${PROJECT_NAME} PRIVATE GRPC_SERVER_URI="127.0.0.1:55559")
target_compile_definitions(${PROJECT_NAME} PRIVATE GATE_ID_ENV="GATE_ID")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${TINYXML2_INCLUDE_DIR}
)

