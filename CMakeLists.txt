# Set the minimum required version of cmake for a project
cmake_minimum_required(VERSION 3.5)

# Set a name, version, and enable languages for the entire project.
project(NollehTokens)

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

find_package(eosio.cdt)

add_subdirectory(contracts)