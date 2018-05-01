#pragma once

#include <overkill/Entity.hpp>
#include <overkill/Scene.hpp>

namespace overkill
{

struct DirectionalLightBO
{
    glm::vec4 direction;     //16 -> 16
    glm::vec4 intensities;   //16 -> 32
};


class EntityDirectionalLight : public Entity
{
public:
    glm::vec3 m_intensities;

    EntityDirectionalLight(C::Tag tag,
					       int id,
					       glm::vec3 rotation,
                           glm::vec3 angVel,
                           glm::vec3 intensities)
    :Entity(tag,
             id,
			 glm::vec3(0),
             rotation,
             glm::vec3(0),
             glm::vec3(0),
             angVel)
    ,m_intensities(intensities)
    {}

    auto pack() ->DirectionalLightBO ;

    glm::mat4 getModelMatrix(glm::mat4 parentModelMatrix = glm::mat4(1));
    void update(float dt, glm::mat4 parentMatrix = glm::mat4(1)) override final;
};


}
