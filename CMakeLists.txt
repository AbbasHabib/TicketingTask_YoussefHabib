cmake_minimum_required(VERSION 3.22)
project(transport_ticketing_system_sim)

# enforce -std=c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Needed for clangd the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Enable/Disable output of compile commands during generation for a target." FORCE)

add_subdirectory(back-office)
add_subdirectory(gate)
add_subdirectory(ticket-vending-machine)

