#pragma once
#include <glm/glm.hpp>
#include <string>

namespace overkill::C
{
    using Tag = std::string;
    using ID  = int;

    constexpr int VersionMajor = 4;
    constexpr int VersionMinor = 1;
    constexpr int WinWidth = 800;
    constexpr int WinHeight = 600;
    constexpr char WinName[] = "Assignment 2 - Cube";
    constexpr float FOV = 90.0f;
    constexpr float ZoomSensitivity = 0.1f;
    constexpr float AspectRatio = (float)WinWidth / (float)WinHeight;
    constexpr float NearClip = 0.1f;
    constexpr float FarClip  = 100.0f;
    const glm::vec4 ClearColor { 1.0f, .8f, .6f, 1.0f};
    constexpr char PathBaseShader[] = "assets/shaders/base.shader";
    constexpr char PathBaseTexture[] = "assets/textures/Checkers.jpg";
}
