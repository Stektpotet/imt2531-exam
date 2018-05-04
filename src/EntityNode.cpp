#include <overkill/EntityNode.hpp>

namespace overkill
{

EntityNode::EntityNode(C::Tag entityTag, int entityID,
                        glm::vec3 pos, glm::vec3 rot, 
                        glm::vec3 scale, glm::vec3 vel, 
                        glm::vec3 angVel) : Entity(entityTag, entityID, pos, rot, scale, vel, angVel)
{
    m_transformMatrix = getModelMatrix();
    update(0);
}

glm::mat4 EntityNode::getModelMatrix(glm::mat4 parentModelMatrix)
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

void EntityNode::update(float dt, glm::mat4 parentMatrix)
{
        m_position += m_velocity * dt;
        m_rotation += m_angularVelocity * dt;

        m_transformMatrix = getModelMatrix(parentMatrix);

        if (m_childIDs.size() > 0)                  // If we actually have kids.
        {            
            for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
            {
                //LOG_DEBUG("\n\nI am entityID () %d Updating child with entityID %d.\n", m_entityID, child);
                Scene::getEntity(child)-> update(dt, m_transformMatrix);
            }
        }
}

void EntityNode::draw(float /*t*/)
{

}

}