#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <overkill/Config.hpp>
#include <overkill/MaterialSystem.hpp>
#include <PMS/logger.h>

namespace overkill 
{
    class Input;
}

class overkill::Input
{
public:
    static float fovy;          // The camera's field of view on Y axis. Used for zoom.
    static float cursorY;       // CursorX and Y is 0 to window size.
    static float cursorX;
    static float mouseX;        // MouseX and Y can be bigger than the window size.
    static float mouseY;
    static bool leftButtonDown;


    static void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
    static void OnCursorHover(GLFWwindow* window, double x, double y);
    static void OnScrollChange(GLFWwindow* window, double x, double y);
    static void OnLeftClick(GLFWwindow* window, int button, int action, int mods);
};
