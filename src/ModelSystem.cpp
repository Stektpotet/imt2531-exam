#include <overkill/ModelSystem.hpp>


namespace overkill
{

std::vector<Model> ModelSystem::m_models;
std::unordered_map<C::Tag, C::ID> ModelSystem::m_mapModelID;

auto ModelSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapModelID[tag];
}

auto ModelSystem::getByTag(const C::Tag& tag) -> Model&
{
    return ModelSystem::m_models[m_mapModelID[tag]];
}

auto ModelSystem::getById(C::ID modelID) -> Model&
{
    return ModelSystem::m_models[modelID];
}

auto packUV(float u, float v) -> GLshort
{
    const auto MAX = 127;
    //const auto MIN = -128;
    const auto CLAMPER = 255;
   
    return (GLint(v * MAX) & CLAMPER) << 8 | (GLint(u * MAX) & CLAMPER);
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

void ModelSystem::reload() 
{   
    // Clean out all buffers from GPU
    for(auto model : ModelSystem::m_models)
    {
        for(auto mesh : model.m_meshes) 
        {
            mesh.m_ebo.clean();
        }
        model.m_vbo.clean();
        model.m_vao.clean();
    }

    // Clear the models on CPU
    ModelSystem::m_models.clear();

    // Unbind callbacks
    MaterialSystem::unbindAll();
    ShaderSystem::unbindAll();

    // Finally load new data
    Watcher::pollEvents();
    ModelSystem::load();
}


auto ModelSystem::makeModel(const C::Tag& tag, const std::string& modelString, Model* outModel) -> C::Err
{
    auto p = Parser(modelString);
    //
    // VERTICES
    //
    // Vertex count
    auto[vertexCountKey, vertexCount, err1] = p.nextKeyInteger();
    if(err1) {
        return 1;
    }

    // Iterate vertices
    std::vector<Vertex> vertices;
    vertices.reserve(vertexCount);
    LOG_DEBUG("vertexcount: %d", vertexCount);

    for(int i = 0; i < vertexCount; ++i) 
    {
        auto[vertexKey, vertex, err2] = p.nextKeyVertex();
        if(err2) {
            return 1;
        }
        vertices.push_back(vertex);
    }



    auto vbufLayout = VertexBufferAttribLayout();

    vbufLayout.push<GL_FLOAT>(3);                       //position -> 0  -> 12 -> 12
    vbufLayout.pushPacked<GL_INT_2_10_10_10_REV>(4);    //normal   -> 12 -> 16 -> 16
    vbufLayout.push<GL_UNSIGNED_SHORT>(2, GL_TRUE);              //uv       -> 16 -> 20 -> 24 ?
    vbufLayout.push<GL_UNSIGNED_BYTE>(4, GL_TRUE);      //color    -> 18 -> 22 -> 20


    // Buffer vertex data to GPU VAO and VBO
    Model newModel;
    newModel.m_tag = tag;
    newModel.m_vbo = VertexBuffer(vertices.data(), vertices.size() * vbufLayout.getStride()); //@NOTE: if there are exploding mesh issues this probably has to do with alignment issues, your GPU preffers data packed 4 by 4 bytes
    newModel.m_vao.addBuffer(newModel.m_vbo, vbufLayout);
    //
    // MESHES
    //
    // Mesh count
    auto[meshCountKey, meshCount, err3] = p.nextKeyInteger();
    if(err3) {
        return 1;
    }

    // Iterate meshes
    for(int i = 0; i < meshCount; ++i) 
    {
        // mesh tag
        auto[meshtagKey, meshtag, err2] = p.nextKeyString();
        if(err2) {
            return 1;
        }
        // material tag
        auto[materialtagKey, materialtag, err4] = p.nextKeyString();
        if(err4) {
            return 1;
        }
        // shader tag
        auto[shadertagKey, shadertag, err5] = p.nextKeyString();
        if(err5) {
            return 1;
        }
        
        // Triangle count
        auto[triCountKey, triCount, err6]   = p.nextKeyInteger();
        if(err6) {
            return 1;
        }

        std::vector<GLuint> indices;
        indices.reserve(triCount*3);
        LOG_DEBUG("tricount: %d", triCount);
        // Triangles 
        for(int j = 0; j < triCount; ++j) 
        {

            auto[triKey, triangle, err7] = p.nextKeyTriangle();
            if(err7) {
                return 1;
            }
            indices.push_back(triangle.a);
            indices.push_back(triangle.b);
            indices.push_back(triangle.c);

        } 

        // Construct mesh, buffer ElementBuffer data to GPU
        auto meshID = newModel.m_meshes.size();
        auto newMesh = newModel.m_meshes.emplace_back(
            Mesh{
                meshtag,
                ElementBuffer(indices.data(), indices.size()),
                MaterialSystem::getIdByTag(materialtag),
                ShaderSystem::copyByTag(shadertag)
            }
        );
        newMesh.m_shaderProgram.setMaterial(MaterialSystem::getByTag(materialtag));

    }


    *outModel = newModel;
    return 0;
}


void ModelSystem::push(const C::Tag& tag, const std::string& filepath)  
{
    C::Err err;

    std::string modelString;
    err = Util::fileToString(filepath, &modelString);
    if (err) {
        LOG_WARN("Util::fileToString: Failed to load model file %s", filepath.data());
        ModelSystem::m_mapModelID[tag] = 0;
        return;
    }

    Model model;
    err = ModelSystem::makeModel(tag, modelString, &model);
    if(err) {
        LOG_WARN("ModelSystem::makeModel: Failed to parse model string");
        ModelSystem::m_mapModelID[tag] = 0;
        return;
    }

    ModelSystem::m_mapModelID[tag] = ModelSystem::m_models.size();
    ModelSystem::m_models.push_back(model);

    for (std::size_t i = 0; i < model.m_meshes.size(); i++)
    {
        // Bind to MaterialSystem update event
        MaterialSystem::bindOnUpdate(
            MaterialSystem::getById(model.m_meshes[i].m_materialID).m_tag,
            m_mapModelID[model.m_tag],
            i,
            [](C::ID materialID, C::ID modelID, C::ID meshID) {
                auto& model = ModelSystem::getById(modelID);
                auto& mesh = model.m_meshes[meshID];
                mesh.m_materialID = materialID;
                mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(materialID));
            }
        );

        // Bind to ShaderSystem update event
        ShaderSystem::bindOnUpdate(
            model.m_meshes[i].m_shaderProgram.m_tag,
            m_mapModelID[model.m_tag],
            i,
            [](C::ID shaderID, C::ID modelID, C::ID meshID) {
                auto& model = ModelSystem::getById(modelID);
                auto& mesh = model.m_meshes[meshID];
                mesh.m_shaderProgram = ShaderSystem::copyById(shaderID);
                mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(mesh.m_materialID));
            }
        );
    }
}




void ModelSystem::load()
{
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "models");

    ModelSystem::push("_default", "assets/models/_default.yml");
    LOG_DEBUG("Model from file: assets/models/_default.yml");

    for (auto e: fevents)
    {   
        auto filepath = C::ModelsFolder + ("/" + e.tag) + "." + e.extension;
        LOG_DEBUG("Model from file: %s", filepath.data());

        ModelSystem::push(e.tag, filepath);
    }

}

} // ::overkill
