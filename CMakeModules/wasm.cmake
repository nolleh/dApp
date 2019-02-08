# for unittest, originated from eosio/eos, modify little to compatible eosio.cdt by nolleh

macro(add_unittest_header_files TARGET DESTINATION_FOLDER)
    set (target ${TARGET})
    
    # add_custom_command(OUTPUT ${DESTINATION_FOLDER}/${target}.wasm
    #     DEPENDS ${target}.wast
    #     COMMAND $<TARGET_FILE:eosio-wast2wasm> ${DESTINATION_FOLDER}/${target}.wast ${DESTINATION_FOLDER}/${target}.wasm -n
    #     COMMENT "Generating WASM ${target}.wasm"
    #     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #     VERBATIM
    # )
    # set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${target}.wasm)

    # add_custom_command(OUTPUT ${DESTINATION_FOLDER}/${target}.wast.hpp
    #     DEPENDS ${DESTINATION_FOLDER}/${target}.wast
    #     COMMAND echo "const char* const ${TARGET_VARIABLE}_wast = R\"=====("  > ${DESTINATION_FOLDER}/${target}.wast.hpp
    #     COMMAND cat ${DESTINATION_FOLDER}/${target}.wast >> ${DESTINATION_FOLDER}/${target}.wast.hpp
    #     COMMAND echo ")=====\";"  >> ${DESTINATION_FOLDER}/${target}.wast.hpp
    #     COMMENT "Generating ${target}.wast.hpp"
    #     VERBATIM
    # )

    if (EXISTS ${DESTINATION_FOLDER}/${target}.abi)
        add_custom_command(TARGET ${TARGET}.wasm
            POST_BUILD
            DEPENDS ${DESTINATION_FOLDER}/${target}.abi
            COMMAND echo "const char* const ${TARGET_VARIABLE}_abi = R\"=====("  > ${DESTINATION_FOLDER}/${target}.abi.hpp
            COMMAND cat ${DESTINATION_FOLDER}/${target}.abi >> ${DESTINATION_FOLDER}/${target}.abi.hpp
            COMMAND echo ")=====\";"  >> ${DESTINATION_FOLDER}/${target}.abi.hpp
            COMMENT "Generating ${target}.abi.hpp"
            VERBATIM
        )
        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${target}.abi.hpp)
        set(extra_target_dependency   ${DESTINATION_FOLDER}/${target}.abi.hpp)
    else()
        message("failed to find abi files in " ${DESTINATION_FOLDER}/${target}.abi)
    endif()

endmacro(add_unittest_header_files)