#include "Window.h"

#include <glad/glad.h>

#include <iostream>

namespace
{
void framebufferSizeCallback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}
}

Window::Window(int width, int height, const char* title)
{
    handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!handle_)
    {
        std::cerr << "Failed to create GLFW window\n";
        return;
    }

    glfwMakeContextCurrent(handle_);
    glfwSetFramebufferSizeCallback(handle_, framebufferSizeCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
        return;
    }

    glViewport(0, 0, width, height);
}

Window::~Window()
{
    if (handle_)
    {
        glfwDestroyWindow(handle_);
    }
}

bool Window::isValid() const
{
    return handle_ != nullptr;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(handle_);
}

void Window::pollEvents() const
{
    glfwPollEvents();
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(handle_);
}

void Window::processInput() const
{
    if (glfwGetKey(handle_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(handle_, GLFW_TRUE);
    }
}

bool Window::isMouseButtonPressed(int button) const
{
    return glfwGetMouseButton(handle_, button) == GLFW_PRESS;
}

void Window::cursorPosition(double& x, double& y) const
{
    glfwGetCursorPos(handle_, &x, &y);
}

void Window::framebufferSize(int& width, int& height) const
{
    glfwGetFramebufferSize(handle_, &width, &height);
}
