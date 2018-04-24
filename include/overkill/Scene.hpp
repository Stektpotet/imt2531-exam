#pragma once

#include <algorithm>
#include <vector>

#include <overkill/EntityModel.hpp>

namespace overkill 
{

class EntityModel;

class Scene
{
private:
    static std::vector<EntityModel> m_modelEntities;    // All the 3D models in the world.
    static std::vector<int> m_rootEntities;             // All the entityIDs that are root noes in world.
    // vector for light.

public:
    Scene();        //load from file in future.
    static void loadScene();        // Dummy loader.
    static int addEntityModel(EntityModel & model);
    static void removeRoot(int ID);
    // static add light(light).
    static EntityModel* getEntityModel(int ID);

    static void update(float dt);   // Cass update function for all root entities.
    static void draw(float t);

};

}