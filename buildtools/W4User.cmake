if(NOT EMSCRIPTEN)
    message(FATAL_ERROR [=[
        W4 projects must be built with Emscripten CMake toolchain
        Use commandline tool `w4` to build project
        OR
        Pass flag `-DCMAKE_TOOLCHAIN_FILE=<Emscripten.cmake file location>` to CMake at configure step
]=])
endif ()

set(INDENT "   ")

macro(Indent)
    list(APPEND CMAKE_MESSAGE_INDENT "${INDENT}" )
endmacro()

macro(UnIndent)
    list(POP_BACK CMAKE_MESSAGE_INDENT)
endmacro()


file(TO_CMAKE_PATH "$ENV{W4}" W4ROOT)
set(W4SDK ${W4ROOT}/sdk)


include(W4EMS)
if (NOT DEFINED ENV{W4_NATIVE_TOOLS})
    set(ENV{W4_NATIVE_TOOLS} "${W4SDK}/bin")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(W4_FRAMEWORK         "${W4SDK}/lib/libw4-${CMAKE_BUILD_TYPE}.a")
set(W4_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/_out")
set(W4_CACHE_DIRECTORY   "${CMAKE_SOURCE_DIR}/.cache")
set(W4_GENERATED_DIR     "${CMAKE_BINARY_DIR}/.generated")

set(W4_BUILD_RES_SCRIPT  "${W4SDK}/buildtools/build-res.py")
set(W4_BUILD_HTM_SCRIPT  "${W4SDK}/buildtools/build-htm.py")
set(W4_TEMPLATE_HTM      "${W4SDK}/template/index.html")
set(W4_TEMPLATE_CSS      "${W4SDK}/template/index.css")


set(W4_RUNTIME_OUTPUT_DIRECTORY "${W4_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}" CACHE PATH "W4 output directory")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message("--------------------------------------------   ,---MM  ")
message("-------------------------------------------- ~(__(o.o) ")
message("--------------------------------------------  UU UU    ")
message("(c) 2018-2021 W4 Nyan-Kit SDK by Ciliz")
message("Product Folder : ${W4_RUNTIME_OUTPUT_DIRECTORY}")
message("Python         : ${Python_EXECUTABLE}")
message("Native Tools   : $ENV{W4_NATIVE_TOOLS}")
message("Resource Tool  : ${W4_BUILD_RES_SCRIPT}")
message("Html Tool      : ${W4_BUILD_HTM_SCRIPT}")
message("Default Htm    : ${W4_TEMPLATE_HTM}")
message("Default Css    : ${W4_TEMPLATE_CSS}")

include(W4EMS)

add_library(w4 INTERFACE)

target_include_directories(w4 INTERFACE "${W4SDK}/include" "${W4SDK}/include/external")
target_link_options(w4 INTERFACE "SHELL:--js-library \"${W4SDK}/w4.js\"")
target_link_libraries(w4 INTERFACE ${W4_FRAMEWORK})


function(W4DeclareWebApp APP_PATH)
Indent()
    get_filename_component(APP_NAME "${APP_PATH}" NAME)
    message("> Build Application ${APP_NAME} -> ${CMAKE_BUILD_TYPE}")

    set(APP_GENERATED_DIR "${W4_GENERATED_DIR}/${APP_NAME}")
    set(APP_TEMPORARY_DIR "${CMAKE_BINARY_DIR}/.tmp/${APP_NAME}")

    add_executable(${APP_NAME})
    target_link_libraries(${APP_NAME} PRIVATE w4)
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_GENERATED_DIR}" OUTPUT_NAME ${APP_NAME})

    target_include_directories(${APP_NAME} PUBLIC
        "${APP_PATH}"
        "${APP_PATH}/interface"
        "${APP_PATH}/include"
        "${APP_PATH}/sources"
    )

    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${APP_PATH}/sources/*.c"
        "${APP_PATH}/sources/*.cpp"
        "${APP_PATH}/*.h"
        "${APP_PATH}/*.hpp"
        "${APP_PATH}/*.inl"
    )

    target_sources(${APP_NAME} PRIVATE "${SOURCES}")
    set_target_properties(${APP_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")
    file(GLOB SOURCES_JS CONFIGURE_DEPENDS "${APP_PATH}/js/*.js")

    foreach(SOURCE_JS IN LISTS SOURCES_JS)
        message("- ${SOURCE_JS}")
        target_link_options(${APP_NAME} PUBLIC "SHELL:--js-library \"${SOURCE_JS}\"")
        set_property(TARGET ${APP_NAME} APPEND PROPERTY LINK_DEPENDS "${SOURCE_JS}")
        set_property(TARGET ${APP_NAME} APPEND PROPERTY INTERFACE_LINK_DEPENDS "${SOURCE_JS}")
    endforeach ()

    file(GLOB HTM_FILES CONFIGURE_DEPENDS "${APP_PATH}/*.html")
    if(NOT HTM_FILES)
        set(HTM_FILES ${W4_TEMPLATE_HTM})
    endif()

    add_custom_command(
            TARGET ${APP_NAME} PRE_BUILD
            COMMAND "${CMAKE_COMMAND}" -E env "W4_NATIVE_TOOLS=$ENV{W4_NATIVE_TOOLS}"
            "${Python_EXECUTABLE}" "${W4_BUILD_RES_SCRIPT}"
            "${APP_PATH}"                                            # project dir
            "${APP_TEMPORARY_DIR}"                                   # temporary directory
            $<TARGET_PROPERTY:${APP_NAME},RUNTIME_OUTPUT_DIRECTORY>  # build directory
            "${W4_RUNTIME_OUTPUT_DIRECTORY}"                         # deploy directory
            "${W4_CACHE_DIRECTORY}/${APP_NAME}"                      # cache directory
            "${CMAKE_BUILD_TYPE}"                                    # build type
            $<$<CONFIG:Shipping>:"--bundled">                        # bundled if shipping
            $<$<CONFIG:ShippingMinSize>:"--bundled">                 #
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            COMMENT "> Build resources for ${APP_NAME}..."

    )

#####################

    foreach(SOURCE_HTM IN LISTS HTM_FILES)
        get_filename_component(BASE_HTM_NAME ${SOURCE_HTM} NAME_WE)
        get_filename_component(BASE_HTM_DIR  ${SOURCE_HTM} DIRECTORY)

        set(DESTINATION_HTM ${W4_RUNTIME_OUTPUT_DIRECTORY}/${APP_NAME}/${BASE_HTM_NAME}.html)

        set(SOURCE_CSS ${BASE_HTM_DIR}/${BASE_HTM_NAME}.css)
        set(SOURCE_PAK ${BASE_HTM_DIR}/${BASE_HTM_NAME}.pack)
        if(NOT EXISTS "${SOURCE_CSS}")
            set(SOURCE_CSS ${W4_TEMPLATE_CSS})
        endif()

        message("- ${DESTINATION_HTM}")
        message("   + ${SOURCE_HTM}")
        message("   + ${SOURCE_CSS}")

        if(EXISTS "${SOURCE_PAK}")
            message("   + ${SOURCE_PAK}")
            set_property(TARGET ${APP_NAME} APPEND PROPERTY LINK_DEPENDS "${SOURCE_PAK}")
        endif()

        set_property(TARGET ${APP_NAME} APPEND PROPERTY LINK_DEPENDS "${SOURCE_HTM}")
        set_property(TARGET ${APP_NAME} APPEND PROPERTY LINK_DEPENDS "${SOURCE_CSS}")

        add_custom_command(
            TARGET ${APP_NAME} POST_BUILD
            DEPENDS ${HTM_FILES} ${CSS_FILE} ${W4_BUILD_HTM_SCRIPT}
            COMMAND
                "${CMAKE_COMMAND}" -E env "W4_NATIVE_TOOLS=$ENV{W4_NATIVE_TOOLS}"
                "${Python_EXECUTABLE}" "${W4_BUILD_HTM_SCRIPT}"
                "${APP_PATH}"                                           # project directory
                "${APP_TEMPORARY_DIR}"                                  # temporary dir
                $<TARGET_PROPERTY:${APP_NAME},RUNTIME_OUTPUT_DIRECTORY> # build directory
                "${W4_RUNTIME_OUTPUT_DIRECTORY}"                        # deploy directory
                "${SOURCE_HTM}"                                         # input html tpl
                "${DESTINATION_HTM}"                                    # destination html
                "${SOURCE_CSS}"                                         # css file
                "${SOURCE_PAK}"                                         # inject file
                $<$<CONFIG:Shipping>:"--bundled">                       # bundled if shipping
                $<$<CONFIG:ShippingMinSize>:"--bundled">                #
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            COMMENT "> Build ${HTM_NAME} file for ${APP_NAME}..."
        )
    endforeach ()




endfunction()
