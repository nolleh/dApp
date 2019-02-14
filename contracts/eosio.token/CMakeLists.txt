cmake_minimum_required(VERSION 3.5)
project(eosio.token VERSION 1.0.0)

find_package(eosio.cdt)

add_contract(eosio.token eosio.token eosio.token.cpp)

# abi 파일이 항상 CURRENT_BINARY 에서 생성되므로.
add_unittest_header_files(eosio.token ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
