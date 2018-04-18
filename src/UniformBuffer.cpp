#include <overkill/UniformBuffer.hpp>

namespace overkill
{
//lightblock
//mvpblock
//cam pos 
struct uniformBlock
{
    float camera_position[4];
    float light_position[4];
    float light_diffuse[4];
} shader_data;


}