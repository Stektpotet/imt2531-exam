#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <overkill/Config.hpp>
#include <overkill/Init.hpp>
#include <overkill/Input.hpp>
#include <overkill/gl_caller.hpp>
#include <overkill/Renderer.hpp>
#include <overkill/Util.hpp>

#include <overkill/TextureSystem.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/Scene.hpp>
#include <overkill/ModelSystem.hpp>
#include <overkill/EntityModel.hpp>
#include <overkill/EntityCamera.hpp>
#include <overkill/Watcher.hpp>


using namespace overkill;


void loadOBJFiles();

int main(int argc, char** args)
{
    if (argc > 2)
    {
        LOG_ERROR("No more than 0 or 1 parameters. Parameter is path to scene file to load. Example: ./cube assets/scenes/_default.yml");
    }

    Init::Config();

    // Init libraries + Watcher callbacks
	C::window = Init::GLFW(
        C::GLVersionMajor, 
        C::GLVersionMinor, 
        C::WinWidth, 
        C::WinHeight,
        C::WinName);

    Init::GLEW();
    Init::OpenGL(C::ClearColor);
    Watcher::pollEvents();

    // Load asset subsystems
    TextureSystem::load();
    ShaderSystem::createUniformBuffers();
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();
    
    loadOBJFiles();

    
    LOG_DEBUG("Argc %d, args[1] %s", argc, args[1]);

    if (argc == 1)
    {
        Scene::load("assets/scenes/demo1.yml");
    }
    else if (argc == 2)
    {
        Scene::load(std::string(args[1]));
    }


    float oldT = 0, t = 0, dt = 0;

    for(;;)
    {
        t = (float)glfwGetTime();
        dt = t - oldT;
        if ((glfwGetKey(C::window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(C::window) != 0))
            break;

        Renderer::clear();
        Scene::update(dt);
        

        // Draws all the models in the scene.
        Scene::draw(t);     

        //This can be done to debug positions
        //glm::mat4 camPosDebugM2W = glm::translate(glm::mat4(1), glm::vec3(cameraTransform.position));
        //Renderer::draw(ModelSystem::getById(0), camPosDebugM2W, t);

        glfwSwapBuffers(C::window);
        glfwPollEvents();

        oldT = t;

        // break; // For testing load performance
    }

    Scene::clean();
    ModelSystem::clean();
    ShaderSystem::clean();
    TextureSystem::clean();

    glfwDestroyWindow(C::window);
    glfwTerminate();
	return 0;
}



void loadOBJFiles() 
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

        
        if (auto err = TextureSystem::loadOBJ(materials, objBasedir, e.tag); err) {
            LOG_ERROR("TextureSystem failed to load OBJ: %s", objFilepath.data());
        }
        
        
        if (auto err = MaterialSystem::loadOBJ(materials, e.tag); err) {
            LOG_ERROR("MaterialSystem failed to load OBJ: %s", objFilepath.data());
        }

        
        if (auto err = ModelSystem::loadOBJ(attributes, shapes, materials, e.tag); err) {
            LOG_ERROR("ModelSystem failed to load OBJ: %s", objFilepath.data());
        }

    }

}

