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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <overkill/Init.hpp>
#include <overkill/gl_caller.hpp>
#include <overkill/renderer.hpp>
#include <overkill/vertexArray.hpp>
#include <overkill/vertexBuffer.hpp>
#include <overkill/elementBuffer.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/texture.hpp>
#include <overkill/mesh.hpp>

static void OnInputKeyPress(GLFWwindow* window, int keyCode, int scanCode, int mods)
{
    printf("Pressing %i, as char: %c\n", keyCode, char(keyCode));
    if (keyCode == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, 1);
    }

}
static void OnInputKeyHold(GLFWwindow* window, int keyCode, int scanCode, int mods)
{
    printf("Holding %i, as char: %c\n", keyCode, char(keyCode));
    if (keyCode == GLFW_KEY_W)
    {

    }
    if (keyCode == GLFW_KEY_S)
    {

    }
}
static void OnInputKeyUnpress(GLFWwindow* window, int keyCode, int scanCode, int mods)
{
    printf("Unpressed %i, as char: %c\n", keyCode, char(keyCode));
}
static void OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        OnInputKeyPress(window, keyCode, scanCode, mods);
        break;
    case GLFW_REPEAT:
        OnInputKeyHold(window, keyCode, scanCode, mods);
        break;
    case GLFW_RELEASE:
        OnInputKeyUnpress(window, keyCode, scanCode, mods);
        break;
    }
}

static float fovy = 90, aspect = 1;
static void OnCursorHover(GLFWwindow* window, double x, double y)
{
    fovy = (y / 512) + 32;
}
int main()
{
    using namespace overkill;
    using BackgroundColor = glm::vec4;
    using VersionMajor = int;
    using VersionMinor = int;
    using Width        = int;
    using Height       = int;

	auto window = Init::GLFW(
        VersionMajor(4), VersionMinor(1), Width(800), Height(600), "Assignment 2 - Cube");

    Init::GLEW();
    Init::OpenGL(
        BackgroundColor{ 1.0f, .8f, .6f, 1.0f}); //(0.05f, 0.06f, 0.075f, 1.0f) for sexy dark blue-grey



    struct Vertex
    {
        GLfloat position[3];
        
        // @note normalized integers (decimal floating point number): 
        // @ref  https://www.khronos.org/opengl/wiki/Normalized_Integer
        // @ref  https://stackoverflow.com/questions/14036892/using-gl-int-2-10-10-10-rev-in-glvertexattribpointer
        GLuint  normal;
        GLshort  uv_1[2];
        //GLhalf  uv_2[2];
        //GLhalf  uv_3[2];
        // etc.
        GLubyte  color[4];
    };

    Vertex vertices[] = {

        // Cube 1
        { { -0.5f, -0.5f,  0.5f },  1,  { 0, 0 }, { 255,     255-96,  255-192, 255 } },
        { {  0.5f, -0.5f,  0.5f },  1,  { 1, 0 }, { 255-16,  255-112, 255-208, 255 } },
        { {  0.5f,  0.5f,  0.5f },  1,  { 1, 1 }, { 255-32,  255-128, 255-224, 255 } },
        { { -0.5f,  0.5f,  0.5f },  1,  { 0, 1 }, { 255-48,  255-144, 255-240, 255 } },

        { { -0.5f, -0.5f, -0.5f },  1,  { 1, 0 }, { 255-64,  255-160, 255-255, 255 } },
        { { -0.5f,  0.5f, -0.5f },  1,  { 1, 1 }, { 255-80,  255-176, 255, 255 } },

        { {  0.5f, -0.5f, -0.5f },  1,  { 0, 0 }, { 255-96,  255-192, 255-16, 255 } },
        { {  0.5f,  0.5f, -0.5f },  1,  { 0, 1 }, { 255-112, 255-208, 255-32, 255 } },

        { { -0.5f, -0.5f, -0.5f },  1,  { 0, 1 }, { 255-128, 255-224, 255-48, 255 } },
        { {  0.5f, -0.5f, -0.5f },  1,  { 1, 1 }, { 255-144, 255-240, 255-64, 255 } },

        { { -0.5f,  0.5f, -0.5f },  1,  { 0, 0 }, { 255-160, 255-255,  255-80, 255 } },
        { {  0.5f,  0.5f, -0.5f },  1,  { 1, 0 }, { 255-176, 255,      255-96, 255 } },

        { { -0.5f,  0.5f,  0.5f },  1,  { 0, 1 }, { 255-192, 255-16,   255-112, 255 } },
        { {  0.5f,  0.5f,  0.5f },  1,  { 1, 1 }, { 255-208, 255-32,   255-128, 255 } },


        // Cube 2
        { { -1.0f, -1.0f,  1.0f },  1,  { 0, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f, -1.0f,  1.0f },  1,  { 1, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f,  1.0f },  1,  { 1, 1 }, { 255, 255, 255, 80 } },
        { { -1.0f,  1.0f,  1.0f },  1,  { 0, 1 }, { 255, 255, 255, 80 } },

        { { -1.0f, -1.0f, -1.0f },  1,  { 1, 0 }, { 255, 255, 255, 80 } },
        { { -1.0f,  1.0f, -1.0f },  1,  { 1, 1 }, { 255, 255, 255, 80 } },

        { {  1.0f, -1.0f, -1.0f },  1,  { 0, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f, -1.0f },  1,  { 0, 1 }, { 255, 255, 255, 80 } },

        { { -1.0f, -1.0f, -1.0f },  1,  { 0, 1 }, { 255, 255, 255, 80 } },
        { {  1.0f, -1.0f, -1.0f },  1,  { 1, 1 }, { 255, 255, 255, 80 } },

        { { -1.0f,  1.0f, -1.0f },  1,  { 0, 0 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f, -1.0f },  1,  { 1, 0 }, { 255, 255, 255, 80 } },

        { { -1.0f,  1.0f,  1.0f },  1,  { 0, 1 }, { 255, 255, 255, 80 } },
        { {  1.0f,  1.0f,  1.0f },  1,  { 1, 1 }, { 255, 255, 255, 80 } },

    };

    unsigned int indicies[] = {

        // Cube 1
        0, 1, 2,
        2, 3, 0,

        4, 0, 3,
        3, 5, 4,

        1, 6, 7,
        7, 2, 1,

        8, 9, 1,
        1, 0, 8,

        10, 11, 9,
        9,  8,  10,

        12, 13, 11,
        11, 10, 12,


        // Cube 2
        14, 15, 16,
        16, 17, 14,

        18, 14, 3+14,
        3 + 14, 5 + 14, 4 + 14,

        1+14, 6+14, 7+14,
        7+14, 2+14, 1+14,

        8+14, 9+14, 1+14,
        1+14, 0+14, 8+14,

        10 + 14, 11 + 14, 9 + 14,
        9 + 14,  8 + 14,  10 + 14,

        12+14, 13+14, 11+14,
        11+14, 10+14, 12+14,
    };


    auto renderer = EdgeRenderer();

    Mesh mesh;
    
    mesh.m_vbo = VertexBuffer(vertices, 14 * 2 * sizeof(Vertex));
    //mesh.m_vao = VertexArray();
    //auto vbo;
    //TODO
    // VetexBufferLayout
    // glVertexAttribPointer(location, items, type, normalized, stride, start);
    auto vbufLayout = VertexBufferAttribLayout();
    vbufLayout.push(3, GL_FLOAT);                       //position;
    vbufLayout.push(1, GL_UNSIGNED_INT);                //normal
    vbufLayout.push(2, GL_SHORT);                       //uv
    vbufLayout.push(4, GL_UNSIGNED_BYTE, GL_TRUE);      //color;

    ShaderProgram shader("assets/shaders/base.shader");

    //auto vbufLayout = CreateFromProgram(GLuint(shader));
    mesh.m_vao.addBuffer(mesh.m_vbo, vbufLayout);
    auto ebuf = ElementBuffer(indicies, 36*2);


    //SCALE -> ROTATE -> TRANSLATE
    glm::mat4 projection = glm::perspective(90.0f, (GLfloat)1024 / (GLfloat)1024, 0.1f, 100.0f);

    //GLCall(glSetUn)
    GLint uniformMVP, uniformTime;
    GLint uniformMVP2, uniformTime2;

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

    glfwSetKeyCallback(window, OnInputKey); //set upp callbacks
    glfwSetCursorPosCallback(window, OnCursorHover);


    for(;;)
    {
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        renderer.clear();
        renderer.draw(mesh.m_vao, ebuf, shader);

        projection = glm::perspective(fovy, aspect, 0.1f, -100.0f);
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
