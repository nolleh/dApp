# cmake_minimum_required(VERSION 3.12)

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_EXTENSIONS ON )
set( CXX_STANDARD_REQUIRED ON)

#set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fexceptions")

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling tests with TCMalloc")
    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

set(ENABLE_COVERAGE_TESTING TRUE CACHE BOOL "Build for code coverage analysis")

if(ENABLE_COVERAGE_TESTING)
  SET(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS}")
  find_program( LCOV_PATH  lcov )
  find_program( LLVMCOV_PATH  llvm-cov )
  find_program( GENHTML_PATH NAMES genhtml)
endif()

find_package(LLVM 4.0 REQUIRED CONFIG)

link_directories(${LLVM_LIBRARY_DIR})

find_package(eosio REQUIRED)

# Settings the Boost Library 
#set(Boost_USE_STATIC_LIBS ON) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.67 REQUIRED COMPONENTS
    unit_test_framework)

enable_testing()

file(GLOB UNIT_TESTS "*.cpp")

include_directories(${CMAKE_BINARY_DIR}/contracts)
link_libraries(${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_eosio_test(eosio.token2 ${UNIT_TESTS})

## tests
set(Coverage_NAME ${PROJECT_NAME}_ut_coverage)

if(NOT LCOV_PATH)
    message(FATAL_ERROR "lcov not found! Aborting...")
endif() # NOT LCOV_PATH

# if(NOT LLVMCOV_PATH)
#     message(FATAL_ERROR "llvm-cov not found! Aborting...")
# endif() # NOT LCOV_PATH

# if(NOT GENHTML_PATH)
#     message(FATAL_ERROR "genhtml not found! Aborting...")
# endif() # NOT GENHTML_PATH

# no spaces allowed within tests list
# set(ctest_tests 'eosio.token_binaryen|eosio.token_wavm')
set(ctest_tests 'eosio.token2_binaryen')
set(ctest_exclude_tests '')

message("target:" ${Coverage_NAME})
# Setup target
add_custom_target(${Coverage_NAME} ALL

    # Cleanup lcov
    COMMAND ${LCOV_PATH} --directory . --zerocounters

    # Run tests
    COMMAND ${PROJECT_BINARY_DIR}/tools/ctestwrapper.sh -R ${ctest_tests} -E ${ctest_exclude_tests}
    COMMAND ${LCOV_PATH} --directory . --capture --gcov-tool ${CMAKE_SOURCE_DIR}/tools/llvm-gcov.sh --output-file ${Coverage_NAME}.info
    COMMAND ${LCOV_PATH} -remove ${Coverage_NAME}.info '*/boost/*' '/usr/lib/*' '/usr/include/*' '*/externals/*' '*/fc/*' '*/wasm-jit/*' --output-file ${Coverage_NAME}_filtered.info
    COMMAND ${GENHTML_PATH} -o ${Coverage_NAME} ${PROJECT_BINARY_DIR}/tests/${Coverage_NAME}_filtered.info
    COMMAND if [ "$CI" != "true" ]\; then ${CMAKE_COMMAND} -E remove ${Coverage_NAME}.base ${Coverage_NAME}.info ${Coverage_NAME}_filtered.info ${Coverage_NAME}.total ${PROJECT_BINARY_DIR}/${Coverage_NAME}.info.cleaned ${PROJECT_BINARY_DIR}/${Coverage_NAME}_filtered.info.cleaned\; fi
    WORKING_DIRECTORY ${PROJECT_CURRENT_BINARY_DIR}
    COMMENT "Resetting code coverage counters to zero. Processing code coverage counters and generating report. Report published in ./${Coverage_NAME}"
)

  # Show info where to find the report
add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
    COMMAND ;
    COMMENT "Open ./${Coverage_NAME}/index.html in your browser to view the coverage report."
    )