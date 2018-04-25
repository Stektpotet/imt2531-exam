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
    printf("%i\n", r);
    return r;
}

auto Util::packUV(float u, float v) -> GLushort
{
    const auto MAX = 255; // 1111 1111
    const auto DISCARDMASK = 511; //1111 1111 1111 1111

    GLushort su = (GLushort(u * MAX) & DISCARDMASK) << 8;
    GLushort sv = (GLushort(v * MAX) & DISCARDMASK);
    GLushort r = (GLushort(u * MAX) & DISCARDMASK) << 8 | (GLushort(v * MAX) & DISCARDMASK);
    return r;
}

void Util::printMatrix(const glm::mat4& m, const std::string & name)
{
    LOG_DEBUG("\n%s\t 4x4\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n",
        name.c_str(),
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3], 
        m[2][0], m[2][1], m[2][2], m[2][3], 
        m[3][0], m[3][1], m[3][2], m[3][3]);
}

}
