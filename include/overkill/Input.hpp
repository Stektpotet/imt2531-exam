#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <overkill/Scene.hpp>
#include <overkill/Config.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/ModelSystem.hpp>
#include <overkill/SeasonSystem.hpp>
#include <overkill/Scene.hpp>
#include <PMS/logger.h>

namespace overkill 
{

    enum NavigationKey
    {
        I = 0, J = 1, K = 2, L = 3, H = 4, Y = 5, //Flightcam controls
        N = 6, M = 7,
        W = 8, A = 9, S = 10, D = 11, Q = 12, E = 13, //glider controls
        MAX_KEYS = 14
};

class Input
{
private:
    static const int ASCII_MAP_OFFSET = GLFW_KEY_SPACE;
    static const int ASCII_MAP_MAX = GLFW_KEY_Z;
    static const int ASCII_MAP_SIZE = ASCII_MAP_MAX - ASCII_MAP_OFFSET;

    static bool m_asciiMap[ASCII_MAP_SIZE];

    static bool m_isCursorCaptured;

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
    static bool m_navKeyPressed[MAX_KEYS];// Which navigation keys are pressed. WASD-QE keys.
    static bool m_shift;

    static void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods);
    static void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods);
    static void OnCursorHover(GLFWwindow* window, double x, double y);
    static void OnScrollChange(GLFWwindow* window, double x, double y);
    static void OnMouseClick(GLFWwindow* window, int button, int action, int mods);

    static void clearMap();
    static bool getKey(int glfw_key);
};

}