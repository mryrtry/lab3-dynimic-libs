if (WIN32)
    set(PLUGIN_EXTENSION "dll")
else()
    set(PLUGIN_EXTENSION "so")
endif()

set(PLUGINS_TRANSFORM_BUILD_DIR "${CMAKE_BINARY_DIR}/solution/plugins_build/transform")

set(PLUGINS_FORMAT_BUILD_DIR "${CMAKE_BINARY_DIR}/solution/plugins_build/format")

file(MAKE_DIRECTORY ${PLUGINS_BUILD_DIR})

include_directories(${CMAKE_SOURCE_DIR}/plugins/include ${CMAKE_SOURCE_DIR}plugins/src)

function(add_transform_plugin plugin_name)
    set(PLUGIN_SOURCE "${CMAKE_SOURCE_DIR}/plugins/src/transform/${plugin_name}.c")
    add_library(${plugin_name} SHARED ${PLUGIN_SOURCE} ${CMAKE_SOURCE_DIR}/plugins/src/transform_utils.c ${CMAKE_SOURCE_DIR}/plugins/src/image.c)
    set_target_properties(${plugin_name} PROPERTIES
            PREFIX ""
            SUFFIX ".${PLUGIN_EXTENSION}"
            LIBRARY_OUTPUT_DIRECTORY ${PLUGINS_TRANSFORM_BUILD_DIR}
            RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_TRANSFORM_BUILD_DIR}
    )
endfunction()

function(add_format_plugin plugin_name)
    set(PLUGIN_SOURCE "${CMAKE_SOURCE_DIR}/plugins/src/format/${plugin_name}.c")
    add_library(${plugin_name} SHARED ${PLUGIN_SOURCE} ${CMAKE_SOURCE_DIR}/plugins/src/image.c)
    set_target_properties(${plugin_name} PROPERTIES
            PREFIX ""
            SUFFIX ".${PLUGIN_EXTENSION}"
            LIBRARY_OUTPUT_DIRECTORY ${PLUGINS_FORMAT_BUILD_DIR}
            RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_FORMAT_BUILD_DIR}
    )
endfunction()

file(GLOB TRANSFORM_PLUGINS "${CMAKE_SOURCE_DIR}/plugins/src/transform/*.c")
foreach(TRANSFORM_PLUGIN ${TRANSFORM_PLUGINS})
    get_filename_component(PLUGIN_NAME ${TRANSFORM_PLUGIN} NAME_WE)
    add_transform_plugin(${PLUGIN_NAME})
endforeach()

file(GLOB FORMAT_PLUGINS "${CMAKE_SOURCE_DIR}/plugins/src/format/*.c")
foreach(FORMAT_PLUGIN ${FORMAT_PLUGINS})
    get_filename_component(PLUGIN_NAME ${FORMAT_PLUGIN} NAME_WE)
    add_format_plugin(${PLUGIN_NAME})
endforeach()


