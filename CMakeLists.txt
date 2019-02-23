cmake_minimum_required(VERSION 3.5)

project(eosio.token2)

# 이 옵션은 eosio-cpp 에서는 조회하지 않는다. 따라서 켜지 않는다.
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules")

include(wasm)

# set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wshadow -Werror -Wno-error=dangling-else -fexceptions")
add_subdirectory(contracts)
add_subdirectory(tests)
add_subdirectory(tools)