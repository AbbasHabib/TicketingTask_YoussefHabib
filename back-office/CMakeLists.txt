cmake_minimum_required(VERSION 3.22)
project(back-office_app)

# enforce -std=c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Crow REQUIRED)


add_executable(${PROJECT_NAME}
    src/main.cpp
    src/TicketsController.cpp
    src/TicketsRepository.cpp
    src/TicketsService.cpp
    src/TransactionTrackingServiceImpl.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE TICKETS_FILE_PATH="/data/tickets.json")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Crow::Crow
    transaction_tracking
)