cmake_minimum_required(VERSION 3.5)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules")

include(wasm)

#add_subdirectory(libraries)
add_subdirectory(contracts)