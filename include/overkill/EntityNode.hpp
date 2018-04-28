#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Scene.hpp> 
#include <overkill/Entity.hpp>

namespace overkill
{


class EntityNode : public Entity
{

private:
    glm::mat4 m_transformMatrix;
    glm::mat4 getModelMatrix(glm::mat4 parentModelMatrix = glm::mat4(1));

public:
    EntityNode( C::Tag EntityTag, int entityID,
                glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 rot = glm::vec3(0,0,0), 
                glm::vec3 scale = glm::vec3(1,1,1), glm::vec3 vel = glm::vec3(0,0,0), 
                glm::vec3 angVel = glm::vec3(0,0,0));

    void update(float dt, glm::mat4 parentMatrix = glm::mat4(1)) override final;
    void draw(float t) override final;
};

}

// This is sad. This class is sad.