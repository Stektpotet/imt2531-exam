#pragma once
#include <glm/glm.hpp>
#include <string>
#include <PMS/logger.h>

namespace overkill
{


class C
{
public:
    using Tag = std::string;
    using ID  = int;

    constexpr static unsigned int StrSize = 80;
    constexpr static float PI             = 3.14159265359f;
    constexpr static char  Configpath[C::StrSize] = "config.yml";

    static int   GLVersionMajor  ;
    static int   GLVersionMinor  ;
    static int   WinWidth        ;
    static int   WinHeight       ;
    static char  WinName[StrSize];
    
    static float FOV             ;
    static float MaxFOV          ;
    static float MinFOV          ;
    static float ZoomSensitivity ;
    static float LookSensitivity ;
    static float PanSensitivity  ;
    static float AspectRatio     ;
    static float NearClip        ;
    static float FarClip         ;
    static float CameraOffset    ;
    static glm::vec4 ClearColor  ;

    static char TexturesFolder[StrSize];
    static char ShadersFolder[StrSize];
    static char MaterialsFolder[StrSize];
    static char ModelsFolder[StrSize];
    static char ScenesFolder[StrSize];

    static unsigned int MAX_LIGHTS; //inject it into shaders
};

}
