cmake_minimum_required(VERSION 3.5)
# project(eosio.token2 VERSION 1.0.0)

find_package(eosio.cdt)
add_contract(eosio.token2 eosio.token2 eosio.token2.cpp)

# abi 파일이 항상 CURRENT_BINARY 에서 생성되므로.
add_unittest_header_files(eosio.token2 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})


# message( "standard-----------------" ${STANDARD_INCLUDE_FOLDERS})
# file(GLOB ABI_FILES "*.abi")
# configure_file("${ABI_FILES}" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
# add_wast_executable(TARGET eosio.token2
#   INCLUDE_FOLDERS "${STANDARD_INCLUDE_FOLDERS}"
#   LIBRARIES libc++ libc eosiolib
#   DESTINATION_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
# )
