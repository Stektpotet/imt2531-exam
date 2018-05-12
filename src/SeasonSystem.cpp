#include <overkill/SeasonSystem.hpp>

namespace overkill
{
    GLuint SeasonSystem::m_terrainDisplayMaterials[3];
    SeasonSystem::DisplayMode SeasonSystem::displayMode;


void SeasonSystem::Init()
{
    displayMode = REGULAR;
    m_terrainDisplayMaterials[0] = MaterialSystem::getIdByTag("terrain_regular");
    m_terrainDisplayMaterials[1] = MaterialSystem::getIdByTag("terrain_mixed");
    m_terrainDisplayMaterials[2] = MaterialSystem::getIdByTag("terrain_contour");

}

GLuint SeasonSystem::nextDisplayMode()
{
    displayMode = SeasonSystem::DisplayMode((displayMode + 1) % END_MODE);
    return m_terrainDisplayMaterials[displayMode];
}

}
