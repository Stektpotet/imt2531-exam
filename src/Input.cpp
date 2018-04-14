#include <overkill/Input.hpp>

namespace overkill
{
    extern float Input::fovy = 90;
    extern float Input::cursorX = 0;
    extern float Input::cursorY = 0;


    void Input::OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods)
    {
        printf("Pressing %i, as char: %c\n", keyCode, char(keyCode));
        if (keyCode == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, 1);
        }

    }

    void Input::OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods)
    {
        printf("Holding %i, as char: %c\n", keyCode, char(keyCode));
        if (keyCode == GLFW_KEY_W)
        {

        }
        if (keyCode == GLFW_KEY_S)
        {

        }
    }

    void Input::OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods)
    {
        printf("Unpressed %i, as char: %c\n", keyCode, char(keyCode));
    }
    
    void Input::OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
            OnInputKeyPress(window, keyCode, scanCode, mods);
            break;
        case GLFW_REPEAT:
            OnInputKeyHold(window, keyCode, scanCode, mods);
            break;
        case GLFW_RELEASE:
            OnInputKeyUnpress(window, keyCode, scanCode, mods);
            break;
        }
    }

    void Input::OnCursorHover(GLFWwindow* window, double x, double y)
    {
        cursorX = x;
        cursorY = y;
    }

    void Input::OnScrollChange(GLFWwindow* window, double x, double y)
    {
        fovy += (x / 512) + 32;
    }

}