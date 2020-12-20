add_executable (msynth main.cpp)

add_custom_command(TARGET msynth POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/glsl ${CMAKE_CURRENT_BINARY_DIR}/glsl)
add_custom_command(TARGET msynth POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/wav ${CMAKE_CURRENT_BINARY_DIR}/wav)

target_link_libraries(msynth LINK_PUBLIC openal alut framework glfw libglew_static)
