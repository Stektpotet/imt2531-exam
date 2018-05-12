#include <overkill/SeasonSystem.hpp>

namespace overkill
{
    GLuint SeasonSystem::m_terrainDisplayMaterials[3];
    SeasonSystem::DisplayMode SeasonSystem::m_displayMode;
    bool SeasonSystem::m_season_paused;
    bool SeasonSystem::m_day_night_paused;
    float SeasonSystem::m_seasonTime;
    float SeasonSystem::m_dayTime;



void SeasonSystem::Init()
{
    m_season_paused = false;
    m_day_night_paused = false;
    m_displayMode = REGULAR;
    m_terrainDisplayMaterials[0] = MaterialSystem::getIdByTag("terrain_regular");
    m_terrainDisplayMaterials[1] = MaterialSystem::getIdByTag("terrain_mixed");
    m_terrainDisplayMaterials[2] = MaterialSystem::getIdByTag("terrain_contour");

    m_seasonTime = 0;
    m_dayTime = 0;
}

float SeasonSystem::getSeasonTime()
{
    return m_seasonTime;
}

float SeasonSystem::getDayTime()
{
    return m_dayTime;
}

GLuint SeasonSystem::nextDisplayMode()
{
    m_displayMode = SeasonSystem::DisplayMode((m_displayMode + 1) % END_MODE);
    return m_terrainDisplayMaterials[m_displayMode];
}

void SeasonSystem::Update(const float deltaTime)
{
    if (!m_season_paused)
    {
        m_seasonTime += deltaTime * SeasonSystem::SEASON_SPEED;
    }
    if (!m_day_night_paused)
    {
        m_dayTime += deltaTime * SeasonSystem::DAY_NIGHT_SPEED;
    }
    HandleInput();
}

void SeasonSystem::HandleInput()
{
     if(Input::getKey(SeasonSystem::TOGGLE_DISPLAY_MODE))
     {
         LOG_INFO("Switching Terrain display mode!");
         GLuint matID = SeasonSystem::nextDisplayMode();
         for (auto& mesh : ModelSystem::getByTag("terrain").m_meshes)
         {
             mesh.m_materialID = matID;
         }
     }

     //SEASON CYCLE
     else if (Input::getKey(SeasonSystem::TOGGLE_SEASON_PAUSE))
     {
         m_season_paused = !m_season_paused;

         LOG_INFO("Season cycle %s ", m_season_paused ? "paused" : "un-paused");
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_WINTER))
     {
         LOG_INFO("SEASON SET: WINTER");

         m_seasonTime = 0.0f;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_SPRING))
     {
         LOG_INFO("SEASON SET: SPRING");
         m_seasonTime = 0.25f;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_SUMMER))
     {
         LOG_INFO("SEASON SET: SUMMER");
         m_seasonTime = 0.5f;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_AUTUMN))
     {
         LOG_INFO("SEASON SET: AUTUMN");
         m_seasonTime = 0.75f;
     }

     //DAY NIGHT CYCLE
     else if (Input::getKey(SeasonSystem::TOGGLE_DAY_NIGHT_PAUSE))
     {
         m_day_night_paused = !m_day_night_paused;
         LOG_INFO("Day/Night cycle %s ", m_day_night_paused ? "paused" : "un-paused");
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_MORNING))
     {
         LOG_INFO("TIME SET: MORNING");
         m_dayTime = 0.0f;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_NOON))
     {
         LOG_INFO("TIME SET: NOON");
         m_dayTime = 0.125f;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_AFTERNOON))
     {
         LOG_INFO("TIME SET: AFTERNOON");
         m_dayTime = 0.25f;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_NIGHT))
     {
         LOG_INFO("TIME SET: NIGHT");
         m_dayTime = 0.375f;
     }
}

}
