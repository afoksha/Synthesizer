add_executable (msynth main.cpp glad.c)

add_custom_command(TARGET msynth POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/glsl ${CMAKE_CURRENT_BINARY_DIR}/glsl)

target_link_libraries(msynth LINK_PUBLIC glfw3 x11 xrandr xi xxf86vm gl)
