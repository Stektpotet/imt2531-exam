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
    bool  Input::m_navKeyPressed[] = {false, false, false, false, false, false, false, false}; // Which navigation keys are pressed. WASD-QE keys. -> but now IJKL-YH
    bool  Input::m_shift{ false };

    bool  Input::m_asciiMap[Input::ASCII_MAP_SIZE]; //keyboard map from ascii value 32 -> 90

    void Input::clearMap()
    {
        for (int i = 0; i < Input::ASCII_MAP_SIZE; i++)
        {
            m_asciiMap[i] = false; //reset previous input state
        }
    }

    void Input::OnInputKeyPress(GLFWwindow* window, int keyCode, int /*scanCode*/, int mods)
    {
        // LOG_DEBUG("Pressing %i, as char: %c\n", keyCode, char(keyCode));
        int index = keyCode - Input::ASCII_MAP_OFFSET;
        if ((index < Input::ASCII_MAP_SIZE))
        {
            m_asciiMap[index] = true;
        }

        switch (keyCode)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, 1);
                break;
            case GLFW_KEY_F1:
                ShaderSystem::reload();
                break;
            case GLFW_KEY_F2:
                MaterialSystem::reload();
                Init::loadOBJFiles(false, true, false);
                break;
            case GLFW_KEY_F3:
                ModelSystem::reload();
                Init::loadOBJFiles(false, false, true);
                break;
            case GLFW_KEY_F4:
                Scene::reload();
                break;
            case GLFW_KEY_SPACE:
            {
                EntityCamera * cam = (EntityCamera*)Scene::getActiveCamera();
                if (cam != nullptr)
                {
                    cam->cycleMode();
                    LOG_INFO("Cycled camera mode.");
                }
                else
                {
                    LOG_WARN("Main camera not set. The scene most likely loaded incorrectly.")
                }
                break;
            }
            case GLFW_KEY_TAB:
                Scene::cycleCameras();
                LOG_INFO("Cycled cameras.");
                break;
           
            case GLFW_KEY_I:        
                m_navKeyPressed[I] = true;
                break;
            case GLFW_KEY_K:        
                m_navKeyPressed[K] = true;
                break;
            case GLFW_KEY_L:        
                m_navKeyPressed[L] = true;    
                break;
            case GLFW_KEY_J:        
                m_navKeyPressed[J] = true;
                break;
            case GLFW_KEY_H:        
                m_navKeyPressed[H] = true;
                break;
            case GLFW_KEY_Y:        
                m_navKeyPressed[Y] = true;
                break;
            case GLFW_KEY_N:
                m_navKeyPressed[N] = true;
                break;
            case GLFW_KEY_M:
                m_navKeyPressed[M] = true;
                break;
        }
        
    }

    void Input::OnInputKeyHold(GLFWwindow* /*window*/, int /*keyCode*/, int /*scanCode*/, int /*mods*/)
    {
        // LOG_DEBUG("Holding %i, as char: %c\n", keyCode, char(keyCode));
    }

    void Input::OnInputKeyUnpress(GLFWwindow* /*window*/, int keyCode, int /*scanCode*/, int mods)
    {
        // LOG_DEBUG("Unpressed %i, as char: %c\n", keyCode, char(keyCode));

        switch (keyCode)
        {
        case GLFW_KEY_I:
            m_navKeyPressed[I] = false;
            break;
        case GLFW_KEY_K:
            m_navKeyPressed[K] = false;
            break;
        case GLFW_KEY_L:
            m_navKeyPressed[L] = false;
            break;
        case GLFW_KEY_J:
            m_navKeyPressed[J] = false;
            break;
        case GLFW_KEY_H:
            m_navKeyPressed[H] = false;
            break;
        case GLFW_KEY_Y:
            m_navKeyPressed[Y] = false;
            break;
        case GLFW_KEY_N:
            m_navKeyPressed[N] = false;
            break;
        case GLFW_KEY_M:
            m_navKeyPressed[M] = false;
            break;
        }
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

        ////Camera rotation:
        
#ifdef __APPLE__
        if (m_leftButtonDown) //Click and drag to rotate.
        {
#endif
            EntityCamera* camera = (EntityCamera*) Scene::getActiveCamera();
            if (camera != nullptr)
            {
                glm::vec3 rot = camera-> getRotation();
                camera-> setRotation(rot + glm::vec3((-deltaY), -deltaX, 0) * C::LookSensitivity);
            }
            else 
            {
                LOG_WARN("Main camera not set. The scene most likely loaded incorrectly.")
            }
#ifdef __APPLE__
        }
#endif


        m_cursorX = x;  // Save current cursor pos.
        m_cursorY = y;  // important to do this last since the logic to rotate and pan*/
    }                   // uses the difference between now sand last mouse pos.

    void Input::OnScrollChange(GLFWwindow* /*window*/, double /*x*/, double /*y*/)
    {
    //    printf("Scroll: x: %f,\ty:%f\t\tfovy:%f\n", x, y, m_fovy);
    }

    void Input::OnMouseClick(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                // printf("Left button pressed.\n");
                m_leftButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {
                // printf("Left button released.\n");       
                m_leftButtonDown = false;
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                // printf("Right button pressed.\n");
                m_rightButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {
                // printf("Right button released.\n");       
                m_rightButtonDown = false;
            }
        }
    }
    bool Input::getKey(const int glfw_key)
    {
        int index = glfw_key - Input::ASCII_MAP_OFFSET;
        if (index > Input::ASCII_MAP_MAX)
        {
            return false;
        }
        return Input::m_asciiMap[index];
    }
 
}