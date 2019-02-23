# for unittest, originated from eosio/eos, modify little to compatible eosio.cdt by nolleh

macro(add_unittest_header_files TARGET SOURCE_DIR DESTINATION_DIR)
    set(target ${TARGET})
    string(REPLACE "." "_" TARGET_VARIABLE "${target}")

    # add_custom_command(TARGET ${TARGET}.wasm
    #     POST_BUILD
    #     DEPENDS ${SOURCE_DIR}/${target}.wasm
    #     COMMAND eosio-wasm2wast ${DESTINATION_DIR}/${target}.wasm > ${DESTINATION_DIR}/${target}.wast
    #     COMMENT "Generating WAST ${target}.wast"
    #     #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #     VERBATIM
    # )
    # #set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${target}.wasm)

    # add_custom_command(TARGET ${TARGET}.wasm
    #     POST_BUILD
    #     DEPENDS ${DESTINATION_DIR}/${target}.wast
    #     COMMAND echo "const char* const ${TARGET_VARIABLE}_wast = R\"=====("  > ${DESTINATION_DIR}/${target}.wast.hpp
    #     COMMAND cat ${DESTINATION_DIR}/${target}.wast >> ${DESTINATION_DIR}/${target}.wast.hpp
    #     COMMAND echo ")=====\";"  >> ${DESTINATION_DIR}/${target}.wast.hpp
    #     COMMENT "Generating ${target}.wast.hpp"
    #     VERBATIM
    # )

    add_custom_command(TARGET ${TARGET}.wasm
        POST_BUILD
        DEPENDS ${SOURCE_DIR}/${target}.abi
        COMMAND echo "const char* const ${TARGET_VARIABLE}_abi = R\"=====("  > ${DESTINATION_DIR}/${target}.abi.hpp
        COMMAND cat ${DESTINATION_DIR}/${target}.abi >> ${DESTINATION_DIR}/${target}.abi.hpp
        COMMAND echo ")=====\";"  >> ${DESTINATION_DIR}/${target}.abi.hpp
        COMMENT "Generating ${target}.abi.hpp"
        VERBATIM
    )
    #set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${target}.abi.hpp)
    #set(extra_target_dependency   ${DESTINATION_FOLDER}/${target}.abi.hpp)

endmacro(add_unittest_header_files)