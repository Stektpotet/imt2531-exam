#include <overkill/Input.hpp>

namespace overkill
{
    float Input::fovy = C::FOV;
    float Input::cursorX = 0;
    float Input::cursorY = 0;
    float Input::mouseX = 0;
    float Input::mouseY = 0;
    bool Input::leftButtonDown = false;

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
        if (leftButtonDown) //Click and drag to rotate.
        {
            mouseX += x - cursorX;
            mouseY += y - cursorY;
        }
        cursorX = x;
        cursorY = y;
    }

    void Input::OnScrollChange(GLFWwindow* window, double x, double y)
    {
        // fovy += (y / 512) + 32;
        fovy -= y * C::ZoomSensitivity;
        printf("Scroll: x: %f,\ty:%f\t\tfovy:%f\n", x, y, fovy);
    }

    void Input::OnLeftClick(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            printf("Left button pressed.\n");
            leftButtonDown = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            printf("Left button released.\n");       
            leftButtonDown = false;
        }
    }
}