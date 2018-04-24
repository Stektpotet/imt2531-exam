#pragma once

#include <algorithm>
#include <vector>

#include <overkill/Entity.hpp>
#include <overkill/EntityModel.hpp>

namespace overkill 
{

// class Entity;
class EntityModel;

class Scene
{
private:
    static std::vector<Entity*> m_entities;    // All the 3D models in the world.
    static std::vector<int> m_rootEntities;             // All the entityIDs that are root noes in world.
    // vector for light.

    static void setChild(int parentID, int childID);

public:
    Scene();        //load from file in future.
    static void loadScene();        // Dummy loader.
    static int addEntity(Entity* model);
    // static add light(light).
    static Entity* getEntity(int ID);

    static void update(float dt);   // Cass update function for all root entities.
    static void draw(float t);
    static void clean();            // Deletes all the constructed entities.
};

}