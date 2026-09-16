add_executable(test
    test.cpp
)

target_include_directories(test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(test
    PRIVATE
        cweb_dependencies
)

