#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Material.hpp>
#include <overkill/Entity.hpp>
#include <overkill/ModelSystem.hpp>
#include <overkill/Renderer.hpp>

namespace overkill
{
    class Renderer;

class EntityModel : public Entity
{
private:
    int m_modelID;         // What model with id will be used to draw Entity.
    glm::vec3 m_scale;

public:
    glm::mat4 getModelMatrix();

    EntityModel(C::Tag modelTag,
                glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 rot = glm::vec3(0,0,0), 
                glm::vec3 scale = glm::vec3(1,1,1), glm::vec3 vel = glm::vec3(0,0,0), 
                glm::vec3 angVel = glm::vec3(0,0,0));

    int getModel();
    glm::vec3 getScale();
    void setModelByID(int modelID);
    void setModelByTag(C::Tag tag);
    void setScale(glm::vec3 scale = glm::vec3(1));

    void draw();
};

}
