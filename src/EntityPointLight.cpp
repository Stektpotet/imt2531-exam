#include <overkill/EntityPointLight.hpp>

namespace overkill 
{


auto EntityPointLight::pack() -> PointLightBO
{
    return PointLightBO
    {
        glm::vec4{m_position.x, m_position.y, m_position.z, 0},
        glm::vec4{m_intensities.r, m_intensities.g, m_intensities.b, 0},
        m_constantFalloff,
        m_linearFalloff,
        m_quadraticFalloff
    };
}

}