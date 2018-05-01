#include <overkill/EntityCamera.hpp>

namespace overkill
{

EntityCamera::EntityCamera(C::Tag entityTag, int entityID, 
                            glm::vec3 pos, glm::vec3 rot, 
                            glm::vec3 vel, glm::vec3 angVel, 
                            CameraMode camMode,
                            float FOV, float aspectRatio, float nearClip, float farclip)
                            : Entity(entityTag, entityID, pos, rot, glm::vec3(1), vel, angVel)  // default scale is 1.
{
    m_modelSpacePos = pos;
    m_cameraMode = camMode;
    m_FOV = glm::radians<float>(FOV);
    m_aspectRatio = aspectRatio;
    m_nearClip = nearClip;
    m_farClip = farclip;
    m_cameraTransform.projectionMatrix = glm::perspective(m_FOV, m_aspectRatio, m_nearClip, m_farClip);
}

glm::mat4 EntityCamera::getModelToWorldMatrix(glm::mat4 parentModelMatrix)
{
    glm::mat4 model = glm::mat4(1);
    glm::mat4 worldPos = glm::mat4(1);
    glm::mat4 rotation = glm::mat4(1);

    rotation = glm::rotate(rotation, m_rotation.y, glm::vec3(0, 1, 0));
    rotation = glm::rotate(rotation, m_rotation.x, glm::vec3(1, 0, 0));     //Rotate in model space.
    rotation = glm::rotate(rotation, m_rotation.z, glm::vec3(0, 0, 1));

    worldPos = glm::translate(glm::mat4(1), glm::vec3(m_modelSpacePos.x, m_modelSpacePos.y, -m_modelSpacePos.z));                        //Translate in world space.

    if (m_cameraMode == FREELOOK)
    {
        model = parentModelMatrix * worldPos * rotation;        
    }
    else if (m_cameraMode == ORBITAL)
    {
        /*glm::vec3 direction = glm::vec3{
            cos(m_rotation.y)*cos(m_rotation.x),
            sin(m_rotation.y)*cos(m_rotation.x),
            sin(m_rotation.z)
        };*/
        model = parentModelMatrix * rotation * worldPos;        
    }
    else 
    {
        LOG_ERROR("Camera mode is not a valid value.");
    }

    return model;
}

glm::mat4 EntityCamera::getViewMatrix(glm::mat4 parentModelMatrix)
{
    return glm::inverse(getModelToWorldMatrix(parentModelMatrix));
}

void EntityCamera::checkInput()
{
    switch (m_cameraMode)
    {
        case FREELOOK:
            if (Input::m_navKeyPressed[W])
            {
                m_modelSpacePos += glm::vec3(C::PanSensitivity * -glm::sin(m_rotation.y) * glm::cos(m_rotation.x),
                                        C::PanSensitivity * glm::sin(m_rotation.x), 
                                        C::PanSensitivity * glm::cos(m_rotation.y) * glm::cos(m_rotation.x));
            }
            if (Input::m_navKeyPressed[S])
            {
                m_modelSpacePos += glm::vec3(C::PanSensitivity * glm::sin(m_rotation.y) * glm::cos(m_rotation.x),
                                        C::PanSensitivity * -glm::sin(m_rotation.x), 
                                        C::PanSensitivity * -glm::cos(m_rotation.y) * glm::cos(m_rotation.x));
            }
            if (Input::m_navKeyPressed[D])
            {
                m_modelSpacePos += glm::vec3(C::PanSensitivity * glm::cos(m_rotation.y) * glm::cos(m_rotation.x),
                                        0, 
                                        C::PanSensitivity * glm::sin(m_rotation.y) * glm::cos(m_rotation.x));        
            }
            if (Input::m_navKeyPressed[A])
            {
                m_modelSpacePos += glm::vec3(-C::PanSensitivity * glm::cos(m_rotation.y) * glm::cos(m_rotation.x),
                                        0, 
                                        -C::PanSensitivity * glm::sin(m_rotation.y) * glm::cos(m_rotation.x));
            }
            if (Input::m_navKeyPressed[Q])
            {
                m_modelSpacePos += glm::vec3(0, -C::PanSensitivity, 0);
            }
            if (Input::m_navKeyPressed[E])
            {
                m_modelSpacePos += glm::vec3(0, C::PanSensitivity, 0);
            }
            break;

        case ORBITAL:
            if (Input::m_navKeyPressed[W])
            {
                m_modelSpacePos += glm::vec3(0, 0, C::PanSensitivity);
            }
            if (Input::m_navKeyPressed[S])
            {
                m_modelSpacePos += glm::vec3(0, 0, -C::PanSensitivity);
            }
            if (Input::m_navKeyPressed[D])
            {
                m_modelSpacePos += glm::vec3(C::PanSensitivity, 0, 0);
            }
            if (Input::m_navKeyPressed[A])
            {
                m_modelSpacePos += glm::vec3(-C::PanSensitivity, 0, 0);
            }
            if (Input::m_navKeyPressed[Q])
            {
                m_modelSpacePos += glm::vec3(0, -C::PanSensitivity, 0);
            }
            if (Input::m_navKeyPressed[E])
            {
                m_modelSpacePos += glm::vec3(0, C::PanSensitivity, 0);
            }
        break;
    }
}


void EntityCamera::cycleMode()
{
    switch (m_cameraMode)
    {
        case FREELOOK: 
            m_position.z = (glm::min)(-40.0f, m_position.z);  // Returns whichever value is smallest.
            m_cameraMode = ORBITAL; 
            break;

        case ORBITAL:
            m_cameraMode = FREELOOK;
            break;
    }
}

void EntityCamera::update(float dt, glm::mat4 parentMatrix)
{
    
        m_position = m_modelSpacePos + m_velocity + glm::vec3(parentMatrix[3]) * dt;
        m_rotation += m_angularVelocity * dt;

        m_cameraTransform.viewMatrix = getViewMatrix(parentMatrix);
        m_cameraTransform.position = glm::vec4(m_position.x, m_position.y, -m_position.z, 1.0f);
        m_transformMatrix = getModelToWorldMatrix(parentMatrix);

        /*LOG_DEBUG("Update()\n\nentityID %d, entityTag %s,\nFOV %f, aspectRatio %f\nnearClip %f, farClip %f \nm_position %f, %f, %f\nm_rotation %f, %f, %f\nm_angVel %f, %f, %f\ndeltatime %f\n", 
                 m_entityID, m_entityTag.data(),
                 m_FOV, m_aspectRatio, m_nearClip, m_farClip,
                 m_position.x, m_position.y, m_position.z, 
                 glm::degrees(m_rotation.x), glm::degrees(m_rotation.y), glm::degrees(m_rotation.z), 
                 glm::degrees(m_angularVelocity.x), glm::degrees(m_angularVelocity.y), glm::degrees(m_angularVelocity.z), 
                 dt);
         Util::printMatrix(parentMatrix, "ParentMatrix:");
         Util::printMatrix(m_cameraTransform.viewMatrix, "ViewMatrix:");
         printf("\n\n");
*/

        if (m_childIDs.size() > 0)                  // If we actually have kids.
        {            
            for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
            {
                //LOG_DEBUG("\n\nI am entityID () %d Updating child with entityID %d.\n", m_entityID, child);
                Scene::getEntity(child)-> update(dt, m_transformMatrix);
            }
        }
}

void EntityCamera::draw(float /*t*/)
{

}

}