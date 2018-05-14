#pragma once

#include <overkill/EntityModel.hpp>

namespace overkill
{
class EntityGlider: public EntityModel
{
    const float ACCELERATION_STEP = 2.5f;

    const float SPEED_TO_KM_H = 30.0f;

protected:
    float m_speed = 5.0f;
    float m_acceleration = 0;

    int m_currentSpawnIndex = 0;
    std::vector<Entity*> m_spawnLocations;


    void handleInput();


public:

    EntityGlider(C::Tag modelTag, C::Tag EntityTag, int entityID,
        glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 rot = glm::vec3(0, 0, 0),
        glm::vec3 scale = glm::vec3(1, 1, 1), glm::vec3 vel = glm::vec3(0, 0, 0),
        glm::vec3 angVel = glm::vec3(0, 0, 0));
       
    void addSpawnLocation(Entity* spawnPos);
    //TODO add set currentpos = spawn

    glm::mat4 getModelMatrix(glm::mat4 parentModelMatrix = glm::mat4(1));
    
    virtual void update(float dt, glm::mat4 parentMatrix = glm::mat4(1)) override;


    float getSpeed_KMPH() const;
    float getAcceleration() const;
};

}