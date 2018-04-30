#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Scene.hpp>
#include <overkill/Entity.hpp>
#include <overkill/Input.hpp>


namespace overkill
{

struct CameraTransform      // Not to be confused with the cameras actual transform defined in Entity.
{                           // This struct is used to convey info to main and shader.
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::vec4 position;
};

enum CameraMode
{
    FREELOOK,
    ORBITAL
};

class EntityCamera : public Entity
{
private:
    float m_FOV;
    float m_aspectRatio;
    float m_nearClip;
    float m_farClip;
    enum CameraMode m_cameraMode;
    glm::vec3 m_modelSpacePos;

    glm::mat4 getViewMatrix(glm::mat4 parentMatrix = glm::mat4(1));
    glm::mat4 getModelToWorldMatrix(glm::mat4 parentMatrix = glm::mat4(1));

public:
    CameraTransform m_cameraTransform;

    EntityCamera(C::Tag entityTag, int entityID, 
                glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 rot = glm::vec3(0,0,0), 
                glm::vec3 vel = glm::vec3(0,0,0), glm::vec3 angVel = glm::vec3(0,0,0),
                enum CameraMode camMode = FREELOOK,
                float FOV = C::FOV, float aspectRatio = C::AspectRatio, 
                float nearClip = C::NearClip, float farClip = C::FarClip);

    void checkInput();
    void cycleMode();       // Cycles between the different camera modes. Currently FREELOOK and ORBITAL.

    void update(float dt, glm::mat4 parentMatrix = glm::mat4(1)) override final;
    void draw(float t) override final;
};

}