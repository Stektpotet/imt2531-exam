#pragma once

#include <string>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Config.hpp>

namespace overkill 
{

class Entity
{
protected:
    C::Tag m_tag;
    int m_entityID;                 // ID scene uses to keep track of all entities. Also parentID.
    int m_parentID;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_velocity;
    glm::vec3 m_angularVelocity;

public:
    Entity(C::Tag tag, int m_parentID, glm::vec3 pos, glm::vec3 rot, glm::vec3 vel, glm::vec3 angVel);
    int getParent();
    int getEntityID();
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getVelocity();
    glm::vec3 getAngularVelocity();
    void setParent(int parentID);
    void setEntityID(int ID);
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);
    void setVelocity(glm::vec3 vel);
    void setAngularVelocity(glm::vec3 angVel); 

    void update(float dt);  
};

}