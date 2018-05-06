#include <overkill/Init.hpp>

namespace overkill
{

auto Init::GLFW(
    const int openglMajor,
    const int openglMinor,
    const int wwidth,
    const int wheight,
    const char* wname) -> GLFWwindow*
{
    if (!glfwInit()){
        glfwTerminate();
        exit(-1); //(Init::logtag, "Failed to init GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wwidth, wheight, wname, NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        glfwTerminate();
        exit(-1); // LOG_ERROR(Init::logtag, "Failed to open GLFW window");
    }

#ifndef __APPLE__
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

    glfwSwapInterval(1);
    //q  glfwSetKeyCallback(window, key_callback);

    glfwSetKeyCallback(window, Input::OnInputKey); //set upp callbacks
    glfwSetCursorPosCallback(window, Input::OnCursorHover);
    glfwSetScrollCallback(window, Input::OnScrollChange);
    glfwSetMouseButtonCallback(window, Input::OnMouseClick);

    return window;
}

void Init::GLEW()
{
#ifndef WIN32
    glewExperimental = GL_TRUE;  // Intel cpu graphics support for macOS/linux
#endif

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(-1); // LOG_ERROR(Init::logtag, "Failed to init GLEW");
    }
}

void Init::OpenGL(const glm::vec4 background)
{
    GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
    GLCall(glClearColor(background.x, background.y, background.z, background.w));

    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glFrontFace(GL_CCW)); // the direction in which faces are defined (determins back/front): GL_CCW | GL_CW
    GLCall(glCullFace(GL_BACK));// the face side to cull away: GL_FRONT | GL_BACK | GL_FRONT_AND_BACK

    GLCall(glEnable(GL_BLEND));
    //GLCall(glEnable(GL_DEPTH_TEST)); //enabled to avoid ugly artifacts that depend on the angle of view and drawing order
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLPrintMaxConstants(); 
}




void Init::Config() 
{
    auto filestring = Util::fileToString(C::Configpath);
    Parser p(filestring);


    LOG_INFO("Loading %s", C::Configpath)

    //
    // OPENGL CONFIG
    //


    // int   C::GLVersionMajor 
    if (auto[key, GLVersionMajor, err] = p.nextKeyInteger(); err) {
        LOG_ERROR("config.yml error on key --> %s...", key.data());
        return;
    } else {
        C::GLVersionMajor = GLVersionMajor;
        LOG_INFO("%s: %d", std::string(key), C::GLVersionMajor);
    }

    // int   C::GLVersionMinor
    if (auto[key, GLVersionMinor, err] = p.nextKeyInteger(); err) {
        LOG_ERROR("config.yml error on key --> %s...", key.data());
        return;
    } else {
        C::GLVersionMinor = GLVersionMinor;
        LOG_INFO("%s: %d",key.data(), C::GLVersionMinor);        
    }


    //
    // WINDOW CONFIG
    //

    // int   C::WinWidth     
    if (auto[key, WinWidth, err] = p.nextKeyInteger(); err) {
        LOG_ERROR("config.yml error on key --> %s...", key.data());
        return;
    } else {
        C::WinWidth = WinWidth;
        LOG_INFO("%s: %d",key.data(), C::WinWidth);                
    }

    // int   C::WinHeight 
    if (auto[key, WinHeight, err] = p.nextKeyInteger(); err) {
        LOG_ERROR("config.yml error on key --> %s...", key.data());
        return;
    } else {
        C::WinHeight = WinHeight;
        LOG_INFO("%s: %d",key.data(), C::WinHeight);                
    }

    // char  C::WinName[MaxString]    
    if (auto[key, WinName, err] = p.nextKeyString(); err) {
        LOG_ERROR("config.yml error on key --> %s...", key.data());
        return;
    } else {
        strcpy(C::WinName, WinName.data());
        LOG_INFO("%s: %s",key.data(), C::WinName);                    
    }       


    // glm::vec4 C::ClearColor  
    if (auto[key, ClearColor, err] = p.nextKeyColor(); err) {
        LOG_ERROR("config.yml error on key --> %s...", key.data());
        return;
    } else {
        C::ClearColor = ClearColor;
        LOG_INFO("%s: %f %f %f %f",key.data(), C::ClearColor.r, C::ClearColor.g, C::ClearColor.b, C::ClearColor.a );                            
    }
    



    //
    // CAMERA CONFIG
    //


    // float C::FOV           
    if (auto[key, FOV, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on FOV key --> %s...", key.data());
        return;
    } else {
        C::FOV = FOV;
        LOG_INFO("%s: %f",key.data(), C::FOV);
    }       

    // float C::MaxFOV   
    if (auto[key, MaxFOV, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on MAXFOV key --> %s...", key.data());
        return;
    } else {
        C::MaxFOV = MaxFOV;
        LOG_INFO("%s: %f",key.data(), C::MaxFOV);        
    }              
    // float C::MinFOV 
    if (auto[key, MinFOV, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on MINFOV key --> %s...", key.data());
        return;
    } else {
        C::MinFOV = MinFOV;
        LOG_INFO("%s: %f",key.data(), C::MinFOV );        
        
    }                
    // float C::ZoomSensitivity 
    if (auto[key, ZoomSensitivity, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on zoomsensitivity key --> %s...", key.data());
        return;
    } else {
        C::ZoomSensitivity = ZoomSensitivity;
        LOG_INFO("%s: %f",key.data(), C::ZoomSensitivity);                
    }       
    // float C::LookSensitivity 
    if (auto[key, LookSensitivity, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on lookSensitivity key --> %s...", key.data());
        return;
    } else {
        C::LookSensitivity = LookSensitivity;
        LOG_INFO("%s: %f",key.data(), C::LookSensitivity);                        
    }       
    // float C::PanSensitivity  
    if (auto[key, PanSensitivity, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on panSensitivity key --> %s...", key.data());
        return;
    } else {
        C::PanSensitivity = PanSensitivity;
        LOG_INFO("%s: %f",key.data(), C::PanSensitivity);                                
    }       
    // float C::NearClip   
    if (auto[key, NearClip, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on NearClip key --> %s...", key.data());
        return;
    } else {
        C::NearClip = NearClip;
        LOG_INFO("%s: %f",key.data(), C::NearClip);                                        
    }            
    // float C::FarClip      
    if (auto[key, FarClip, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on Farclip key --> %s...", key.data());
        return;
    } else {
        C::FarClip = FarClip;
        LOG_INFO("%s: %f",key.data(), C::FarClip);                                                
    }          
    // float C::CameraOffset    
    if (auto[key, CameraOffset, err] = p.nextKeyFloat(); err) {
        LOG_ERROR("config.yml error on CameraOffset key --> %s...", key.data());
        return;
    } else {
        C::CameraOffset = CameraOffset;
        LOG_INFO("%s: %f",key.data(), C::CameraOffset);                                                        
    }


    //
    // ASSETS CONFIG
    //


    // char C::PythonPath  
    if (auto[key, PythonPath, err] = p.nextKeyString(); err) {
        LOG_ERROR("config.yml error on key --> %s...", key.data());
        return;
    } else {
        C::PythonPath = PythonPath;
        LOG_INFO("%s: %s",key.data(), C::PythonPath.data());                                                                
    }       
 

    // char C::TexturesFolder[MaxString]   
    if (auto[key, TexturesFolder, err] = p.nextKeyString(); err) {
        LOG_ERROR("config.yml error on Texture folder key --> %s...", key.data());
        return;
    } else {
        strcpy(C::TexturesFolder, std::string(TexturesFolder).data());
        LOG_INFO("%s: %s",key.data(), C::TexturesFolder);                                                                
    }       
 

    // char C::ShadersFolder[MaxString] 
    if (auto[key, ShadersFolder, err] = p.nextKeyString(); err) {
        LOG_ERROR("config.yml error on shaderFolder key --> %s...", key.data());
        return;
    } else {
        strcpy(C::ShadersFolder, std::string(ShadersFolder).data());
        LOG_INFO("%s: %s",key.data(), C::ShadersFolder);                                                                
        
    }       

    // char C::MaterialsFolder[MaxString] 
    if (auto[key, MaterialsFolder, err] = p.nextKeyString(); err) {
        LOG_ERROR("config.yml error on Materials folder key --> %s...", key.data());
        return;
    } else {
        strcpy(C::MaterialsFolder, std::string(MaterialsFolder).data());
        LOG_INFO("%s: %s",key.data(), C::MaterialsFolder);                                                                        
    }       

    // char C::ModelsFolder[MaxString]   
    if (auto[key, ModelsFolder, err] = p.nextKeyString(); err) {
        LOG_ERROR("config.yml error on models folder  key --> %s...", key.data());
        return;
    } else {
        strcpy(C::ModelsFolder, std::string(ModelsFolder).data());
        LOG_INFO("%s: %s",key.data(), C::ModelsFolder);                                                                                
    }       

    // char C::ScenesFolder[MaxString]    
    if (auto[key, ScenesFolder, err] = p.nextKeyString(); err) {
        LOG_ERROR("config.yml error on scenes folder key --> %s...", key.data());
        return;
    } else {
        strcpy(C::ScenesFolder, std::string(ScenesFolder).data());
        LOG_INFO("%s: %s",key.data(), C::ScenesFolder);                                                                                        
    }       
 

    //
    // SHADERS CONFIG
    //
    // unsigned int C::MAX_LIGHTS
    if (auto[key, MAX_LIGHTS, err] = p.nextKeyInteger(); err) {
        LOG_ERROR("config.yml error maxLights on key --> %s...", key.data());
        return;
    } else {
        C::MAX_LIGHTS = unsigned(MAX_LIGHTS);
        LOG_INFO("%s: %u",key.data(), C::MAX_LIGHTS);                                                                                        
    }
}


} // ::overkill
