#include <glm/glm.hpp>

namespace overkill::C
{
    const int VersionMajor = 4;
    const int VersionMinor = 1;
    const int WinWidth = 800;
    const int WinHeight = 600;
    const char* WinName = "Assignment 2 - Cube";
    const float FOV = 90.0f;
    const float AspectRatio = (float)WinWidth / (float)WinHeight;
    const float NearClip = 0.1f;
    const float FarClip  = 100.0f;
    const glm::vec4 ClearColor { 1.0f, .8f, .6f, 1.0f};
}
