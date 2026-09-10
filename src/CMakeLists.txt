add_subdirectory(database)
add_subdirectory(models)

file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(server
    ${SERVER_SOURCES}
)

target_include_directories(server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/database/models
)

target_link_libraries(server
    PRIVATE
        cweb_dependencies
        cweb_models
)
