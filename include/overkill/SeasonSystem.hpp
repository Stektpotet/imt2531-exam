#pragma once
#include <overkill/MaterialSystem.hpp>
#include <overkill/Input.hpp>
#include <math.h>       /* fmod */

namespace overkill
{

class SeasonSystem 
{
private:

    static const int TOGGLE_DISPLAY_MODE = GLFW_KEY_O;


    static const int TOGGLE_SEASON_PAUSE = GLFW_KEY_5;

    static const int SWITCH_TO_WINTER = GLFW_KEY_1;
    static const int SWITCH_TO_SPRING = GLFW_KEY_2;
    static const int SWITCH_TO_SUMMER = GLFW_KEY_3;
    static const int SWITCH_TO_AUTUMN = GLFW_KEY_4;


    static const int TOGGLE_DAY_NIGHT_PAUSE = GLFW_KEY_0;

    static const int SWITCH_TO_MORNING      = GLFW_KEY_6;
    static const int SWITCH_TO_NOON         = GLFW_KEY_7;
    static const int SWITCH_TO_AFTERNOON    = GLFW_KEY_8;
    static const int SWITCH_TO_NIGHT        = GLFW_KEY_9;



    static inline const float SEASON_SPEED     = 0.025f;
    static inline const float DAY_NIGHT_SPEED  = 0.5f;

    static inline const C::Tag SEASON_NAME[4] = {
        "Winter","Spring","Summer","Autumn"
    };

    static inline const C::Tag MONTH_NAME[12] = {
        "December", "January", "February", "March", 
        "April", "May", "June", "July", "August",
        "September", "October", "November", 
    };

    static inline const C::Tag DAY_PHASE_NAME[7] = {
        "Morning", "Noon", "Afternoon", "Evening", "Night", "Midnight", "Too Early For Me"
    };

public:

    static enum DisplayMode : int
    {
        REGULAR = 0,
        MIXED = 1,
        CONTOUR = 2,
        END_MODE = 3
    };

    static enum Season : int
    {
        WINTER = 0,
        SPRING = 1,
        SUMMER = 2,
        AUTUMN = 3,
        END_SEASON = 4
    };

    static enum DayPhase : int
    {
        MORNING = 0,
        NOON = 1,
        AFTERNOON = 2,
        NIGHT = 3,
        END_DAY = 4
    };
private:

    //SeasonSystem States

    static GLuint m_terrainDisplayMaterials[3];
    static DisplayMode m_displayMode;
    static bool m_season_paused;
    static bool m_day_night_paused;

    static float m_seasonTime;
    static float m_dayTime;

    static int m_currentSeason;
    static int m_currentMonth;
    static int m_currentDayPhase;

public:
    //Must be called after other system inits.
    static void Init();

    static float getSeasonTime();
    static const C::Tag& getSeasonName();
    static const C::Tag& getMonthName();
    static const C::Tag& getDayPhaseName();

    static float getDayTime();

    static GLuint nextDisplayMode();

    static void Update(const float dt);
    static void HandleInput();


};

}