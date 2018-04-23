#pragma once
#include <glm/glm.hpp>
#include <string>

namespace overkill::C
{
    using Tag = std::string;
    using ID  = int;

    constexpr int VersionMajor = 4;
    constexpr int VersionMinor = 1;
    constexpr int WinWidth = 1000;
    constexpr int WinHeight = 800;
    constexpr char WinName[] = "Assignment 2 - Cube";
    constexpr float FOV = 88.0f;
    constexpr float MaxFOV = 88.2f;
    constexpr float MinFOV = 87.97f;
    constexpr float ZoomSensitivity = 0.0033f;
    constexpr float LookSensitivity = 0.013f;
    constexpr float PanSensitivity = 9;
    constexpr float AspectRatio = (float)WinWidth / (float)WinHeight;
    constexpr float NearClip = 0.1f;
    constexpr float FarClip  = 100.0f;
    constexpr float CameraOffset = -70;      //How far back the camera is from the center of the scene.
    const glm::vec4 ClearColor { 0.05f, 0.06f, 0.075f, 1.0f };
    constexpr char PathBaseShader[] = "assets/shaders/base.shader";
    constexpr char PathBaseTexture[] = "assets/textures/Checkers.jpg";
    constexpr float PI = 3.14159265359f;

    constexpr char TexturesFolder[]  = "assets/textures";
    constexpr char ShadersFolder[]   = "assets/shaders";
    constexpr char MaterialsFolder[] = "assets/materials";
    constexpr char ModelsFolder[]    = "assets/models";
    constexpr char ScenesFolder[]    = "assets/scenes";

       
}
