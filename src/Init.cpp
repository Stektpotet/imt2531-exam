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
    if (auto[key, GLVersionMajor, err] = p.keyInteger("GLVersionMajor"); err) {
        return;
    } else {
        C::GLVersionMajor = GLVersionMajor;
        LOG_INFO("%s: %d", std::string(key).data(), C::GLVersionMajor);
    }

    // int   C::GLVersionMinor
    if (auto[key, GLVersionMinor, err] = p.keyInteger("GLVersionMinor"); err) {
        return;
    } else {
        C::GLVersionMinor = GLVersionMinor;
        LOG_INFO("%s: %d",std::string(key).data(), C::GLVersionMinor);        
    }


    //
    // WINDOW CONFIG
    //

    // int   C::WinWidth
    if (auto[key, WinWidth, err] = p.keyInteger("WinWidth"); err) {
        return;
    } else {
        C::WinWidth = WinWidth;
        LOG_INFO("%s: %d",std::string(key).data(), C::WinWidth);                
    }

    // int   C::WinHeight 
    if (auto[key, WinHeight, err] = p.keyInteger("WinHeight"); err) {
        return;
    } else {
        C::WinHeight = WinHeight;
        LOG_INFO("%s: %d",std::string(key).data(), C::WinHeight);                
    }

    // char  C::WinName[MaxString]
    if (auto[key, WinName, err] = p.keyString("WinName"); err) {
        return;
    } else {
        strcpy(C::WinName, std::string(WinName).data());
        LOG_INFO("%s: %s",std::string(key).data(), C::WinName);                    
    }       


    // glm::vec4 C::ClearColor
    if (auto[key, ClearColor, err] = p.keyVec4("ClearColor"); err) {
        return;
    } else {
        C::ClearColor = ClearColor;
        LOG_INFO("%s: %f %f %f %f",std::string(key).data(), C::ClearColor.r, C::ClearColor.g, C::ClearColor.b, C::ClearColor.a );                            
    }
    



    //
    // CAMERA CONFIG
    //


    // float C::FOV           
    if (auto[key, FOV, err] = p.keyFloat("FOV"); err) {
        return;
    } else {
        C::FOV = FOV;
        LOG_INFO("%s: %f",std::string(key).data(), C::FOV);
    }       

    // float C::MaxFOV   
    if (auto[key, MaxFOV, err] = p.keyFloat("MaxFOV"); err) {
        return;
    } else {
        C::MaxFOV = MaxFOV;
        LOG_INFO("%s: %f",std::string(key).data(), C::MaxFOV);        
    }              
    // float C::MinFOV 
    if (auto[key, MinFOV, err] = p.keyFloat("MinFOV"); err) {
        return;
    } else {
        C::MinFOV = MinFOV;
        LOG_INFO("%s: %f",std::string(key).data(), C::MinFOV );        
        
    }                
    // float C::ZoomSensitivity 
    if (auto[key, ZoomSensitivity, err] = p.keyFloat("ZoomSensitivity"); err) {
        return;
    } else {
        C::ZoomSensitivity = ZoomSensitivity;
        LOG_INFO("%s: %f",std::string(key).data(), C::ZoomSensitivity);                
    }       
    // float C::LookSensitivity 
    if (auto[key, LookSensitivity, err] = p.keyFloat("LookSensitivity"); err) {
        return;
    } else {
        C::LookSensitivity = LookSensitivity;
        LOG_INFO("%s: %f",std::string(key).data(), C::LookSensitivity);                        
    }       
    // float C::PanSensitivity  
    if (auto[key, PanSensitivity, err] = p.keyFloat("PanSensitivity"); err) {
        return;
    } else {
        C::PanSensitivity = PanSensitivity;
        LOG_INFO("%s: %f",std::string(key).data(), C::PanSensitivity);                                
    }       
    // float C::NearClip   
    if (auto[key, NearClip, err] = p.keyFloat("NearClip"); err) {
        return;
    } else {
        C::NearClip = NearClip;
        LOG_INFO("%s: %f",std::string(key).data(), C::NearClip);                                        
    }            
    // float C::FarClip      
    if (auto[key, FarClip, err] = p.keyFloat("FarClip"); err) {
        return;
    } else {
        C::FarClip = FarClip;
        LOG_INFO("%s: %f",std::string(key).data(), C::FarClip);                                                
    }          
    // float C::CameraOffset    
    if (auto[key, CameraOffset, err] = p.keyFloat("CameraOffset"); err) {
        return;
    } else {
        C::CameraOffset = CameraOffset;
        LOG_INFO("%s: %f",std::string(key).data(), C::CameraOffset);                                                        
    }


    //
    // ASSETS CONFIG
    //


    // char C::PythonPath
    if (auto[key, PythonPath, err] = p.keyString("PythonPath"); err) {
        return;
    } else {
        C::PythonPath = PythonPath;
        LOG_INFO("%s: %s",std::string(key).data(), C::PythonPath.data());                                                                
    }       
 

    // char C::TexturesFolder[MaxString]   
    if (auto[key, TexturesFolder, err] = p.keyString("TexturesFolder"); err) {
        return;
    } else {
        strcpy(C::TexturesFolder, std::string(TexturesFolder).data());
        LOG_INFO("%s: %s",std::string(key).data(), C::TexturesFolder);                                                                
    }       
 

    // char C::ShadersFolder[MaxString] 
    if (auto[key, ShadersFolder, err] = p.keyString("ShadersFolder"); err) {
        return;
    } else {
        strcpy(C::ShadersFolder, std::string(ShadersFolder).data());
        LOG_INFO("%s: %s",std::string(key).data(), C::ShadersFolder);                                                                
        
    }       

    // char C::MaterialsFolder[MaxString] 
    if (auto[key, MaterialsFolder, err] = p.keyString("MaterialsFolder"); err) {
        return;
    } else {
        strcpy(C::MaterialsFolder, std::string(MaterialsFolder).data());
        LOG_INFO("%s: %s",std::string(key).data(), C::MaterialsFolder);
    }       

    // char C::ModelsFolder[MaxString]   
    if (auto[key, ModelsFolder, err] = p.keyString("ModelsFolder"); err) {
        return;
    } else {
        strcpy(C::ModelsFolder, std::string(ModelsFolder).data());
        LOG_INFO("%s: %s",std::string(key).data(), C::ModelsFolder);
    }       

    // char C::ScenesFolder[MaxString]    
    if (auto[key, ScenesFolder, err] = p.keyString("ScenesFolder"); err) {
        return;
    } else {
        strcpy(C::ScenesFolder, std::string(ScenesFolder).data());
        LOG_INFO("%s: %s",std::string(key).data(), C::ScenesFolder);
    }       
 
    // char C::ObjsFolder[MaxString]    
    if (auto[key, ObjsFolder, err] = p.keyString("ObjsFolder"); err) {
        return;
    } else {
        strcpy(C::ObjsFolder, std::string(ObjsFolder).data());
        LOG_INFO("%s: %s",std::string(key).data(), C::ObjsFolder);
    }       
 


    //
    // SHADERS CONFIG
    //
    // unsigned int C::MAX_LIGHTS
    if (auto[key, MAX_LIGHTS, err] = p.keyInteger("MAX_LIGHTS"); err) {
        return;
    } else {
        C::MAX_LIGHTS = unsigned(MAX_LIGHTS);
        LOG_INFO("%s: %u",std::string(key).data(), C::MAX_LIGHTS);                                                                                        
    }
}




void Init::loadOBJFiles(bool loadTextures, bool loadMaterials, bool loadModels) 
{
    auto getBaseDir = [](const std::string& filepath) -> std::string 
    {
        if (filepath.find_last_of("/\\") != std::string::npos) {
            return filepath.substr(0, filepath.find_last_of("/\\")+1);
        }
        return ".";
    };


    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "objs");

    LOG_DEBUG("Loading objs! count: %lu", static_cast<unsigned long>(fevents.size()));
    for (auto e: fevents)
    {   
        std::string objFilepath = C::ObjsFolder + ("/" + e.tag + "/" + e.tag + "." + e.extension);
        std::string objBasedir  = getBaseDir(objFilepath);
        LOG_DEBUG("Obj from file: %s", objFilepath.data());


        auto attributes = tinyobj::attrib_t{};
        auto shapes       = std::vector<tinyobj::shape_t>{};
        auto materials    = std::vector<tinyobj::material_t>{};
        auto err          = std::string{};


        auto success = tinyobj::LoadObj(
            &attributes,
            &shapes, 
            &materials, 
            &err, 
            objFilepath.c_str(), 
            objBasedir.c_str() );

        if (!err.empty()) {
            LOG_ERROR("ERR: %s\n", err.c_str());
        }

        if (!success) {
            LOG_ERROR("!success");
        }


        if (loadTextures) 
        {        
            if (auto err = TextureSystem::loadOBJ(materials, objBasedir, e.tag); err) {
                LOG_ERROR("TextureSystem failed to load OBJ: %s", objFilepath.data());
            }
        }
        
        if(loadMaterials)
        {
            if (auto err = MaterialSystem::loadOBJ(materials, e.tag); err) {
                LOG_ERROR("MaterialSystem failed to load OBJ: %s", objFilepath.data());
            }
        }
        
        if (loadModels) 
        {
            if (auto err = ModelSystem::loadOBJ(attributes, shapes, materials, e.tag); err) {
                LOG_ERROR("ModelSystem failed to load OBJ: %s", objFilepath.data());
            }
        }
    }

}

} // ::overkill
