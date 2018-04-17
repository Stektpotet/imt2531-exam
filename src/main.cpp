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
	modelTransform.m_position = glm::vec3{0,0,2};


    glm::mat4 projection = glm::perspective(C::FOV, C::AspectRatio, C::NearClip, C::FarClip);
    glm::mat4 camera = glm::mat4(1); 
    glm::mat4 pivot = glm::translate(glm::mat4(1),glm::vec3(0, 0, C::CameraOffset));  //Camera pos in world.
    glm::mat4 view = glm::mat4(1);
	glm::mat4 m2w = modelTransform.modelToWorld();

    //GLCall(glSetUn)
    GLint uniformMVP, uniformTime;
    GLint uniformView;                                  //Will communicate camera orientation to shader.
	GLint uniformM2W;

    shader.bind();
	uniformM2W = shader.getUniformLocation("m2w");
    uniformMVP  = shader.getUniformLocation("projection");
    uniformTime = shader.getUniformLocation("time");
    uniformView = shader.getUniformLocation("view");

	GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
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
		GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
        GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
        GLCall(glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)));
        GLCall(glUniform1f(uniformTime, (float)glfwGetTime()));

        glfwSwapBuffers(window);
        glfwPollEvents();

        // LIVE UPDATE SHADER AND MATERIALS
        // Util::everyTwoSeconds(t);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
