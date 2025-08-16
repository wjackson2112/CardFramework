file(GLOB SHADERS *.frag *.vert)
foreach(SHADER ${SHADERS})
    file(RELATIVE_PATH FILENAME ${CMAKE_CURRENT_SOURCE_DIR} ${SHADER})
    add_custom_command(
            COMMENT "Copying ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}"
            OUTPUT "${FILENAME}"
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
    )

    string(REPLACE "/" "_" TARGET_NAME ${FILENAME})

    add_custom_target(
        cfshaders_${TARGET_NAME}
        DEPENDS ${FILENAME}
    )
    add_dependencies(CardFramework cfshaders_${TARGET_NAME})
endforeach()
