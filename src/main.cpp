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

#define TINYFILES_IMPLEMENTATION

#include <overkill/Config.hpp>
#include <overkill/Init.hpp>
#include <overkill/Input.hpp>
#include <overkill/gl_caller.hpp>
#include <overkill/Renderer.hpp>
#include <overkill/Util.hpp>

#include <overkill/TextureSystem.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/ModelSystem.hpp>
#include <overkill/Entity.hpp>


#define DEBUG 1

using namespace overkill;


int main()
{

    auto window = Init::GLFW(
        C::VersionMajor,
        C::VersionMinor,
        C::WinWidth,
        C::WinHeight,
        C::WinName);

    // Init libraries + Watcher callbacks
    Init::GLEW();
    Init::OpenGL(C::ClearColor); //(0.05f, 0.06f, 0.075f, 1.0f) for sexy dark blue-grey
    Init::Watcher();

    // Load resource subsystems
    TextureSystem::load();
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();

    // Example model
    auto model = ModelSystem::getByTag("cube");
    ShaderProgram shader = model.m_meshes[0].m_shaderProgram;
    auto renderer = EdgeRenderer();

    Transform modelTransform;
    modelTransform.m_position = glm::vec3{ 0,0,0 };

    Light light;

    struct LightData {
        glm::vec4 position;		//16->16
        glm::vec4 intensities;	//16->32
        float spread;			//4 ->36
		float constant;			//4	->40
		float linear;			//4 ->44
		float quadratic;		//4 ->48

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
    glm::mat4 m2w = modelTransform.modelToWorld();


    //GLCall(glSetUn)
    GLint uniformTime;
	GLint uniformM2W;

	auto matrixBuf = ShaderSystem::getUniformBufferByTag("OK_Matrices");
	auto lightBuf = ShaderSystem::getUniformBufferByTag("OK_Lights");

	shader.bind();
	uniformM2W  = shader.getUniformLocation("m2w");
    uniformTime = shader.getUniformLocation("time");

	auto projectionIndex = matrixBuf.getUniformIndex("projection");
	auto viewIndex       = matrixBuf.getUniformIndex("view");
	auto viewPosIndex    = matrixBuf.getUniformIndex("view_position");

	auto light0PosIndex			= lightBuf.getUniformIndex("light[0].position"); //TODO make this getUniformIndex("light", 0)
	//auto light0IntensitiesIndex = lightBuf.getUniformIndex("light[0].intensities", 0);	
	//auto light0Floats = lightBuf.getUniformIndex("spread", 0);


	LOG_INFO("LIGHT_DATA_SIZE: %i", sizeof(LightData));

	auto light1PosIndex			= lightBuf.getUniformIndex("light[1].position"); // this could also be used as offset for all the other lights
    auto light2PosIndex = lightBuf.getUniformIndex("light[2].position"); // this could also be used as offset for all the other lights


	//auto light1IntensitiesIndex = lightBuf.getUniformIndex("light[1].intensities", 1);
	//auto light1Floats			= lightBuf.getUniformIndex("spread", 1);


	lightBuf.update(light0PosIndex, 16, &(lightData[0].position));
	lightBuf.update(light0PosIndex +16, 16, &(lightData[0].intensities));
	//lightBuf.update(light0Floats+4, sizeof(GLfloat),	&(lightData[0].constant));
	//lightBuf.update(light0Floats+8, sizeof(GLfloat),		&(lightData[0].linear));
	//lightBuf.update(light0Floats+12, sizeof(GLfloat),	&(lightData[0].quadratic));

	lightBuf.update(light1PosIndex, 16, &(lightData[1].position));
	lightBuf.update(light1PosIndex +16, 16, &(lightData[1].intensities));

    lightBuf.update(light2PosIndex + 16, 16, &(lightData[2].intensities));

	//lightBuf.update(light0Floats + 4, sizeof(GLfloat), &(lightData[0].constant));
	//lightBuf.update(light0Floats + 8, sizeof(GLfloat), &(lightData[0].linear));
	//lightBuf.update(light0Floats + 12, sizeof(GLfloat), &(lightData[0].quadratic));



	matrixBuf.update(projectionIndex, sizeof(glm::mat4), glm::value_ptr(projection));
	//GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
	GLCall(glUniformMatrix4fv(uniformM2W, 1, GL_FALSE, glm::value_ptr(m2w)));


    for(;;)
    {
        float t = glfwGetTime();
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        renderer.clear();
        renderer.draw(model);

		//@TODO shader.bindDynamic()
        projection = glm::perspective(Input::m_fovy, C::AspectRatio, 0.1f, -100.0f);
        camera = glm::rotate(glm::mat4(1), (C::LookSensitivity * Input::m_camRotX / C::WinWidth), glm::vec3(0.0f, 1.0f, 0.0f));
        camera = glm::rotate(glm::mat4(1), (C::LookSensitivity * Input::m_camRotY / C::WinHeight), glm::vec3(1.0f, 0.0f, 0.0f)) * camera;
        pivot = glm::translate(glm::mat4(1),glm::vec3(Input::m_camPanX, Input::m_camPanY, C::CameraOffset));  //Camera pos in world.
        
        view = pivot * camera;
        glm::inverse(view); //To reverse both axis, so controls are not reverse.
        shader.bind();
		GLCall(glUniformMatrix4fv(uniformM2W, 1, GL_FALSE, glm::value_ptr(m2w)));
        
        
        float time = (float)glfwGetTime();
        lightData[0].position = glm::vec4(10 * sin(3 * time), 0, 10 * cos(3 * time), 0);
        lightData[0].intensities = glm::vec4(1.5f * sin(1.33*time), 1.5 * cos(1.33*time), 0, 0);

        lightData[1].position = glm::vec4(10 * sin(1.33*time),   10 * cos(1.33*time), 0, 0);
        lightData[1].intensities = glm::vec4(1.5f * sin(1.33*time + C::PI/3), 1.5 * cos(1.33*time+ C::PI/3), 0, 0);

        lightData[2].position = glm::vec4(0, 10 * sin(4.377*time), 10 * cos(4.377*time), 0);
        lightData[2].intensities = glm::vec4(1.5f * sin(1.33*time+(C::PI*2/3)), 1.5 * cos(1.33*time + (C::PI * 2 / 3)), 0, 0);



        lightBuf.update(light0PosIndex, 16, &(lightData[0].position));
        lightBuf.update(light0PosIndex + 16, 16, &(lightData[0].intensities));

        lightBuf.update(light1PosIndex, 16, &(lightData[1].position));
        lightBuf.update(light1PosIndex + 16, 16, &(lightData[1].intensities));

        lightBuf.update(light2PosIndex, 16, &(lightData[2].position));
        lightBuf.update(light2PosIndex + 16, 16, &(lightData[2].intensities));




		matrixBuf.update(projectionIndex, sizeof(glm::mat4), glm::value_ptr(projection));
		matrixBuf.update(viewIndex, sizeof(glm::mat4), glm::value_ptr(view));
		matrixBuf.update(viewPosIndex, sizeof(glm::vec4), glm::value_ptr(pivot));
        //GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
        //GLCall(glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)));
        GLCall(glUniform1f(uniformTime, time));

        glfwSwapBuffers(window);
        glfwPollEvents();

        // LIVE UPDATE SHADER AND MATERIALS
        // Util::everyTwoSeconds(t);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
