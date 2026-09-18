cmake_minimum_required(VERSION 3.20)

project(CWeb LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(include)

# -----------------------------------------------------------------------------
# Server Target
# -----------------------------------------------------------------------------
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

list(FILTER SERVER_SOURCES EXCLUDE REGEX
    ".*/cli/cli\\.cpp$"
)

add_executable(server
    ${SERVER_SOURCES}
)

target_include_directories(server
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(server
    PRIVATE
        cweb_dependencies
)

# -----------------------------------------------------------------------------
# CLI Executable (CLI11)
# -----------------------------------------------------------------------------
add_executable(cli
    src/cli/cli.cpp
)

target_include_directories(cli
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(cli
    PRIVATE
        cweb_dependencies
)

# Assuming your executable target is named 'server'
target_compile_definitions(server PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:_HAS_STD_BYTE=0>
)


# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
add_subdirectory(tests)