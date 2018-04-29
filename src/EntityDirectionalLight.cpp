#include <overkill/EntityDirectionalLight.hpp>

namespace overkill
{
    auto EntityDirectionalLight::pack() -> DirectionalLightBO
    {
        return DirectionalLightBO
        {
            //euler angles to vector conversion
            glm::vec4{ 
                cos(m_rotation.y)*cos(m_rotation.x), 
                sin(m_rotation.y)*cos(m_rotation.x),
                sin(m_rotation.z),
                0
            },
            glm::vec4{ m_intensities.r, m_intensities.g, m_intensities.b, 0 },
        };
    }

}