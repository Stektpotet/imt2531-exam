#include <overkill/EntityGlider.hpp>
#include <overkill/Scene.hpp>

namespace overkill
{

EntityGlider::EntityGlider(
    C::Tag  modelTag, 
    C::Tag  EntityTag, 
    int entityID, 
    glm::vec3 pos, 
    glm::vec3 rot, 
    glm::vec3 scale, 
    glm::vec3 vel, 
    glm::vec3 angVel
) : EntityModel(
    modelTag, 
    EntityTag,
    entityID,
    pos, 
    rot, 
    scale,
    vel, 
    angVel
){}


glm::mat4 EntityGlider::getModelMatrix(glm::mat4 parentModelMatrix)
{
    glm::mat4 model = glm::scale(glm::mat4(1), m_scale);

    model = glm::rotate(model, m_rotation.z, glm::vec3(0,0,1));
    model = glm::rotate(model, m_rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, m_rotation.x, glm::vec3(1,0,0));

    glm::mat4 worldPos = glm::translate(glm::mat4(1), m_position);  //Translate in world space.
    model = parentModelMatrix * worldPos * model;
    return model;
}
void EntityGlider::update(float dt, glm::mat4 parentMatrix)
    {
        if (m_modelID == -1)
        {
            LOG_WARN("Model ID is not set due to bad construct params. Model will not be drawn.");
        }
        m_position += m_velocity * dt;
        m_rotation += m_angularVelocity * dt;

        m_transformMatrix = getModelMatrix(parentMatrix);

        if (m_childIDs.size() > 0)                  // If we actually have kids.
        {            
            for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
            {
                //LOG_DEBUG("\n\nI am entityID %d Updating child with entityID %d.\n", m_entityID, child);
                Scene::getEntity(child)-> update(dt, m_transformMatrix);
            }
        }
    }

}