#include <overkill/Entity.hpp>

namespace overkill
{
    Entity::Entity(C::Tag entityTag, int entityID = -1, glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 rot = glm::vec3(0,0,0), 
                            glm::vec3 vel = glm::vec3(0,0,0), glm::vec3 angVel = glm::vec3(0,0,0))
    {
        m_entityID = entityID;
        m_entityTag = entityTag;
        m_position = pos;
        m_rotation = glm::radians(rot);
        m_velocity = vel;
        m_angularVelocity = glm::radians(angVel);
    }
     
    C::Tag Entity::getTag()
    {
        return m_entityTag;
    }

    int Entity::getEntityID() 
    {   
        return m_entityID;   
    }

    glm::vec3 Entity::getPosition()
    {   
        return m_position;  
    }

    glm::vec3 Entity::getRotation()
    {   
        return glm::degrees(m_rotation);  
    }

    glm::vec3 Entity::getVelocity()
    {   
        return m_velocity;  
    }

    glm::vec3 Entity::getAngularVelocity()
    {   
        return glm::degrees(m_angularVelocity);   
    }

    void Entity::addChild(int childID)
    {   
        m_childIDs.push_back(childID);  
    }

    void Entity::setEntityID(int ID)
    {   
        m_entityID = ID;  
    }

    void Entity::setPosition(glm::vec3 pos)
    {   
        m_position = pos;   
    }

    void Entity::setRotation(glm::vec3 rot)
    {   
        m_rotation = glm::radians(rot);   
    }

    void Entity::setVelocity(glm::vec3 vel)
    {   
        m_velocity = vel;   
    }

    void Entity::setAngularVelocity(glm::vec3 angVel)
    {   
        m_angularVelocity = glm::radians(angVel); 
    }

    void Entity::update(float dt, glm::mat4 /*transformMatrix*/)
    {
        m_position += m_velocity * dt;
        m_rotation += m_angularVelocity * dt;
        

        printf("\n\nUpdate()\n\nentityID %d, entiryTag %s, \nm_position %f, %f, %f\nm_rotation %f, %f, %f\nm_angVel %f, %f, %f\ndeltatime %f\n", 
                m_entityID,  m_entityTag.data(),
                m_position.x, m_position.y, m_position.z, 
                m_rotation.x, m_rotation.y, m_rotation.z, 
                m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z, 
                dt);    
        }

    void Entity::draw(float t)
    {

    }
}