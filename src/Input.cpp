#include <overkill/Input.hpp>

namespace overkill
{
    bool Input::m_leftButtonDown = false;
    bool Input::m_rightButtonDown = false;
    
    float Input::m_fovy = C::FOV;
    float Input::m_cursorX = 0;
    float Input::m_cursorY = 0;
    float Input::m_camRotX = 0;
    float Input::m_camRotY = 0;
    float Input::m_camPanX = 0;
    float Input::m_camPanY = 0;

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

        //Camera rotation:
        if (m_leftButtonDown) //Click and drag to rotate.
        {
            m_camRotX += x - m_cursorX;
            m_camRotY += y - m_cursorY;

            if (m_camRotX / C::WinWidth > C::PI/2) m_camRotX = 0;
            if (m_camRotX < 0) m_camRotX = C::WinWidth * C::PI/2;

            if (m_camRotY / C::WinHeight < -0.4f) m_camRotY = C::WinHeight * -0.4f;   // Lock Y axis on camera to 90deg up, and down,
            if (m_camRotY / C::WinHeight > 0.4f)  m_camRotY = C::WinHeight * 0.4f;    // no looping the camera around.

            printf("camRotX:%f,  \tcamRotY:%f \tcamRotX / C::WinWidth:%f   \tcamRotY / C::WinWidth:%f\n",
                    m_camRotX,     m_camRotY,     m_camRotX / C::WinWidth,       m_camRotY / C::WinHeight);
        }

        //Camera paning:
        if (m_rightButtonDown) //Click and drag to rotate.
        {
            m_camPanX += ((x - m_cursorX) / C::WinWidth * C::PanSensitivity) * ((m_fovy - C::MinFOV + 0.06f) / ((C::MaxFOV - C::MinFOV)  * 0.6f));
            m_camPanY += ((y - m_cursorY) / C::WinHeight * C::PanSensitivity) * ((m_fovy - C::MinFOV + 0.06f) / ((C::MaxFOV - C::MinFOV)  * 0.6f)) * -1;

            printf("camPanX:%f,  \tcamPanY:%f \tcamPanX / C::WinWidth:%f   \tcamPanY / C::WinWidth:%f\n",
                    m_camPanX,     m_camPanY,     m_camPanX / C::WinWidth,       m_camPanY / C::WinHeight);
        }

        m_cursorX = x;  // Save current cursor pos.
        m_cursorY = y;  // important to do this last since the logic to rotate and pan
    }                   // uses the difference between now and last mouse pos.

    void Input::OnScrollChange(GLFWwindow* window, double x, double y)
    {
        // fovy += (y / 512) + 32;
        m_fovy -= y * C::ZoomSensitivity;
        if (m_fovy < C::MinFOV) m_fovy = C::MinFOV;
        if (m_fovy > C::MaxFOV) m_fovy = C::MaxFOV;

        printf("Scroll: x: %f,\ty:%f\t\tfovy:%f\n", x, y, m_fovy);
    }

    void Input::OnMouseClick(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                printf("Left button pressed.\n");
                m_leftButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {
                printf("Left button released.\n");       
                m_leftButtonDown = false;
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                printf("Right button pressed.\n");
                m_rightButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {
                printf("Right button released.\n");       
                m_rightButtonDown = false;
            }
        }
    }
}