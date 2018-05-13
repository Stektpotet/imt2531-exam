#include <overkill/SeasonSystem.hpp>

namespace overkill
{
    GLuint SeasonSystem::m_terrainDisplayMaterials[3];
    SeasonSystem::DisplayMode SeasonSystem::m_displayMode;
    bool SeasonSystem::m_season_paused;
    bool SeasonSystem::m_day_night_paused;
    float SeasonSystem::m_seasonTime;
    float SeasonSystem::m_dayTime;


    int SeasonSystem::m_currentSeason;
    int SeasonSystem::m_currentMonth;
    int SeasonSystem::m_currentDayPhase;

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

const C::Tag & SeasonSystem::getSeasonName()
{
    return SEASON_NAME[m_currentSeason];
}

const C::Tag & SeasonSystem::getMonthName()
{
    return MONTH_NAME[m_currentMonth];
}

const C::Tag & SeasonSystem::getDayPhaseName()
{
    return DAY_PHASE_NAME[m_currentDayPhase];
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
        m_currentSeason = int(fmod(m_seasonTime, 1.0f) * 4);
        m_currentMonth = int(fmod(m_seasonTime, 1.0f) * 12);
    }
    if (!m_day_night_paused)
    {
        m_dayTime += deltaTime * SeasonSystem::DAY_NIGHT_SPEED;
        m_currentDayPhase = int(fmod(m_dayTime, 1.0f) * 7);
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
         m_currentSeason = 0;
         m_currentMonth = 0;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_SPRING))
     {
         LOG_INFO("SEASON SET: SPRING");
         m_seasonTime = 0.25f;
         m_currentSeason = 1;
         m_currentMonth = 3;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_SUMMER))
     {
         LOG_INFO("SEASON SET: SUMMER");
         m_seasonTime = 0.5f;
         m_currentSeason = 2;
         m_currentMonth = 6;
     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_AUTUMN))
     {
         LOG_INFO("SEASON SET: AUTUMN");
         m_seasonTime = 0.75f;
         m_currentSeason = 3;
         m_currentMonth = 9;
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
         m_currentDayPhase = 0;

     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_NOON))
     {
         LOG_INFO("TIME SET: NOON");
         m_dayTime = 0.125f;
         m_currentDayPhase = 1;

     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_AFTERNOON))
     {
         LOG_INFO("TIME SET: AFTERNOON");
         m_dayTime = 0.25f;
         m_currentDayPhase = 2;

     }
     else if (Input::getKey(SeasonSystem::SWITCH_TO_NIGHT))
     {
         LOG_INFO("TIME SET: EVENING");
         m_dayTime = 0.375f;
         m_currentDayPhase = 3;

     }
}

}
