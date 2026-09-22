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
# # 
# add_custom_command(TARGET test POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<TARGET_FILE:redis++>
#         $<TARGET_FILE_DIR:test>

#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<TARGET_FILE:hiredis>
#         $<TARGET_FILE_DIR:test>
# )

