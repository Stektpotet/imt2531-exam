#include <overkill/EntityPointLight.hpp>

namespace overkill 
{


auto EntityPointLight::pack() -> PointLightBO
{
    return PointLightBO
    {
        glm::vec4{m_position.x, m_position.y, m_position.z, 0},
        glm::vec4{m_intensities.r, m_intensities.g, m_intensities.b, 0},
        m_constantFalloff,
        m_linearFalloff,
        m_quadraticFalloff
    };
}


glm::mat4 EntityPointLight::getModelMatrix(glm::mat4 parentModelMatrix)
{
    glm::mat4 model = glm::mat4(1);

    model = glm::scale(model, m_scale);
    model = glm::rotate(model, m_rotation.x, glm::vec3(1, 0, 0));     //Rotate in model space.
    model = glm::rotate(model, m_rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, m_rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 worldPos = glm::translate(glm::mat4(1), m_position);  //Translate in world space.
    model = parentModelMatrix * worldPos * model;

    return model;
}

#define DEBUG 0

void EntityPointLight::update(float dt, glm::mat4 parentMatrix)
{
    m_transformMatrix = getModelMatrix(parentMatrix);
    m_position += m_velocity * dt;
    m_rotation += m_angularVelocity * dt;

    m_position = glm::vec3(parentMatrix[3]);


#if DEBUG
    LOG_DEBUG("Update()\n\nentityID %d, entiryTag %s, \nm_position %f, %f, %f\nm_rotation %f, %f, %f\nm_angVel %f, %f, %f\ndeltatime %f\n",
    m_entityID,  m_entityTag.data(),
    m_position.x, m_position.y, m_position.z,
    glm::degrees(m_rotation.x), glm::degrees(m_rotation.y), glm::degrees(m_rotation.z),
    glm::degrees(m_angularVelocity.x), glm::degrees(m_angularVelocity.y), glm::degrees(m_angularVelocity.z),
    dt);

    Util::printMatrix(parentMatrix, "ParentMatrix:");
    Util::printMatrix(m_transformMatrix, "TransormMatrix:");
    printf("\n\n");
#endif
    

    if (m_childIDs.size() > 0)                  // If we actually have kids.
    {
        for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
        {
            //LOG_DEBUG("\n\nI am entityID %d Updating child with entityID %d.\n", m_entityID, child);
            Scene::getEntity(child)->update(dt, m_transformMatrix);
        }
    }
}
}