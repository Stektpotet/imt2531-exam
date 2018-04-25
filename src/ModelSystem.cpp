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

void ModelSystem::load()
{
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "models");
    
    for(auto e: fevents)
    {   
        auto tag = e.tag;
        // Create new modesl
        m_mapModelID[tag] = m_models.size();
        Model newModel;

        // Model tag
        newModel.m_tag = tag;
        
        // Read from modelfile
        auto filepath = C::ModelsFolder + ("/" + tag) + "." + e.extension;
        auto filestring = Util::fileToString(filepath.data());
        auto p = Parser(filestring);
        LOG_DEBUG("Parsing: %s", filepath.data());

        //
        // VERTICES
        //
        // Vertex count
        auto[vertexCountKey, vertexCount, err1] = p.nextKeyInteger();
        if(err1) {
            break; // @TODO Set default model before breaking
        }

        // Iterate vertices
        std::vector<Vertex> vertices;
        vertices.reserve(vertexCount);
        LOG_DEBUG("vertexcount: %d", vertexCount);

        for(int i = 0; i < vertexCount; ++i) 
        {
            auto[vertexKey, vertex, err2] = p.nextKeyVertex();
            if(err2) {
                return;
            }
            vertices.push_back(vertex);
        }



        auto vbufLayout = VertexBufferAttribLayout();

        vbufLayout.push<GL_FLOAT>(3);                       //position -> 0  -> 12 -> 12
        vbufLayout.pushPacked<GL_INT_2_10_10_10_REV>(4);    //normal   -> 12 -> 16 -> 16
        vbufLayout.push<GL_UNSIGNED_BYTE>(4, GL_TRUE);      //color    -> 18 -> 22 -> 20
        vbufLayout.pushPacked<GL_UNSIGNED_BYTE>(1, 2, GL_TRUE);//uv    -> 16 -> 18 -> 22
        vbufLayout.push<GL_BYTE>(2);                        //alignment-> 22 -> 24 == sizeof(Vertex)


        // Buffer vertex data to GPU VAO and VBO
        newModel.m_vbo = VertexBuffer(vertices.data(), vertices.size() * vbufLayout.getStride()); //@NOTE: if there are exploding mesh issues this probably has to do with alignment issues, your GPU preffers data packed 4 by 4 bytes

        newModel.m_vao.addBuffer(newModel.m_vbo, vbufLayout);
        //
        // MESHES
        //
        // Mesh count
        auto[meshCountKey, meshCount, err3] = p.nextKeyInteger();
        if(err3) {
            return;
        }

        // Iterate meshes
        for(int i = 0; i < meshCount; ++i) 
        {
            // mesh tag
            auto[meshtagKey, meshtag, err2] = p.nextKeyString();
            if(err2) {
                return;
            }
            // material tag
            auto[materialtagKey, materialtag, err4] = p.nextKeyString();
            if(err4) {
                return;
            }
            // shader tag
            auto[shadertagKey, shadertag, err5] = p.nextKeyString();
            if(err5) {
                return;
            }
            
            // Triangle count
            auto[triCountKey, triCount, err6]   = p.nextKeyInteger();
            if(err6) {
                return;
            }

            std::vector<GLuint> indices;
            indices.reserve(triCount*3);
            LOG_DEBUG("tricount: %d", triCount);
            // Triangles 
            for(int j = 0; j < triCount; ++j) 
            {

                auto[triKey, triangle, err7] = p.nextKeyTriangle();
                if(err7) {
                    return;
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

            // Bind to MaterialSystem update event
            MaterialSystem::bindOnUpdate(
                materialtag,
                m_mapModelID[newModel.m_tag],
                meshID,
                [](C::ID materialID, C::ID modelID, C::ID meshID) {

                    auto model = ModelSystem::getById(modelID);
                    auto mesh  = model.m_meshes[meshID];

                    mesh.m_materialID = materialID;
                    mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(materialID));
                }
            );

            // Bind to ShaderSystem update event
            ShaderSystem::bindOnUpdate(
                shadertag,
                m_mapModelID[newModel.m_tag],
                meshID,
                [](C::ID shaderID, C::ID modelID, C::ID meshID) {

                    auto model = ModelSystem::getById(modelID);
                    auto mesh  = model.m_meshes[meshID];

                    mesh.m_shaderProgram = ShaderSystem::copyById(shaderID);
                    mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(mesh.m_materialID));
                }
            );
        }
        m_models.push_back(newModel);

    }
}

} // ::overkill
