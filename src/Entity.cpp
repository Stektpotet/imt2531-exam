#include <overkill/Entity.hpp>

namespace overkill
{
    Entity::Entity(C::Tag entityTag, int entityID = -1, glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), 
                            glm::vec3 scale = glm::vec3(1), glm::vec3 vel = glm::vec3(0), glm::vec3 angVel = glm::vec3(0))
    {
        m_entityID = entityID;
        m_entityTag = entityTag;
        m_position = pos;
        m_rotation = glm::radians(rot);
        m_scale = scale;
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
    void Entity::setScale(glm::vec3 scale)
    {   
        m_scale = scale;   
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
        

        // LOG_DEBUG("Update()\n\nentityID %d, entiryTag %s, \nm_position %f, %f, %f\nm_rotation %f, %f, %f\nm_angVel %f, %f, %f\ndeltatime %f\n", 
        //         m_entityID,  m_entityTag.data(),
        //         m_position.x, m_position.y, m_position.z, 
        //         glm::degrees(m_rotation.x), glm::degrees(m_rotation.y), glm::degrees(m_rotation.z), 
        //         glm::degrees(m_angularVelocity.x), glm::degrees(m_angularVelocity.y), glm::degrees(m_angularVelocity.z), 
        //         dt);    
        // printf("\n\n");

    }


    void Entity::draw(float /*t*/)
    {

    }
}