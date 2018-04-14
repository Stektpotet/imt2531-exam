#pragma once

#include <stdio.h>

#include <GLFW/glfw3.h>
#include <overkill/Input.hpp>

namespace overkill::Input
{
    const float aspect = 1;
    
    extern float fovy;
    extern float cursorX;
    extern float cursorY;


    void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods);
    void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
    void OnCursorHover(GLFWwindow* window, double x, double y);
    void OnScrollChange(GLFWwindow* window, double x, double y);
}