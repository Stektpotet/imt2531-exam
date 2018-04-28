#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Scene.hpp>
#include <overkill/Entity.hpp>
#include <overkill/Material.hpp>
#include <overkill/ModelSystem.hpp>
#include <overkill/Renderer.hpp>
#include <overkill/Util.hpp>

namespace overkill
{

class Renderer;

class EntityModel : public Entity
{
private:
    int m_modelID;         // What model with id will be used to draw Entity.

public:
    EntityModel(C::Tag modelTag, C::Tag EntityTag, int entityID,
                glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 rot = glm::vec3(0,0,0), 
                glm::vec3 scale = glm::vec3(1,1,1), glm::vec3 vel = glm::vec3(0,0,0), 
                glm::vec3 angVel = glm::vec3(0,0,0));

    int getModel();
    glm::vec3 getScale();
    glm::mat4 getModelMatrix(glm::mat4 parentModelMatrix = glm::mat4(1));
    void setModelByID(int modelID);
    void setModelByTag(C::Tag tag);

    void update(float dt, glm::mat4 parentMatrix = glm::mat4(1)) override final;
    void draw(float t) override final;
};

}
