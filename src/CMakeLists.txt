add_executable (msynth main.cpp shader.cpp uniform.cpp dsa_uniform.cpp shader_program.cpp utils.cpp log.cpp)

add_custom_command(TARGET msynth POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/glsl ${CMAKE_CURRENT_BINARY_DIR}/glsl)

target_link_libraries(msynth LINK_PUBLIC glfw libglew_static)
