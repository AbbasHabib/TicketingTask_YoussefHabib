cmake_minimum_required(VERSION 3.22)

SET(TEST_NAME "back-office_test")
# enforce -std=c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)

include(CTest)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags")
    add_compile_options(-O0 -g --coverage)
    add_link_options(--coverage)
endif()


SET(SRC_SOURCES 
    ${PROJECT_SOURCE_DIR}/src/TicketsService.cpp
    ${PROJECT_SOURCE_DIR}/src/TicketsController.cpp
    ${PROJECT_SOURCE_DIR}/src/JsonTicketsRepository.cpp
    ${PROJECT_SOURCE_DIR}/src/TransactionTrackingServiceImpl.cpp
)

add_executable(${TEST_NAME}
    unittests/TicketsServiceTest.cpp
    unittests/JsonTicketsRepositoryTest.cpp
    unittests/TicketsControllerTest.cpp
    ${SRC_SOURCES}
)

target_include_directories(${TEST_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${TEST_NAME} PRIVATE
    Crow::Crow
    transaction_tracking
    GTest::gtest_main
    GTest::gmock_main
)

target_compile_definitions(${TEST_NAME} PRIVATE
    BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}"
)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
