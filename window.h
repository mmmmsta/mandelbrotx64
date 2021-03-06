#pragma once

#pragma comment(lib, "libs/glfw3.lib")

#include "glad/glad.h"
#include "KHR/khrplatform.h"
#include "GLFW/glfw3.h"

#include <vector>



GLFWwindow* initializeWindow(int* windowFlag, const unsigned int width, const unsigned int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

