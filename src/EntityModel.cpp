#include <overkill/EntityModel.hpp>

namespace overkill
{

    EntityModel::EntityModel(C::Tag modelTag, C::Tag entityTag, int entityID,
                            glm::vec3 pos, glm::vec3 rot, 
                            glm::vec3 scale, glm::vec3 vel, 
                            glm::vec3 angVel) : Entity(entityTag, entityID, pos, rot, vel, angVel)
    {
        m_modelID =  ModelSystem::getIdByTag(modelTag);
        m_scale = scale;
        glm::mat4 m_transformMatrix = getModelMatrix();
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
    void EntityModel::addChild(int childID)
    {   
        Scene::removeRoot(childID);
        m_childIDs.push_back(childID);  
    }

    void EntityModel::setModelByID(int modelID)
    {   
        m_modelID = modelID;   }
    
     void EntityModel::setModelByTag(C::Tag tag)
    {   
        m_modelID =  ModelSystem::getIdByTag(tag);  
    }

    void EntityModel::setScale(glm::vec3 scale)
    {   
        m_scale = scale;   
    }

    void EntityModel::update(float dt, glm::mat4 parentMatrix)
    {
        m_position += m_velocity * dt;
        m_rotation += m_angularVelocity * dt;
        
        m_transformMatrix = getModelMatrix(parentMatrix);
        

        printf("\n\nUpdate()\n\nentityID %d, \nm_position %f, %f, %f\nm_rotation %f, %f, %f\nm_angVel %f, %f, %f\ndeltatime %f", 
                m_entityID, 
                m_position.x, m_position.y, m_position.z, 
                m_rotation.x, m_rotation.y, m_rotation.z, 
                m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z, 
                dt);
        Util::printMatrix(parentMatrix, "parentMatrix:");

        if (m_childIDs.size() > 0)                  // If we actually have kids.
        {            
            for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
            {
                printf("\n\nI am entityID %d Updating child with entityID %d.\n", m_entityID, child);
                Scene::getEntityModel(child)-> update(dt, m_transformMatrix);
            }
        }
    }

    void EntityModel::draw(float t)
    {   
        Renderer::draw(ModelSystem::getById(m_modelID), m_transformMatrix, t);
    }

}
