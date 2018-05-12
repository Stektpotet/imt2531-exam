#pragma once
#include <overkill/MaterialSystem.hpp>

namespace overkill
{

class SeasonSystem 
{
    static GLuint m_terrainDisplayMaterials[3];

public:

    //Must be called after other system inits.
    static void Init();

    static GLuint nextDisplayMode();

    static enum DisplayMode : int
    {
        REGULAR  = 0,
        MIXED    = 1,
        CONTOUR  = 2,
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

    static DisplayMode displayMode;

};

}