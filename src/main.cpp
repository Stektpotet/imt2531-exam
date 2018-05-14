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

#include <imgui/imgui.h>

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
#include <overkill/Watcher.hpp>


using namespace overkill;



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
    Init::ImGui(C::window);
    Watcher::pollEvents();

    // Load asset subsystems
    TextureSystem::load();
    ShaderSystem::createUniformBuffers();
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();
    
    Init::loadOBJFiles();

    SeasonSystem::Init();

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

        glfwPollEvents();

        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::Begin("Season Data"); 
        ImGui::BeginGroup();
        ImGui::Text("Season: %f", SeasonSystem::getSeasonTime());
        ImGui::Text("Season: %s", SeasonSystem::getSeasonName().c_str());
        ImGui::Text("Month: %s", SeasonSystem::getMonthName().c_str());
        ImGui::EndGroup();
        ImGui::Spacing();
        ImGui::BeginGroup();
        //ImGui::SliderFloat("Season")
        ImGui::Text("Day: %f", SeasonSystem::getDayTime());
        ImGui::Text("Day Phase: %s", SeasonSystem::getDayPhaseName().c_str());
        ImGui::EndGroup();
        ImGui::Spacing();
        ImGui::BeginGroup();
        //ImGui::SliderFloat("Season")
        ImGui::Text("Glider Info");
        EntityGlider* glider = ((EntityGlider*)Scene::getActiveGlider());
        ImGui::Text("Speed: %.2f KM/H", glider->getSpeed_KMPH());
        ImGui::Text("Acceleration: %.3f", glider->getAcceleration());
        ImGui::Text("Position:");
        ImGui::Text("       X: %0.3f", glider->getPosition().x);
        ImGui::Text("       Y: %0.3f", glider->getPosition().y);
        ImGui::Text("       Z: %0.3f", glider->getPosition().z);
        ImGui::EndGroup();
        ImGui::End();

        Renderer::clear();



        //Update
        SeasonSystem::Update(dt); 

        Scene::update(dt);
        // Draws all the models in the scene.
        Scene::draw(t);     

        //This can be done to debug positions
        //glm::mat4 camPosDebugM2W = glm::translate(glm::mat4(1), glm::vec3(cameraTransform.position));
        //Renderer::draw(ModelSystem::getById(0), camPosDebugM2W, t);

        Input::clearMap();

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(C::window);
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
