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


#include <overkill/Init.hpp>
#include <overkill/Input.hpp>
#include <overkill/gl_caller.hpp>
#include <overkill/renderer.hpp>
#include <overkill/vertexArray.hpp>
#include <overkill/vertexBuffer.hpp>
#include <overkill/elementBuffer.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/texture.hpp>
#include <overkill/EntitySystem.hpp>


int main()
{
    using namespace overkill;
    using BackgroundColor = glm::vec4;
    using VersionMajor = int;
    using VersionMinor = int;
    using Width        = int;
    using Height       = int;
    float fovy = 90;

	auto window = Init::GLFW(
        VersionMajor(4), VersionMinor(1), Width(800), Height(600), "Assignment 2 - Cube");

    Init::GLEW();
    Init::OpenGL(BackgroundColor{ 1.0f, .8f, .6f, 1.0f}); //(0.05f, 0.06f, 0.075f, 1.0f) for sexy dark blue-grey


    auto renderer = EdgeRenderer();
    
    EntitySystem::init();
    
    auto entity = EntitySystem::make("cube");
    auto model  = EntitySystem::getModel(entity);

    //SCALE -> ROTATE -> TRANSLATE
    glm::mat4 projection = glm::perspective(90.0f, (GLfloat)1024 / (GLfloat)1024, 0.1f, 100.0f);

    //GLCall(glSetUn)
    GLint uniformMVP, uniformTime;
    GLint uniformMVP2, uniformTime2;

    ShaderProgram shader("assets/shaders/base.shader");

    shader.bind({});
    uniformMVP  = shader.getUniformLocation("projection");
    uniformTime = shader.getUniformLocation("time");

    GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));

    auto tex = Texture("assets/textures/Checkers.jpg");
    tex.bind();
    GLint uniformTex;
    shader.bind({});
    uniformTex = shader.getUniformLocation("mainTex");
    //GLCall(uniformTex = glGetUniformLocation(GLuint(shader), "mainTex"));
    GLCall(glUniform1i(uniformTex, 0));

    glfwSetKeyCallback(window, Input::OnInputKey); //set upp callbacks
    glfwSetCursorPosCallback(window, Input::OnCursorHover);


    for(;;)
    {
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        renderer.clear();
        renderer.draw(model.m_vao, model.m_meshes[0].m_ebo, shader);

        projection = glm::perspective(Input::fovy, Input::aspect, 0.1f, -100.0f);
        shader.bind({});
        GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
        GLCall(glUniform1f(uniformTime, (float)glfwGetTime()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
