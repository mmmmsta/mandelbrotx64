cmake_minimum_required(VERSION 3.0.0)
project(mandelbrotx64 VERSION 0.1.0)

include(CTest)
enable_testing()



add_subdirectory("${PROJECT_SOURCE_DIR}/glfw-3.3.5" "${PROJECT_SOURCE_DIR}/GLFW/bin" EXCLUDE_FROM_ALL)




add_executable(${PROJECT_NAME} main.cpp window.cpp shader.h window.h abstract.h GLAD/src/glad.c GLAD/include/glad/glad.h)


target_include_directories(${PROJECT_NAME}  PRIVATE GLAD/include)

target_link_libraries(${PROJECT_NAME}  glfw)





set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
