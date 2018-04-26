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

    void Input::OnInputKeyPress(GLFWwindow* window, int keyCode, int /*scanCode*/, int /*mods*/)
    {
        LOG_DEBUG("Pressing %i, as char: %c\n", keyCode, char(keyCode));
        if (keyCode == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, 1);
        }

        // PRESS 2 TO RELOAD ShaderSystem
        else if(keyCode == GLFW_KEY_1) {
            ShaderSystem::reload();
        }
        // PRESS 1 TO RELOAD MaterialSystem
        else if(keyCode == GLFW_KEY_2) {
            MaterialSystem::reload();
        }
        // PRESS 3 TO RELOAD ModelSystem
        else if(keyCode == GLFW_KEY_3) {
            ModelSystem::reload();
        }
        else if(keyCode == GLFW_KEY_4) {

        }

// float newPosX = offset *  sinf( yawRadian ) * cosf( pitchRadian );
// float newPosY = offset * -sinf( pitchRadian );
// float newPosZ = offset *  cosf( yawRadian ) * cosf( pitchRadian );

        Entity* camera = Scene::getEntityByTag("camera");
        glm::vec3 rot = glm::radians(camera-> getRotation());
        if (char(keyCode) == 'W')
        {
            camera -> setPosition(camera -> getPosition() + glm::vec3(
                                                C::PanSensitivity * -glm::sin(rot.y) * glm::cos(rot.x),
                                                C::PanSensitivity * glm::sin(rot.x), 
                                                C::PanSensitivity * glm::cos(rot.y) * glm::cos(rot.x)));
        }
        if (char(keyCode) == 'S')
        {
            camera -> setPosition(camera -> getPosition() + glm::vec3(
                                                C::PanSensitivity * glm::sin(rot.y) * glm::cos(rot.x),
                                                C::PanSensitivity * -glm::sin(rot.x), 
                                                C::PanSensitivity * -glm::cos(rot.y) * glm::cos(rot.x)));
        }
        if (char(keyCode) == 'D')
        {
            camera -> setPosition(camera -> getPosition() + glm::vec3(
                                                C::PanSensitivity * glm::cos(rot.y) * glm::cos(rot.x),
                                                0, 
                                                C::PanSensitivity * glm::sin(rot.y) * glm::cos(rot.x)));        
                                                }
        if (char(keyCode) == 'A')
        {
            camera -> setPosition(camera -> getPosition() + glm::vec3(
                                                -C::PanSensitivity * glm::cos(rot.y) * glm::cos(rot.x),
                                                0, 
                                                -C::PanSensitivity * glm::sin(rot.y) * glm::cos(rot.x)));

        }
        if (char(keyCode) == 'Q')
        {
            camera -> setPosition(camera -> getPosition() + glm::vec3(0, C::PanSensitivity, 0));
        }
        if (char(keyCode) == 'E')
        {
            camera -> setPosition(camera -> getPosition() + glm::vec3(0, -C::PanSensitivity, 0));
        }
    }

    void Input::OnInputKeyHold(GLFWwindow* /*window*/, int keyCode, int /*scanCode*/, int /*mods*/)
    {
        printf("Holding %i, as char: %c\n", keyCode, char(keyCode));
    }

    void Input::OnInputKeyUnpress(GLFWwindow* /*window*/, int keyCode, int /*scanCode*/, int /*mods*/)
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

    void Input::OnCursorHover(GLFWwindow* /*window*/, double x, double y)
    {
        float deltaX = x - m_cursorX;
        float deltaY= y - m_cursorY;

        //Camera rotation:
        if (m_leftButtonDown) //Click and drag to rotate.
        {
            EntityCamera* camera = (EntityCamera*) Scene::getEntityByTag("camera");
            glm::vec3 rot = camera-> getRotation();
            camera-> setRotation(rot + glm::vec3((-deltaY), -deltaX, 0) * C::LookSensitivity);
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
    }                   // uses the difference between now sand last mouse pos.

    void Input::OnScrollChange(GLFWwindow* /*window*/, double x, double y)
    {
       printf("Scroll: x: %f,\ty:%f\t\tfovy:%f\n", x, y, m_fovy);
    }

    void Input::OnMouseClick(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
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