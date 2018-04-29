#pragma once

#include <overkill/Entity.hpp>
#include <overkill/Scene.hpp>

namespace overkill 
{

struct PointLightBO 
{
    glm::vec4 position;     //16 -> 16
    glm::vec4 intensities;  //16 -> 32
    float constant;         //4  -> 36
    float linear;           //4  -> 40
    float quadratic;        //4  -> 44
    float m_padding;        //4  -> 48
};


class EntityPointLight : public Entity
{
public:
    glm::vec3 m_intensities;
    float m_constantFalloff;
    float m_linearFalloff;
    float m_quadraticFalloff;

    EntityPointLight(C::Tag tag,
                     C::ID id,
                     glm::vec3 position,
                     glm::vec3 velocity,
                     glm::vec3 intensities,
                     float constantFalloff,
                     float linearFalloff,
                     float quadraticFalloff)
    :Entity(tag, 
             id, 
             position,  
             glm::vec3(0), 
             glm::vec3(0),
             velocity, 
             glm::vec3(0))
    ,m_intensities(intensities)
    ,m_constantFalloff(constantFalloff)
    ,m_linearFalloff(linearFalloff)
    ,m_quadraticFalloff(quadraticFalloff)
    {}

    auto pack() -> PointLightBO;

    glm::mat4 getModelMatrix(glm::mat4 parentModelMatrix = glm::mat4(1));
    void update(float dt, glm::mat4 parentMatrix = glm::mat4(1)) override final;
};


}