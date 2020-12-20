#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# framework static library
#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
add_library (framework STATIC shader.cpp uniform.cpp dsa_uniform.cpp shader_program.cpp utils.cpp log.cpp 
                              imgui_window.cpp glfw_window.cpp stb_image.cpp stb_image_write.cpp)
target_link_libraries(framework imgui glfw libglew_static)
