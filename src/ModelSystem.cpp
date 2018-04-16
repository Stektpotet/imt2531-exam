#include <overkill/ModelSystem.hpp>

namespace overkill
{

std::vector<Model> ModelSystem::m_models;
std::unordered_map<C::Tag, C::ID> ModelSystem::m_mapModelID;

auto ModelSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapModelID[tag];
}

auto ModelSystem::getByTag(const C::Tag& tag) -> const Model&
{
    return ModelSystem::m_models[m_mapModelID[tag]];
}

auto ModelSystem::getById(C::ID modelID) -> const Model&
{
    return ModelSystem::m_models[modelID];
}
//TODO move it
auto packNormal(float x, float y, float z) -> GLint
{
	float magnitude = sqrt(x * x + y * y + z * z);
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;

	const auto MAX = 511;		//01 1111 1111
	const auto MIN = -512;		//10 0000 0000
	// -1 * 511 = -511
	GLint ix = (GLint(x * MAX) & 1023);
	GLint iy = (GLint(y * MAX) & 1023);
	GLint iz = (GLint(z * MAX) & 1023);
	//1*511 =	511
	//			01 0000 0000
	//-1*511 = -511
	//			10 0000 0001
	GLint r = (iz << 20) | (iy << 10) | ix;
	return r;
}

void ModelSystem::load()
{
    auto vertices = std::vector<Vertex> {

        // Cube 1
        {  -0.5f, -0.5f,  0.5f,  packNormal(-1,-1, 1),   0, 0,  255, 0, 0, 255 },
        {   0.5f, -0.5f,  0.5f,  packNormal(1, -1, 1),   1, 0,  0, 255, 0, 255 },
        {   0.5f,  0.5f,  0.5f,  packNormal(1,  1, 1),   1, 1,  0, 0, 255, 255 },
        {  -0.5f,  0.5f,  0.5f,  packNormal(-1, 1, 1),   0, 1,  255, 255, 255, 255 },

        {  -0.5f, -0.5f, -0.5f,  packNormal(-1,-1,-1),   1, 0,  255, 255, 255, 255 },
        {  -0.5f,  0.5f, -0.5f,  packNormal(-1, 1,-1),   1, 1,  255, 255, 255, 255 },

        {   0.5f, -0.5f, -0.5f,  packNormal(1,-1,-1),   0, 0,  255, 255, 255, 255 },
        {   0.5f,  0.5f, -0.5f,  packNormal(1, 1,-1),   0, 1,  255, 255, 255, 255 },

        {  -0.5f, -0.5f, -0.5f,  packNormal(-1, -1, -1),   0, 1,  255, 255, 255, 255 },
        {   0.5f, -0.5f, -0.5f,  packNormal( 1, -1, -1),   1, 1,  255, 255, 255, 255 },

        {  -0.5f,  0.5f, -0.5f,  packNormal(-1, 1, -1),   0, 0,  255, 255, 255, 255 },
        {   0.5f,  0.5f, -0.5f,  packNormal( 1, 1, -1),   1, 0,  255, 255, 255, 255 },

        {  -0.5f,  0.5f,  0.5f,  packNormal(-1, 1, 1),   0, 1,  255, 255, 255, 255 },
        {   0.5f,  0.5f,  0.5f,  packNormal( 1, 1, 1),   1, 1,  255, 255, 255, 255 },


        //// Cube 2
        //{  -1.0f, -1.0f,  1.0f,  1,   0, 0,  255, 255, 255, 80 },
        //{   1.0f, -1.0f,  1.0f,  1,   1, 0,  255, 255, 255, 80 },
        //{   1.0f,  1.0f,  1.0f,  1,   1, 1,  255, 255, 255, 80 },
        //{  -1.0f,  1.0f,  1.0f,  1,   0, 1,  255, 255, 255, 80 },

        //{  -1.0f, -1.0f, -1.0f,  1,   1, 0,  255, 255, 255, 80 },
        //{  -1.0f,  1.0f, -1.0f,  1,   1, 1,  255, 255, 255, 80 },

        //{   1.0f, -1.0f, -1.0f,  1,   0, 0,  255, 255, 255, 80 },
        //{   1.0f,  1.0f, -1.0f,  1,   0, 1,  255, 255, 255, 80 },

        //{  -1.0f, -1.0f, -1.0f,  1,   0, 1,  255, 255, 255, 80 },
        //{   1.0f, -1.0f, -1.0f,  1,   1, 1,  255, 255, 255, 80 },

        //{  -1.0f,  1.0f, -1.0f,  1,   0, 0,  255, 255, 255, 80 },
        //{   1.0f,  1.0f, -1.0f,  1,   1, 0,  255, 255, 255, 80 },

        //{  -1.0f,  1.0f,  1.0f,  1,   0, 1,  255, 255, 255, 80 },
        //{   1.0f,  1.0f,  1.0f,  1,   1, 1,  255, 255, 255, 80 },

    };

    std::vector<GLuint> indicies {

        // Cube 1
        0, 1, 2,
        2, 3, 0,

        4, 0, 3,
        3, 5, 4,

        1, 6, 7,
        7, 2, 1,

        8, 9, 1,
        1, 0, 8,

        10, 11, 9,
        9,  8,  10,

        12, 13, 11,
        11, 10, 12,


        // Cube 2
        0+14, 1+14, 2+14,
        2+14, 3+14, 0+14,

        4+14, 0+14, 3+14,
        3+14, 5+14, 4+14,

        1+14, 6+14, 7+14,
        7+14, 2+14, 1+14,

        8+14, 9+14, 1+14,
        1+14, 0+14, 8+14,

        10 + 14, 11 + 14, 9 + 14,
        9 + 14,  8 + 14,  10 + 14,

        12+14, 13+14, 11+14,
        11+14, 10+14, 12+14,
    };

    Model model{vertices};
    model.pushMesh(indicies, 0,0);

    m_mapModelID["cube"] = m_models.size();
    m_models.push_back(model);
}

}
