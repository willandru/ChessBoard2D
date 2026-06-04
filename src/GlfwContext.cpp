#include "GlfwContext.h"

#include <GLFW/glfw3.h>

#include <iostream>

GlfwContext::GlfwContext()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    valid_ = true;
}

GlfwContext::~GlfwContext()
{
    glfwTerminate();
}

bool GlfwContext::isValid() const
{
    return valid_;
}
