cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project (LearnOpenGL)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# header-only glm library
#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
message("-------------------------------------------------------------------------------------------------------------------------------------")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/glm/cmake/glm)
add_subdirectory(glm)
include_directories(glm)

#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# build GLFW3-Library
#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
message("-------------------------------------------------------------------------------------------------------------------------------------")
add_subdirectory(glfw)
include_directories(glfw/include)
if (GLFW_BUILD_SHARED_LIBS)
    message("Linking with shared GLFW Library (GLFW_BUILD_SHARED_LIBS:ON)")
else()
    message("Linking with static GLFW Library (GLFW_BUILD_SHARED_LIBS:OFF)")
endif()

#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# build GLEW-Library
#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
message("-------------------------------------------------------------------------------------------------------------------------------------")
set(glew-cmake_BUILD_SHARED ON)
add_subdirectory(glew)
include_directories(glew/include)
if (GLEW_BUILD_STATIC)
    message("Linking with static GLEW Library (GLEW_BUILD_STATIC:ON)")
    add_definitions (-DGLEW_STATIC)
else()
    message("Linking with shared GLEW Library (GLEW_BUILD_STATIC:OFF)")
endif()

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory (src)
