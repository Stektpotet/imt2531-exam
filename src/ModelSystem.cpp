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
    ModelSystem::load();
}

void ModelSystem::load()
{
    std::vector<std::string> tags {
        "cube",
        "out"
    };

    for(auto tag: tags)
    {   
        // Create new modesl
        m_mapModelID[tag] = m_models.size();
        Model newModel;

        // Model tag
        newModel.m_tag = tag;
        
        // Read from modelfile
        auto filepath = C::ModelsFolder + tag + C::ModelsExtension;
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
        for(size_t i = 0; i < vertexCount; ++i) 
        {
            auto[vertexKey, vertex, err2] = p.nextKeyVertex();
            if(err2) {
                return;
            }
            vertices.push_back(vertex);
        }

        // Buffer vertex data to GPU VAO and VBO
        newModel.m_vbo = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
        
        auto vbufLayout = VertexBufferAttribLayout();
        vbufLayout.push(3, GL_FLOAT);                       //position;
        vbufLayout.push(1, GL_UNSIGNED_INT);                //normal
        vbufLayout.push(2, GL_SHORT);                       //uv
        vbufLayout.push(4, GL_UNSIGNED_BYTE, GL_TRUE);      //color;
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
        for(size_t i = 0; i < meshCount; ++i) 
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
            // Triangles 
            for(size_t j = 0; j < triCount; ++j) 
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
