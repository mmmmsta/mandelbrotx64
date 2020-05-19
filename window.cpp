
#include "window.h"
#include <iostream>

GLFWwindow* initializeWindow(int* windowFlag, const unsigned int width, const unsigned int height) {
    bool fullscreen = false;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    GLFWwindow* window = glfwCreateWindow(width, height, "MB Viewer", NULL, NULL);
    int w, h;
    w = width;
    h = height;
    if (fullscreen){
        int monitorCount = 2;
        GLFWmonitor** monitorHandles = glfwGetMonitors(&monitorCount);


        GLFWmonitor* monitor = monitorHandles[0];
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        w = mode->width;
        h = mode->height;

    }

    
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        *windowFlag = -1;
        return window;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        *windowFlag = -1;
        return window;
    }

    glViewport(0, 0, w, h);
    

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    *windowFlag = 0;

    return window;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

