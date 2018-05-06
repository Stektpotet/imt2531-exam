#include <overkill/Util.hpp>

namespace overkill 
{

auto Transform::modelToWorld() -> glm::mat4
{
    auto m2w = glm::scale(glm::mat4(1), m_scale);

    m2w = glm::rotate(m2w, m_rotation.x, right);
    m2w = glm::rotate(m2w, m_rotation.y, up);
    m2w = glm::rotate(m2w, m_rotation.z, forward);

    m2w = glm::translate(m2w, m_position);
    return m2w;
}

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

#include <fstream>
#include <streambuf>

std::ifstream t("file.txt");


// @ref Inspired by https://stackoverflow.com/a/2602060 - 06.05.2018
//    and http://insanecoding.blogspot.no/2011/11/how-to-read-in-file-in-c.html - 06.05.2018
auto Util::fileToString(const std::string& filepath, std::string* outString) -> C::Err
{
    std::ifstream infile(filepath);
    
    if (!infile) {
        return 1;
    }
    // Old method
 /*   std::stringstream ss;
    ss << infile.rdbuf();
    *outString = ss.str(); */

    // New and possible faster method
    // UPDATE: Holy fuck this was 10 times faster than old method when loading dragon.yml (400k verts, 800k tris).. JSolsvik - 06.05.2018
    infile.seekg(0, std::ios::end);
    outString->resize(infile.tellg());
    infile.seekg(0, std::ios::beg);
    infile.read(outString->data(), outString->size());
    infile.close();

    return 0;
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

//TODO move it
auto Util::packNormal(float x, float y, float z) -> GLint
{
    float magnitude = sqrt(x * x + y * y + z * z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    const auto DISCARDMASK = 1023;
    const auto MAX = 511;        //01 1111 1111
    GLint r = ((GLint(z * MAX) & DISCARDMASK) << 20) | ((GLint(y * MAX) & DISCARDMASK) << 10) | (GLint(x * MAX) & DISCARDMASK);
    return r;
}

//Too lossy for our liking, we don't use this any more...
auto Util::packUV(float u, float v) -> GLushort
{
    const auto MAX = 255; // 1111 1111

    GLushort r = (GLubyte(v * MAX)) << 8 | (GLubyte(u * MAX));
    return r;
}

#define DEBUG 0

void Util::printMatrix(const glm::mat4& m, const std::string & name)
{

#if DEBUG

    LOG_DEBUG("\n%s\t 4x4\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n",
        name.c_str(),
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3], 
        m[2][0], m[2][1], m[2][2], m[2][3], 
        m[3][0], m[3][1], m[3][2], m[3][3]);
#endif

}
}
