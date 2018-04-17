#include <overkill/EntityModel.hpp>

namespace overkill
{

    EntityModel::EntityModel(C::Tag modelTag,
                            glm::vec3 pos, glm::vec3 rot, 
                            glm::vec3 scale, glm::vec3 vel, 
                            glm::vec3 angVel) : Entity(modelTag, pos, rot, vel, angVel)
    {
        m_modelID =  ModelSystem::getIdByTag(modelTag);
        m_scale = scale;
    }

     
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

    
    int EntityModel::getModel()
    {   return m_modelID;   }

    void EntityModel::setModelByID(int modelID)
    {   m_modelID = modelID;   }
    
     void EntityModel::setModelByTag(C::Tag tag)
    {   m_modelID =  ModelSystem::getIdByTag(tag);  }

    void EntityModel::draw()
    {   
        Renderer::draw(ModelSystem::getById(m_modelID), getModelMatrix());
    }

}