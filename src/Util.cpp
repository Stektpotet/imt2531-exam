#include <overkill/Util.hpp>

namespace overkill 
{

auto Util::fileToString(const std::string& filepath) -> std::string
{
    std::ifstream infile(filepath);

    if (!infile) {
        LOG_ERROR("file not found: %s", filepath.data());
    }
    std::stringstream ss;
    ss << infile.rdbuf();
    return ss.str();
}


void Util::everyTwoSeconds(float t)
{
    static float twoSecondTick = 0.0f;
    static float twoSecondTickShader = 1.0f;
    if (t - twoSecondTick > 2.0f) 
    {
        //Watcher::scanFiles();
        MaterialSystem::reload();
        twoSecondTick += 2;
    }

    if (t - twoSecondTickShader > 2.0f) {

        ShaderSystem::reload();
        twoSecondTickShader += 2;
    }
};

}
