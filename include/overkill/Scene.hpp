#pragma once

#include <algorithm>
#include <vector>

#include <overkill/Config.hpp>
#include <overkill/EntityModel.hpp>
#include <overkill/EntityCamera.hpp>
#include <overkill/EntityDirectionalLight.hpp>
#include <overkill/EntityPointLight.hpp>
#include <overkill/EntityNode.hpp>
#include <overkill/Parser.hpp>
#include <overkill/Util.hpp>
#include <overkill/Init.hpp>


namespace overkill 
{

class EntityNode;
class EntityModel;
class EntityCamera;


struct DirectionalLight 
{
    glm::vec4 direction;    //16->16
    glm::vec4 intensities;  //16->32
};

class Scene
{
private:
    // static EntityCamera* m_camera;
    static std::vector<Entity*> m_entities;     // All the 3D models in the world.
    static std::vector<int> m_rootEntities;     // All the entityIDs that are root noes in world.
    static EntityCamera* m_activeCamera;          // Pointer to the camera that is currently active.
    static int m_cameraCount;                   // The amount of cameras in the scene.s
    
    static DirectionalLight m_sun;
    static UniformBuffer m_matrixBuffer;
    static UniformBuffer m_lightBuffer;
    static GLuint m_projectionGLindex;
    static GLuint m_pointLightGLindex;
    static GLuint m_sunGLindex;

    // OFFSETS
    constexpr static int m_cameraOffset = 0;
    static           int m_entitiesOffset;
    static           int m_lightsOffset;
    static           int m_lightsCount;

    static std::string m_sceneLoaded;
    //static std::vector<PointLight> m_pointLights;
    //static DirectionalLight        m_sun;

    static void setChild(int parentID, int childID);
    static void bufferPointLights();

public:
    static void load();                         // Dummy loader.
    static bool entityExist(const C::Tag tag);

public:
    Scene();                                    //load from file in future.
    static void load(std::string sceneFile);                         // Dummy loader.
    static void reload(); 
 
    static int addEntity(Entity* model);
    // static add light(light).
    static Entity* getEntity(int ID);
    static Entity* getEntityByTag(const C::Tag tag);
    static EntityCamera* getActiveCamera();
    static void cycleCameras();

    static void update(float dt);               // Cass update function for all root entities.
    static void draw(float t);
    static void clean();                        // Deletes all the constructed entities.
};

}