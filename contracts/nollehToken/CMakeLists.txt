#file(GLOB ABI_FILES "*.abi")

#message("binary here : ${CMAKE_CURRENT_BINARY_DIR}")

#configure_file("${ABI_FILES}" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)

#file(GLOB HEADERS "*.hpp")
file(GLOB SRCS "*.cpp")

#get_filename_component(HEADER_NAMES ${HEADERS} NAME)
get_filename_component(SRC_NAMES ${SRCS} NAME)

#list(APPEND TOKEN_SOURCE_PATH "${HEADER_NAMES} ")
list(APPEND TOKEN_SOURCE_PATH "${SRC_NAMES}")

message(${TOKEN_SOURCE_PATH})

add_contract(nollehToken nollehToken ${TOKEN_SOURCE_PATH})