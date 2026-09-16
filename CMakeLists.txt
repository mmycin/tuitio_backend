cmake_minimum_required(VERSION 3.20)

project(CWeb LANGUAGES CXX)

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

# Exclude standalone executable source files like seeders
list(FILTER SERVER_SOURCES EXCLUDE REGEX
    ".*/database/seeders/seeder\\.cpp$"
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
# Database Seeder Executable
# -----------------------------------------------------------------------------
add_executable(seeder
    src/database/seeders/seeder.cpp
)

target_include_directories(seeder
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(seeder
    PRIVATE
        cweb_dependencies
)

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
add_subdirectory(tests)

