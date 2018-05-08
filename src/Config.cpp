#include <overkill/Config.hpp>


namespace overkill
{

int   C::GLVersionMajor  = 4;
int   C::GLVersionMinor  = 1;
int   C::WinWidth        = 1000;
int   C::WinHeight       = 800;
char  C::WinName[C::StrSize] = "Assignment 2 - Cube";
float C::AspectRatio     = (float)WinWidth / (float)WinHeight;

float C::FOV             = 90.0f;
float C::MaxFOV          = 90.2f;
float C::MinFOV          = 87.97f;
float C::ZoomSensitivity = 0.0033f;
float C::LookSensitivity = 0.013f;
float C::PanSensitivity  = 9;
float C::NearClip        = 0.1f;
float C::FarClip         = 100.0f;
float C::CameraOffset    = -70;      //How far back the camera is from the center of the scene.
glm::vec4 C::ClearColor  = glm::vec4{ 0.05f, 0.06f, 0.075f, 1.0f }; // Sexy blue

char C::TexturesFolder[C::StrSize]  = "assets/textures";
char C::ShadersFolder[C::StrSize]   = "assets/shaders";
char C::MaterialsFolder[C::StrSize] = "assets/materials";
char C::ModelsFolder[C::StrSize]    = "assets/models";
char C::ScenesFolder[C::StrSize]    = "assets/scenes";
char C::ObjsFolder[C::StrSize]    = "assets/objs";

std::string C::PythonPath = "python3";

unsigned int C::MAX_LIGHTS = 8; //inject it into shaders

GLFWwindow* C::window;

}