add_executable(seeder
    ${CMAKE_CURRENT_SOURCE_DIR}/seeder.cpp
)

target_include_directories(seeder
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/thirdparty/dotenv-cpp/include/laserpants/dotenv
        ${CMAKE_SOURCE_DIR}/include/thirdparty/dotenv-cpp/include
)

target_link_libraries(seeder
    PRIVATE
        cweb_dependencies
)