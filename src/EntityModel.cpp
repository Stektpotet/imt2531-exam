#include <overkill/EntityModel.hpp>

namespace overkill
{

    EntityModel::EntityModel(C::Tag modelTag, C::Tag entityTag, int entityID,
                            glm::vec3 pos, glm::vec3 rot, 
                            glm::vec3 scale, glm::vec3 vel, 
                            glm::vec3 angVel) : Entity(entityTag, entityID, pos, rot, scale, vel, angVel)
    {
        if (modelTag == "")
        {
            LOG_WARN("ModelTag is empty. The model will not be drawn.");
            m_modelID = -1;
        }
        else  
        {
            m_modelID =  ModelSystem::getIdByTag(modelTag);
        }
        m_transformMatrix = getModelMatrix();
        update(0);
    }

     
    glm::mat4 EntityModel::getModelMatrix(glm::mat4 parentModelMatrix)
    {   
        glm::mat4 model = glm::mat4(1);

        model = glm::scale(model, m_scale);
        model = glm::rotate(model, m_rotation.x, glm::vec3(1,0,0));     //Rotate in model space.
        model = glm::rotate(model, m_rotation.y, glm::vec3(0,1,0));
        model = glm::rotate(model, m_rotation.z, glm::vec3(0,0,1));
        glm::mat4 worldPos = glm::translate(glm::mat4(1), m_position);  //Translate in world space.
        model = parentModelMatrix * worldPos * model;

        return model;   
    }

    int EntityModel::getModel()
    {   
        return m_modelID;   
    }

    glm::vec3 EntityModel::getScale()
    {  
        return m_scale;  
    }

    void EntityModel::setModelByID(int modelID)
    {   
        m_modelID = modelID;   }
    
     void EntityModel::setModelByTag(C::Tag tag)
    {   
        m_modelID =  ModelSystem::getIdByTag(tag);  
    }

    void EntityModel::update(float dt, glm::mat4 parentMatrix)
    {
        if (m_modelID == -1)
        {
            LOG_WARN("Model ID is not set due to bad construct params. Model will not be drawn.");
        }
        m_position += m_velocity * dt;
        m_rotation += m_angularVelocity * dt;

        m_transformMatrix = getModelMatrix(parentMatrix);
/*
        LOG_DEBUG("Update()\n\nentityID %d, entiryTag %s, \nm_position %f, %f, %f\nm_rotation %f, %f, %f\nm_angVel %f, %f, %f\ndeltatime %f\n", 
                m_entityID,  m_entityTag.data(),
                m_position.x, m_position.y, m_position.z, 
                glm::degrees(m_rotation.x), glm::degrees(m_rotation.y), glm::degrees(m_rotation.z), 
                glm::degrees(m_angularVelocity.x), glm::degrees(m_angularVelocity.y), glm::degrees(m_angularVelocity.z), 
                dt);
        Util::printMatrix(parentMatrix, "ParentMatrix:");
        Util::printMatrix(m_transformMatrix, "TransormMatrix:");
        printf("\n\n");
*/

        if (m_childIDs.size() > 0)                  // If we actually have kids.
        {            
            for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
            {
                //LOG_DEBUG("\n\nI am entityID %d Updating child with entityID %d.\n", m_entityID, child);
                Scene::getEntity(child)-> update(dt, m_transformMatrix);
            }
        }
    }

    void EntityModel::draw(float t)
    {   
        Renderer::draw(ModelSystem::getById(m_modelID), m_transformMatrix, t);
    }

}
