#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Material.hpp>
#include <overkill/Entity.hpp>

namespace overkill
{

class EntityModel : public Entity
{
private:
    unsigned int m_modelID;         // What model with id will be used to draw Entity.
    Material m_material;            // What material it will be drawn with.
    glm::vec3 m_scale;

public:
    EntityModel(C::Tag tag, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec3 vel, 
        glm::vec3 angVel, unsigned int modelID, Material material);

    unsigned int getModel();
    Material getMaterial();
    glm::mat4 getModelMatrix();
    void setModel(unsigned int modelID);
    void setMaterial(Material material);
    
    void draw();
};

}