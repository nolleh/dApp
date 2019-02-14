# cmake_minimum_required(VERSION 3.12)

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_EXTENSIONS ON )
set( CXX_STANDARD_REQUIRED ON)

#set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fexceptions")

# find_package( Gperftools QUIET )
# if( GPERFTOOLS_FOUND )
#     message( STATUS "Found gperftools; compiling tests with TCMalloc")
#     list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
# endif()

find_package(eosio REQUIRED)

# Settings the Boost Library 
#set(Boost_USE_STATIC_LIBS ON) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.67 REQUIRED COMPONENTS
    unit_test_framework)


file(GLOB UNIT_TESTS "*.cpp")

include_directories(${CMAKE_BINARY_DIR}/contracts)
link_libraries(${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_eosio_test(eosio.token ${UNIT_TESTS})
