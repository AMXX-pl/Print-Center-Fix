# Minimum required version of CMake for a project
cmake_minimum_required(VERSION 3.20)

# Add a library target to be built from the source files
add_library(${PROJECT_NAME} SHARED
    # Headers
    "src/amxx_appmod.h"
    "src/appmod.h"
    "src/tramp_hook.h"

    # Sources
    "src/amxx_appmod.cpp"
    "src/appmod.cpp"
    "src/main.cpp"
    "src/tramp_hook.cpp"
)

# Add include directories to a target
target_include_directories(${PROJECT_NAME} PRIVATE
    "include"
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_SOURCE_DIR}/libs/funchook/include"
)

#-------------------------------------------------------------------------------------------
# Metamod configuration
#-------------------------------------------------------------------------------------------

# Uncomment the functions you want to use in your code and specify the desired function names
#set(META_INIT "OnMetaInit")        # void OnMetaInit()
#set(META_QUERY "OnMetaQuery")      # void OnMetaQuery()
set(META_ATTACH "OnMetaAttach")     # MetaStatus OnMetaAttach()
set(META_DETACH "OnMetaDetach")     # void OnMetaDetach()

#-------------------------------------------------------------------------------------------
# Add dependencies to the build
#-------------------------------------------------------------------------------------------

add_subdirectory("${CMAKE_SOURCE_DIR}/libs/funchook" "funchook")
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/cssdk" "cssdk")
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/metamod" "metamod")
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/core" "core")

DisableCodeAnalysis("distorm")
DisableCodeAnalysis("funchook-static")

if(MSVC)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options("distorm" PRIVATE -fsplit-lto-unit)
        target_compile_options("funchook-static" PRIVATE -fsplit-lto-unit)
    endif()
    target_compile_options("distorm" PRIVATE /W0)
    target_compile_options("funchook-static" PRIVATE /W0)
else()
    target_compile_options("distorm" PRIVATE -w)
    target_compile_options("funchook-static" PRIVATE -w)
endif()

set_target_properties("distorm" "funchook-static" PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    "funchook-static" "cssdk" "metamod" "core"
)

#-------------------------------------------------------------------------------------------
# Publish build artifacts
#-------------------------------------------------------------------------------------------

if(PUBLISH_ARTIFACTS)
    set(GITHUB_ENV "artifacts_zip_name=${INFO_PROJECT_NAME}_v${CMAKE_PROJECT_VERSION}_${CMAKE_SYSTEM_NAME}")
    string(REGEX REPLACE "[ \t\r\n]" "" GITHUB_ENV "${GITHUB_ENV}")
    file(WRITE "$ENV{PUBLISH_DIR}/github.env" "${GITHUB_ENV}")
endif()
