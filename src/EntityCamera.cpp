#include <overkill/EntityCamera.hpp>

namespace overkill
{

EntityCamera::EntityCamera(C::Tag entityTag, int entityID, 
                            glm::vec3 pos, glm::vec3 rot, 
                            glm::vec3 vel, glm::vec3 angVel, float FOV, float aspectRatio, float nearClip, float farclip)
                            : Entity(entityTag, entityID, pos, rot, vel, angVel)
{
    m_FOV = FOV;
    m_aspectRatio = aspectRatio;
    m_nearClip = nearClip;
    m_farClip = farclip;
}

glm::mat4 EntityCamera::getViewMatrix(glm::mat4 parentModelMatrix)
{   
    glm::mat4 view = glm::mat4(1);

    view = glm::rotate(view, m_rotation.x, glm::vec3(1,0,0));       //Rotate in model space.
    view = glm::rotate(view, m_rotation.y, glm::vec3(0,1,0));
    view = glm::rotate(view, m_rotation.z, glm::vec3(0,0,1));
    glm::mat4 worldPos = glm::translate(glm::mat4(1), m_position);  //Translate in world space.
    view = parentModelMatrix * view * worldPos;

    return view;   
}

void EntityCamera::update(float dt, glm::mat4 parentMatrix)
{
        m_position += m_velocity * dt;
        m_rotation += m_angularVelocity * dt;

        m_viewMatrix = getViewMatrix(parentMatrix);

        printf("\n\nUpdate()\n\nentityID %d, entityTag %s, \nm_position %f, %f, %f\nm_rotation %f, %f, %f\nm_angVel %f, %f, %f\ndeltatime %f\n", 
                m_entityID, m_entityTag.data(),
                m_position.x, m_position.y, m_position.z, 
                m_rotation.x, m_rotation.y, m_rotation.z, 
                m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z, 
                dt);
        Util::printMatrix(parentMatrix, "ParentMatrix:");
        Util::printMatrix(m_viewMatrix, "ViewMatrix:");


        if (m_childIDs.size() > 0)                  // If we actually have kids.
        {            
            for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
            {
                printf("\n\nI am entityID () %d Updating child with entityID %d.\n", m_entityID, child);
                Scene::getEntity(child)-> update(dt, m_viewMatrix);
            }
        }
}

void EntityCamera::draw(float t)
{

}

}