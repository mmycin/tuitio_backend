# -------------------------
# httplib
# -------------------------
add_library(httplib INTERFACE)
target_include_directories(httplib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)

# -------------------------
# AES
# -------------------------
set(AES_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(AES_CPP_ENABLE_AESNI ON CACHE BOOL "" FORCE)
add_subdirectory(
    thirdparty/aes-cpp
    ${CMAKE_BINARY_DIR}/aes-cpp
)
target_compile_options(aes_cpp PRIVATE
    -maes
    -mpclmul
    -msse2
    -mssse3
)

# -------------------------
# dotenv-cpp
# -------------------------
add_library(dotenv INTERFACE)
target_include_directories(dotenv
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dotenv-cpp/include/laserpants/dotenv
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dotenv-cpp/include
)

# -------------------------
# nlohmann/json
# -------------------------
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/json/single_include
)


# -------------------------
# Project utilities
# -------------------------
add_library(utils STATIC
    "utils/utils.cpp"
)

target_include_directories(utils
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/utils"
)

target_link_libraries(utils
    PUBLIC
        dotenv
)

# -------------------------
# SQLiteCpp
# -------------------------
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/SQLiteCpp)

# -------------------------
# spdlog
# -------------------------
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
add_subdirectory(
    thirdparty/spdlog
    ${CMAKE_BINARY_DIR}/spdlog
)

# -------------------------
# CLI11
# -------------------------
set(CLI11LOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(CLI11LOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CLI11LOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
add_subdirectory(
    thirdparty/CLI11
    ${CMAKE_BINARY_DIR}/CLI11
)

# -------------------------
# Monocypher
# -------------------------
add_library(monocypher STATIC
    thirdparty/monocypher/monocypher.c
)
target_include_directories(monocypher PUBLIC
    thirdparty/monocypher
)
target_link_libraries(utils PUBLIC monocypher)

# -------------------------
# Everything Bundle
# -------------------------
add_library(cweb_dependencies INTERFACE)

target_link_libraries(cweb_dependencies
    INTERFACE
        utils
        httplib
        nlohmann_json
        dotenv
        SQLiteCpp
        aes_cpp::aes_cpp
        spdlog::spdlog
        CLI11::CLI11
        monocypher
)