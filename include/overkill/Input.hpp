#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <overkill/Scene.hpp>
#include <overkill/Config.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/ModelSystem.hpp>
#include <overkill/Scene.hpp>
#include <PMS/logger.h>

namespace overkill 
{

enum NavigationKey
{
    W, A, S, D, Q, E
};

class Input
{
private:
    static bool m_leftButtonDown;
    static bool m_rightButtonDown;


public:
    static float m_fovy;          // The camera's field of view on Y axis. Used for zoom.
    static float m_cursorY;       // CursorX and Y is 0 to window size.
    static float m_cursorX;
    static float m_camRotX;        // Factor camera uses for its rotation/orientation.
    static float m_camRotY;
    static float m_camPanX;        // Factor camera uses for its position/orientation.
    static float m_camPanY;
    static bool m_navKeyPressed[6];// Which navigation keys are pressed. WASD-QE keys.

    static void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
    static void OnCursorHover(GLFWwindow* window, double x, double y);
    static void OnScrollChange(GLFWwindow* window, double x, double y);
    static void OnMouseClick(GLFWwindow* window, int button, int action, int mods);
};

}