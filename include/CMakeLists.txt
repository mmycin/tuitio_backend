# -------------------------
# httplib
# -------------------------

add_library(httplib INTERFACE)

target_include_directories(httplib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)


# -------------------------
# dotenv-cpp
# -------------------------

add_library(dotenv INTERFACE)

target_include_directories(dotenv
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dotenv-cpp/include/laserpants/dotenv
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

add_library(utils INTERFACE)

target_include_directories(utils
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
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
# Everything
# -------------------------

add_library(cweb_dependencies INTERFACE)

target_link_libraries(cweb_dependencies
    INTERFACE
        utils
        httplib
        nlohmann_json
        dotenv
        SQLiteCpp
)
