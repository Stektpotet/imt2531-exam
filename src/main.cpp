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
    Init::OpenGL(C::ClearColor); //(0.05f, 0.06f, 0.075f, 1.0f) for sexy dark blue-grey
    Watcher::pollEvents();

    // Load resource subsystem
    TextureSystem::load();
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();

    auto renderer = Renderer();
    //translation * view * rotate(time*0.1*F, time*0.333334*F, time*0.1666666667*F) //From shader, old system.


    struct LightData {
        glm::vec4 position;		//16->16
        glm::vec4 intensities;	//16->32
  //      float spread;			//4 ->36
		//float constant;			//4	->40
		//float linear;			//4 ->44
		//float quadratic;		//4 ->48

    } lightData[8] = {
        LightData{ { 0, 4, 0, 0	},{ 1.5f, 0, 0	 , 0}, /*0.0f, 0.0f, 0.0f, 0.0f    */ },        
		LightData{ { 1.5f, 0, 0, 0 },{ 0, 0, 1.5f	 , 0 },/* 0.0f, 0.0f, 0.0f, 0.0f   */     },
        LightData{ { 3, 1, 0, 0	},{ 0, 1.5f, 0		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0, -2, 0,0	},{ 1, 0, 0		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0, 1, -4,0	},{ 1, 0, 1		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0, -5, 0,0	},{ 1, 1, 0		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0, 5, 0,0	},{ 1, 1, 1		 , 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
        LightData{ { 0,3, 3	,0	},{ 0.5, 0.5, 0.5, 0}, /*10.0f, 10.0f, 10.0f, 10.0f*/ },
    };

    glm::mat4 projection = glm::perspective(C::FOV, C::AspectRatio, C::NearClip, C::FarClip);
    glm::mat4 camera = glm::mat4(1);
    glm::mat4 pivot = glm::translate(glm::mat4(1), glm::vec3(0, 0, C::CameraOffset));  //Camera pos in world.
    glm::mat4 view = glm::mat4(1);
    // TODO remove if compiles.
    // glm::mat4 m2w = modelTransform.modelToWorld();


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
    lightBuf.update(restOfTheLights, 32 * 5, &(lightData[3]));



    for(;;)
    {
        t = (float)glfwGetTime();
        dt = t - oldT;
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        Renderer::clear();
        Scene::update(dt);
        
        
        // UPDATE CAMERA DATA
        projection = glm::perspective(Input::m_fovy, C::AspectRatio, 0.1f, -100.0f);
        camera = glm::rotate(glm::mat4(1), (Input::m_camRotX), glm::vec3(0.0f, 1.0f, 0.0f));
        camera = glm::rotate(glm::mat4(1), (Input::m_camRotY), glm::vec3(1.0f, 0.0f, 0.0f)) * camera;
        pivot = glm::translate(glm::mat4(1), glm::vec3(Input::m_camPanX, Input::m_camPanY, C::CameraOffset));  
        view = pivot * camera;
        glm::inverse(view); //To reverse both axis, so controls are not reverse.


        // UPDATE LIGHT DATA
        lightData[0].position = glm::vec4(10 * sin(3 * t), (sin(0.666f*t) + 2.0f), 10 * cos(3 * t), 0);
        //lightData[0].intensities = glm::vec4(0.85f *(sin(0.33*t)*0.5f+1), 0.85f * (cos(0.33*t)*0.5f + 1), 0, 0);

        lightData[1].position = glm::vec4(10 * sin(1.33*t), (sin(0.999f*t) + 2.0f), 10 * cos(1.33*t), 0);
        //lightData[1].intensities = glm::vec4(0.85f * (sin(1.33*t + C::PI / 3)*0.5f+1), 0, 0.85f * (cos(1.33*t + C::PI / 3)*0.5f + 1), 0);

        lightData[2].position = glm::vec4(10 * sin(4.377*t), (sin(0.333f*t)+2.0f), 10 * cos(4.377*t), 0);
        //lightData[2].intensities = glm::vec4(0, 0.85f * (cos(1.33*t + (C::PI * 2 / 3))*0.5f + 1), 0.85f * (sin(1.33*t + (C::PI * 2 / 3))*0.5f + 1), 0);

        // UPDATE GLOBAL UNIFORM BUFFERS
        lightBuf.update(light0PosIndex, 32, &(lightData[0]));
        
        lightBuf.update(light1PosIndex, 32, &(lightData[1]));
        
        lightBuf.update(light2PosIndex, 32, &(lightData[2]));

        matrixBuf.update(projectionIndex, sizeof(glm::mat4), glm::value_ptr(projection));
        matrixBuf.update(viewIndex, sizeof(glm::mat4), glm::value_ptr(view));
        matrixBuf.update(viewPosIndex, sizeof(glm::vec4), glm::value_ptr(pivot));

        Scene::draw(t);      // Draws all the models in the scene.

        glfwSwapBuffers(window);
        glfwPollEvents();

        oldT = t;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
