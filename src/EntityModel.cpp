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
    glm::mat4 model = glm::mat4(1);

    model =  glm::translate(model, m_position);
    model = glm::rotate(model, m_rotation.x, glm::vec3(1,0,0));
    model = glm::rotate(model, m_rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, m_rotation.z, glm::vec3(0,0,1));
    model = glm::scale(model, m_scale);

    return model;   
}

    
int EntityModel::getModel()
{   return m_modelID;   }

glm::vec3 EntityModel::getScale()
{   return m_scale;  }

void EntityModel::setModelByID(int modelID)
{   m_modelID = modelID;   }
    
    void EntityModel::setModelByTag(C::Tag tag)
{   m_modelID =  ModelSystem::getIdByTag(tag);  }

void EntityModel::setScale(glm::vec3 scale)
{   m_scale = scale;   }


void EntityModel::draw()
{   
    Renderer::draw(ModelSystem::getById(m_modelID), getModelMatrix());
}


}
