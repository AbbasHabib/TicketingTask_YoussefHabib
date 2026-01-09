project(back-office_app)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/TicketsController.cpp
    src/TicketsRepository.cpp
    src/TicketsService.cpp
)

find_package(Crow REQUIRED)
find_package(RapidJSON REQUIRED)

target_compile_definitions(${PROJECT_NAME} PRIVATE TICKETS_FILE_PATH="/data/tickets.json")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RAPIDJSON_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Crow::Crow
)