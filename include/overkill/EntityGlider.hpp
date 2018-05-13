#pragma once

#include <overkill/EntityModel.hpp>

namespace overkill
{
class EntityGlider: public EntityModel
{
public:
    EntityGlider(C::Tag modelTag, C::Tag EntityTag, int entityID,
        glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 rot = glm::vec3(0, 0, 0),
        glm::vec3 scale = glm::vec3(1, 1, 1), glm::vec3 vel = glm::vec3(0, 0, 0),
        glm::vec3 angVel = glm::vec3(0, 0, 0));
        

    glm::mat4 getModelMatrix(glm::mat4 parentModelMatrix = glm::mat4(1));
    
    void update(float dt, glm::mat4 parentMatrix = glm::mat4(1)) override;
};

}