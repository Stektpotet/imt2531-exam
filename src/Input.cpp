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
        LOG_DEBUG("Pressing %i, as char: %c\n", keyCode, char(keyCode));
        if (keyCode == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, 1);
        }
        else if(keyCode == GLFW_KEY_1) {
            MaterialSystem::reload();
        }
        else if(keyCode == GLFW_KEY_2) {

        }
        else if(keyCode == GLFW_KEY_3) {

        }
        else if(keyCode == GLFW_KEY_4) {

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

            if (mouseX / C::WinWidth > C::PI/2) mouseX = 0;
            if (mouseX < 0) mouseX = C::WinWidth * C::PI/2;

            if (mouseY / C::WinHeight < -0.4f) mouseY = C::WinHeight * -0.4f;   // Lock Y axis on camera to 90deg up, and down,
            if (mouseY / C::WinHeight > 0.4f)  mouseY = C::WinHeight * 0.4f;    // no looping the camera around.

            printf("mouseX:%f,  \tmouseY:%f \tmouseX / C::WinWidth:%f   \tmouseY / C::WinWidth:%f\n",
                    mouseX,     mouseY,     mouseX / C::WinWidth,       mouseY / C::WinHeight);
        }
        cursorX = x;
        cursorY = y;
    }

    void Input::OnScrollChange(GLFWwindow* window, double x, double y)
    {
        // fovy += (y / 512) + 32;
        fovy -= y * C::ZoomSensitivity;
        if (fovy < C::MinFOV) fovy = C::MinFOV;
        if (fovy > C::MaxFOV) fovy = C::MaxFOV;

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