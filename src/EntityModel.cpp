#include <overkill/EntityModel.hpp>

namespace overkill
{

    EntityModel::EntityModel(C::Tag tag, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec3 vel, 
        glm::vec3 angVel, unsigned int modelID, Material material)
        : Entity(tag, pos, rot, vel, angVel)
    {
        m_modelID = modelID;
        m_material = material;
        m_scale = scale;
    }
    
    unsigned int EntityModel::getModel()
    {   return m_modelID;   }
    
    Material EntityModel::getMaterial()
    {   return m_material;   }
    
    glm::mat4 EntityModel::getModelMatrix()
    {   
        glm::mat4 rotationMatrix = glm::mat4(1);
        glm::rotate(rotationMatrix, m_rotation.x, glm::vec3(0,1,0));
        glm::rotate(rotationMatrix, m_rotation.y, glm::vec3(0,1,0));
        glm::rotate(rotationMatrix, m_rotation.z, glm::vec3(0,1,0));

        return  glm::translate(glm::mat4(1), m_position) *
                rotationMatrix *
                glm::scale(glm::mat4(1), m_scale);
    }

    void EntityModel::setModel(unsigned int modelID)
    {   m_modelID = modelID;   }
    
    void EntityModel::setMaterial(Material material)
    {   m_material = material;   }
    
    void EntityModel::draw()
    {   
        // Renderer, plz draw!
    }

}