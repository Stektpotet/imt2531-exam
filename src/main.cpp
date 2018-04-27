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


int main()
{
    Init::loadConfig();

    // Init libraries + Watcher callbacks
	auto window = Init::GLFW(
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

    struct DirectionalLight {
        glm::vec4 direction;    //16->16
        glm::vec4 intensities;  //16->32
    } sun = DirectionalLight{ -glm::vec4{ 10, 9, 8, 7 },{ 1.0f, 0.756862745f, 0.552941176f,0.0f } };

    struct PointLight {
        glm::vec4 position;		//16 -> 16
        glm::vec4 intensities;	//16 -> 32
		float constant;			//4	 -> 36
		float linear;			//4  -> 40
		float quadratic;		//4  -> 44
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

    Scene::loadScene();

    float oldT = 0, t = 0, dt = 0;

	auto matrixBuf = ShaderSystem::getUniformBufferByTag("OK_Matrices");
	auto lightBuf = ShaderSystem::getUniformBufferByTag("OK_Lights");

    // Get Uniform buffer indicies
    auto projectionIndex = matrixBuf.getUniformIndex("projection");


    auto light0PosIndex  = lightBuf.getUniformIndex("light[0].position"); 
    auto light1PosIndex  = lightBuf.getUniformIndex("light[1].position"); 
    auto light2PosIndex  = lightBuf.getUniformIndex("light[2].position"); 
    auto restOfTheLights = lightBuf.getUniformIndex("light[3].position");

    auto sunIndex = lightBuf.getUniformIndex("sun.direction");

    //push data on the nondynamic area of the light uBuffer
    //lightBuf.update(restOfTheLights, 32 * 5, &(lightData[3]));
    lightBuf.update(light0PosIndex, sizeof(PointLight), &(lightData[0]));
    lightBuf.update(light1PosIndex, sizeof(PointLight), &(lightData[1]));
    lightBuf.update(light2PosIndex, sizeof(PointLight), &(lightData[2]));
    lightBuf.update(restOfTheLights, sizeof(PointLight)*5, &(lightData[3]));
    lightBuf.update(sunIndex, sizeof(DirectionalLight), &(sun));

    auto cameraID = Scene::getEntityByTag("camera")->getEntityID();

    for(;;)
    {
        t = (float)glfwGetTime();
        dt = t - oldT;
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        Renderer::clear();
        Scene::update(dt);
        {
            // UPDATE LIGHT DATA
            lightData[0].position = glm::vec4(40*sin(0.999f * t), sin(t)*5.0f+ 5.0f, 40 * cos(0.999f * t), 0);
            //lightData[0].intensities = glm::vec4(0.85f *(sin(0.33*t)*0.5f+1), 0.85f * (cos(0.33*t)*0.5f + 1), 0, 0);

            lightData[1].position = glm::vec4(40*sin(1.333f*t), cos(t*2)*5.0f + 5.0f, 40 * cos(1.333f*t), 0);
            //lightData[1].intensities = glm::vec4(0.85f * (sin(1.33*t + C::PI / 3)*0.5f+1), 0, 0.85f * (cos(1.33*t + C::PI / 3)*0.5f + 1), 0);

            lightData[2].position = glm::vec4(40* sin(1.666f*t), sin(t*3)*5.0f + 5.0f, 40 * cos(1.666f*t), 0);
            //lightData[2].intensities = glm::vec4(0, 0.85f * (cos(1.33*t + (C::PI * 2 / 3))*0.5f + 1), 0.85f * (sin(1.33*t + (C::PI * 2 / 3))*0.5f + 1), 0);

            // UPDATE GLOBAL UNIFORM BUFFERS
        
            lightBuf.update(light0PosIndex, 16, &(lightData[0]));
            lightBuf.update(light1PosIndex, 16, &(lightData[1]));
            lightBuf.update(light2PosIndex, 16, &(lightData[2]));
        }

        // UPDATE CAMERA MATRICES
        {
        CameraTransform cameraTransform = ((EntityCamera*)Scene::getEntity(cameraID))->m_cameraTransform;
            //CameraTransform cameraTransform = ((EntityCamera*)Scene::getEntity(cameraID))-> m_cameraTransform;
            matrixBuf.update(projectionIndex, sizeof(CameraTransform), &cameraTransform);
        }

        // Draws all the models in the scene.
        Scene::draw(t);     

        //This can be done to debug positions
        //glm::mat4 camPosDebugM2W = glm::translate(glm::mat4(1), glm::vec3(cameraTransform.position));
        //Renderer::draw(ModelSystem::getById(0), camPosDebugM2W, t);

        glfwSwapBuffers(window);
        glfwPollEvents();

        oldT = t;
    }

    Scene::clean(); 
    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
