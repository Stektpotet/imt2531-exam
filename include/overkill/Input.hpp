#pragma once

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <overkill/Config.hpp>

namespace overkill 
{
    class Input;
}

class overkill::Input
{
public:
    static float fovy;
    static float cursorY;
    static float cursorX;


    static void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
    static void OnCursorHover(GLFWwindow* window, double x, double y);
    static void OnScrollChange(GLFWwindow* window, double x, double y);
};
