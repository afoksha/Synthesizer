#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# framework static library
#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
add_library (framework STATIC shader.cpp uniform.cpp dsa_uniform.cpp shader_program.cpp utils.cpp log.cpp 
                              imgui_window.cpp glfw_window.cpp stb_image.cpp stb_image_write.cpp)
target_link_libraries(framework imgui glfw libglew_static)

add_executable (msynth main.cpp)

add_custom_command(TARGET msynth POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/glsl ${CMAKE_CURRENT_BINARY_DIR}/glsl)

target_link_libraries(msynth LINK_PUBLIC framework glfw libglew_static)
