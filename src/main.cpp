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


int main(int argc, char** args)
{
    if (argc > 2)
    {
        LOG_ERROR("No more than 0 or 1 parameters. Parameter is path to scene file to load. Example: ./cube assets/scenes/_default.yml");
    }

    Init::loadConfig();

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

    // Load resource subsystem
    TextureSystem::load();
    ShaderSystem::createUniformBuffers();
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();

    
    LOG_DEBUG("Argc %d, args[1] %s", argc, args[1]);

    if (argc == 1)
    {
        Scene::load("assets/scenes/_default.yml");
    }
    else if (argc == 2)
    {
        Scene::load(std::string(args[1]));
    }


    //exit(0); // for performance analysis
    float oldT = 0, t = 0, dt = 0;


    struct PointLight {
        glm::vec4 position;     //16 -> 16
        glm::vec4 intensities;  //16 -> 32
        float constant;         //4  -> 36
        float linear;           //4  -> 40
        float quadratic;        //4  -> 44
        float alignment;        //4  -> 48
    } lightData[8] = {
        PointLight{ { -15, 2,  10, 0 }, { 8, 0, 0, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
        PointLight{ {  0,  2,  10, 0 }, { 0, 8, 0, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
        PointLight{ {  15, 2,  10, 0 }, { 0, 0, 8, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
        PointLight{ {  0, -2,  0,  0 }, { 0, 0, 0, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
        PointLight{ {  0,  1, -4,  0 }, { 0, 0, 0, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
        PointLight{ {  0, -5,  0,  0 }, { 0, 0, 0, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
        PointLight{ {  0,  5,  0,  0 }, { 0, 0, 0, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
        PointLight{ {  0,  3,  3,  0 }, { 0, 0, 0, 0 }, 1.0f, 0.03125f, 0.0625f, 0 },
    };

    struct DirectionalLight {
        glm::vec4 direction;    //16->16
        glm::vec4 intensities;  //16->32
    } sun = DirectionalLight { 
        -glm::vec4{ 10, 9, 8, 7 },
        { 1.0f, 0.756862745f, 0.552941176f,0.0f } 
    };

	auto matrixBuf        = ShaderSystem::getUniformBufferByTag("OK_Matrices");
	auto lightBuf         = ShaderSystem::getUniformBufferByTag("OK_Lights");
    auto projectionIndex  = matrixBuf.getUniformIndex("projection");
    auto pointlightIndex  = lightBuf.getUniformIndex("light[0].position"); 
    auto sunIndex         = lightBuf.getUniformIndex("sun.direction");

    //push data on the nondynamic area of the light uBuffer
    //lightBuf.update(restOfTheLights, 32 * 5, &(lightData[3]));
    
    lightBuf.update(pointlightIndex, sizeof(PointLight) * C::MAX_LIGHTS, lightData);
    lightBuf.update(sunIndex,        sizeof(DirectionalLight), &(sun));


    for(;;)
    {
        t = (float)glfwGetTime();
        dt = t - oldT;
        if ((glfwGetKey(C::window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(C::window) != 0))
            break;

        Renderer::clear();
        Scene::update(dt);
        {
            lightBuf.update(pointlightIndex, sizeof(PointLight) * C::MAX_LIGHTS, lightData);
        }

        // UPDATE CAMERA MATRICES
        if (auto camera = Scene::getActiveCamera(); camera != nullptr)
        {
            CameraTransform cameraTransform = camera -> m_cameraTransform;
            //CameraTransform cameraTransform = ((EntityCamera*)Scene::getEntity(cameraID))-> m_cameraTransform;
            matrixBuf.update(projectionIndex, sizeof(CameraTransform), &cameraTransform);
        }
        else 
        {
            LOG_WARN("Main camera not set. The scene most likely loaded incorrectly.")
        }
        

        // Draws all the models in the scene.
        Scene::draw(t);     

        //This can be done to debug positions
        //glm::mat4 camPosDebugM2W = glm::translate(glm::mat4(1), glm::vec3(cameraTransform.position));
        //Renderer::draw(ModelSystem::getById(0), camPosDebugM2W, t);

        glfwSwapBuffers(C::window);
        glfwPollEvents();

        oldT = t;
    }

    Scene::clean(); 
    glfwDestroyWindow(C::window);
    glfwTerminate();
	return 0;
}
