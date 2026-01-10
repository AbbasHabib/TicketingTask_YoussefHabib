cmake_minimum_required(VERSION 3.22)
project(ticket-vending-machine_app)

# enforce -std=c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(PahoMqttCpp REQUIRED)
find_package(cpr REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/CprHttpClient.cpp
    src/PahoMqttClient.cpp
    src/TicketProcessor.cpp
    src/TicketVendingMachine.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    PahoMqttCpp::paho-mqttpp3
    cpr::cpr
)


target_compile_definitions(${PROJECT_NAME} PRIVATE MQTT_SERVER_URI="mqtt://127.0.0.1:29999")
target_compile_definitions(${PROJECT_NAME} PRIVATE HTTP_SERVER_URI="http://127.0.0.1:55556") 
    

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

