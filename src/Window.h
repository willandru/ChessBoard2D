#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool isValid() const;
    bool shouldClose() const;
    void pollEvents() const;
    void swapBuffers() const;
    void processInput() const;
    bool isMouseButtonPressed(int button) const;
    void cursorPosition(double& x, double& y) const;
    void framebufferSize(int& width, int& height) const;

private:
    GLFWwindow* handle_ = nullptr;
};
