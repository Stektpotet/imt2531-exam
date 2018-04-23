#pragma once

#include <vector>

#include <overkill/EntityModel.hpp>

namespace overkill 
{

class Scene
{
private:
    std::vector<EntityModel> m_modelEnties;
    // vector for light.

public:
    Scene();        //load from file in future.
    int addEntityModel(EntityModel model);
    // add light.


};

}