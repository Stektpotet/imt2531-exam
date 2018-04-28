#pragma once

#include <algorithm>
#include <vector>

#include <overkill/Config.hpp>
#include <overkill/EntityModel.hpp>
#include <overkill/EntityCamera.hpp>
#include <overkill/Parser.hpp>
#include <overkill/Util.hpp>
#include <overkill/Init.hpp>

namespace overkill 
{

class EntityModel;
class EntityCamera;

class Scene
{
private:
    // static EntityCamera* m_camera;
    static std::vector<Entity*> m_entities;     // All the 3D models in the world.
    static std::vector<int> m_rootEntities;     // All the entityIDs that are root noes in world.
    static EntityCamera* m_activeCamera;          // Pointer to the camera that is currently active.
    static int m_cameraCount;                   // The amount of cameras in the scene.s

    //static std::vector<PointLight> m_pointLights;
    //static DirectionalLight        m_sun;

    static void setChild(int parentID, int childID);
    static bool entityExist(const C::Tag tag);

public:
    Scene();                                    //load from file in future.
    static void load();                         // Dummy loader.
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