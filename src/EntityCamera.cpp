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
    view = parentModelMatrix * worldPos * view;

    return view;   
}

void EntityCamera::update(float dt)
{

        if (m_childIDs.size() > 0)                  // If we actually have kids.
        {            
            for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
            {
                printf("\n\nI am entityID %d Updating child with entityID %d.\n", m_entityID, child);
                Scene::getEntity(child)-> update(dt, m_viewMatrix);
            }
        }
}


}