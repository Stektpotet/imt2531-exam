#include <overkill/EntityGlider.hpp>
#include <overkill/Scene.hpp>

namespace overkill
{

EntityGlider::EntityGlider(
    C::Tag  modelTag, 
    C::Tag  EntityTag, 
    int entityID, 
    glm::vec3 pos, 
    glm::vec3 rot, 
    glm::vec3 scale, 
    glm::vec3 vel, 
    glm::vec3 angVel
) : EntityModel(
    modelTag, 
    EntityTag,
    entityID,
    pos, 
    rot, 
    scale,
    vel, 
    angVel
){}

void EntityGlider::addSpawnLocation(Entity * spawnPos)
{
    m_spawnLocations.push_back(spawnPos);
}


glm::mat4 EntityGlider::getModelMatrix(glm::mat4 parentModelMatrix)
{
    glm::mat4 model = glm::scale(glm::mat4(1), m_scale);

    model = glm::rotate(model, -m_angularVelocity.y*m_velocity.x, glm::vec3(0,0,1));
    model = glm::rotate(model, m_rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, m_rotation.x, glm::vec3(1,0,0));

    glm::mat4 worldPos = glm::translate(glm::mat4(1), m_position);  //Translate in world space.
    model = parentModelMatrix * worldPos * model;
    return model;
}
void EntityGlider::update(float dt, glm::mat4 parentMatrix)
{

#if DEBUG
    if (m_modelID == -1)
    {
        LOG_WARN("Model ID is not set due to bad construct params. Model will not be drawn.");
    }
#endif

    m_rotation += m_angularVelocity * dt;

    m_angularVelocity *= 0.9f * dt; //angular drag
    

    m_speed += m_acceleration;
    m_acceleration = glm::sign(m_acceleration) * glm::clamp(glm::abs(m_acceleration) - 0.5f, 0.0f, ACCELERATION_STEP);

    m_velocity = m_speed * glm::vec3(sin(m_rotation.y)*cos(m_rotation.x), -sin(m_rotation.x), cos(m_rotation.y)*cos(m_rotation.x));

    m_position += m_velocity * dt;
    m_transformMatrix = getModelMatrix(parentMatrix);
    
    if (m_childIDs.size() > 0)                  // If we actually have kids.
    {            
        for (const auto child : m_childIDs)     // For every childID in childIDs-vector.
        {
            //LOG_DEBUG("\n\nI am entityID %d Updating child with entityID %d.\n", m_entityID, child);
            Scene::getEntity(child)-> update(dt, m_transformMatrix);
        }
    }
    handleInput();
}

float EntityGlider::getSpeed_KMPH() const
{
    return m_speed * SPEED_TO_KM_H;
}
float EntityGlider::getAcceleration() const
{
    return m_acceleration;
}
void EntityGlider::handleInput()
{
    m_angularVelocity.x += Input::m_navKeyPressed[W] ? 0.3f : 0;
    m_angularVelocity.x -= Input::m_navKeyPressed[S] ? 0.3f : 0;
    m_angularVelocity.y += Input::m_navKeyPressed[A] ? 0.5f : 0;
    m_angularVelocity.y -= Input::m_navKeyPressed[D] ? 0.5f : 0;

    if (Input::getKey(GLFW_KEY_COMMA))
    {
        m_acceleration += ACCELERATION_STEP;
    }
    if (Input::getKey(GLFW_KEY_PERIOD))
    {
        m_acceleration -= ACCELERATION_STEP;
    }

    if (Input::getKey(GLFW_KEY_F))
    {
        m_currentSpawnIndex = (m_currentSpawnIndex + 1) % m_spawnLocations.size();
        auto spawn = m_spawnLocations.at(m_currentSpawnIndex);
        setPosition(spawn->getPosition());        
        setRotation(glm::radians(spawn->getRotation()));

    }
    else if (Input::getKey(GLFW_KEY_R))
    {
        auto spawn = m_spawnLocations.at(m_currentSpawnIndex);
        setPosition(spawn->getPosition());
        setRotation(glm::radians(spawn->getRotation()));
    }
}

}