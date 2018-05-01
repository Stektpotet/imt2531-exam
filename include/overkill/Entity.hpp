#pragma once
#include <string>
#include <vector>

#include <overkill/Config.hpp>

namespace overkill 
{


class Entity
{

protected:
    C::Tag m_entityTag;                 // Tag that is uniqe for every entity type; cameraEntiry, lightEntiry, modelEntity.    
    int m_entityID;                     // ID scene uses to keep track of all entities. Also parentID.
    std::vector<int> m_childIDs;        // The ids of all the entity's children.
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::vec3 m_velocity;
    glm::vec3 m_angularVelocity;
    glm::mat4 m_transformMatrix;

public:
    Entity(C::Tag entityTag, int entityID, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec3 vel, glm::vec3 angVel);
    virtual ~Entity(){}
    C::Tag getTag();
    int getEntityID();
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getVelocity();
    glm::vec3 getAngularVelocity();
    void addChild(int childID);
    void setEntityID(int ID);
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);
    void setScale(glm::vec3 scale = glm::vec3(1));
    void setVelocity(glm::vec3 vel);
    void setAngularVelocity(glm::vec3 angVel); 

    virtual void update(float dt, glm::mat4 /*transformMatrix*/ = glm::mat4(1));  
    virtual void draw(float /*t*/);
};

}