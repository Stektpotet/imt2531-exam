#include <overkill/Entity.hpp>

namespace overkill
{
    Entity::Entity(C::Tag tag, glm::vec3 pos, glm::vec3 rot, glm::vec3 vel, glm::vec3 angVel)
    {
        m_tag = tag;
        m_position = pos;
        m_rotation = rot;
        m_velocity = vel;
        m_angularVelocity = angVel;
    }
    
    glm::vec3 Entity::getPosition()
    {   return m_position;  }

    glm::vec3 Entity::getRotation()
    {   return m_rotation;  }

    glm::vec3 Entity::getVelocity()
    {   return m_velocity;  }

    glm::vec3 Entity::getAngularVelocity()
    {   return m_angularVelocity;   }

    void Entity::setPosition(glm::vec3 pos)
    {   m_position = pos;   }

    void Entity::setRotation(glm::vec3 rot)
    {   m_rotation = rot;   }

    void Entity::setVelocity(glm::vec3 vel)
    {   m_velocity = vel;   }

    void Entity::setAngularVelocity(glm::vec3 angVel)
    {   m_angularVelocity = angVel; }

    void Entity::update()
    {
        m_position += m_velocity;
        m_rotation += m_angularVelocity;
    }

}