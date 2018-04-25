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

#define DEBUG 1

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
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();


    struct LightData {
        glm::vec4 position;		//16->16
        glm::vec4 intensities;	//16->32
		float constant;			//4	->40
		float linear;			//4 ->44
		float quadratic;		//4 ->48

    } lightData[8] = {
        LightData{ { -15, 2, 10, 0 },{ 5, 0, 0, 0},  1.0f, 0.03125f, 0.0625f },
		LightData{ { 0,   2, 10, 0 },{ 0, 5, 0, 0 }, 1.0f, 0.03125f, 0.0625f },
        LightData{ { 15,  2, 10, 0 },{ 0, 0, 5, 0},  1.0f, 0.03125f, 0.0625f },
        LightData{ { 0, -2, 0,0	},{ 1, 0, 0		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0, 1, -4,0	},{ 1, 0, 1		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0, -5, 0,0	},{ 1, 1, 0		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0, 5, 0,0	},{ 1, 1, 1		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0,3, 3	,0	},{ 0.5, 0.5, 0.5, 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
    };

    Scene::loadScene();

    float oldT = 0, t = 0, dt = 0;

	auto matrixBuf = ShaderSystem::getUniformBufferByTag("OK_Matrices");
	auto lightBuf = ShaderSystem::getUniformBufferByTag("OK_Lights");

    // Get Uniform buffer indicies
    auto projectionIndex = matrixBuf.getUniformIndex("projection");
    auto viewIndex       = matrixBuf.getUniformIndex("view");
    auto viewPosIndex    = matrixBuf.getUniformIndex("view_position");

    auto light0PosIndex  = lightBuf.getUniformIndex("light[0].position"); 
    auto light1PosIndex  = lightBuf.getUniformIndex("light[1].position"); 
    auto light2PosIndex  = lightBuf.getUniformIndex("light[2].position"); 
    auto restOfTheLights = lightBuf.getUniformIndex("light[3].position");

    //push data on the nondynamic area of the light uBuffer
    //lightBuf.update(restOfTheLights, 32 * 5, &(lightData[3]));

    lightBuf.update(light0PosIndex, sizeof(LightData), &(lightData[0]));
    lightBuf.update(light1PosIndex, sizeof(LightData), &(lightData[1]));
    lightBuf.update(light2PosIndex, sizeof(LightData), &(lightData[2]));

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
            lightData[0].position = glm::vec4(-15, 2, 20 * cos(0.999f * t), 0);
            //lightData[0].intensities = glm::vec4(0.85f *(sin(0.33*t)*0.5f+1), 0.85f * (cos(0.33*t)*0.5f + 1), 0, 0);

            lightData[1].position = glm::vec4(0, 2, 20 * cos(1.333f*t), 0);
            //lightData[1].intensities = glm::vec4(0.85f * (sin(1.33*t + C::PI / 3)*0.5f+1), 0, 0.85f * (cos(1.33*t + C::PI / 3)*0.5f + 1), 0);

            lightData[2].position = glm::vec4(15, 2, 20 * cos(1.666*t), 0);
            //lightData[2].intensities = glm::vec4(0, 0.85f * (cos(1.33*t + (C::PI * 2 / 3))*0.5f + 1), 0.85f * (sin(1.33*t + (C::PI * 2 / 3))*0.5f + 1), 0);

            // UPDATE GLOBAL UNIFORM BUFFERS
        
            lightBuf.update(light0PosIndex, 16, &(lightData[0]));
            lightBuf.update(light1PosIndex, 16, &(lightData[1]));
            lightBuf.update(light2PosIndex, 16, &(lightData[2]));
        }

        // UPDATE CAMERA MATRICES
        {
            CameraTransform cameraTransform = ((EntityCamera*)Scene::getEntityByTag("camera"))-> m_cameraTransform;
            matrixBuf.update(projectionIndex, sizeof(glm::mat4), glm::value_ptr(cameraTransform.projectionMatrix));
            matrixBuf.update(viewIndex, sizeof(glm::mat4), glm::value_ptr(cameraTransform.viewMatrix));
        }

        // Draws all the models in the scene.
        Scene::draw(t);     

        glfwSwapBuffers(window);
        glfwPollEvents();

        oldT = t;
    }

    Scene::clean(); 
    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
