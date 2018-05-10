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

void ModelSystem::clean() 
{
    // Clean out all buffers from GPU
    for (auto model : ModelSystem::m_models)
    {
        for (auto mesh : model.m_meshes)
        {
            mesh.m_ebo.clean();
        }
        model.m_vbo.clean();
        model.m_vao.clean();
    }

    // Clear the models on CPU
    ModelSystem::m_models.clear();
}

void ModelSystem::reload() 
{   
    ModelSystem::clean();

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
    std::vector<Vertex> vertices;

    // Vertex count
    if (auto[key, vertexCount, err] = p.keyInteger("vertices"); err) {
        return 1;
    } else {
        vertices.resize(vertexCount);
        LOG_DEBUG("vertexcount: %d", vertexCount);

        for(int i = 0; i < vertexCount; ++i) 
        {
            vertices[i] = p.onlyVertex();
        }        
    }



    auto vbufLayout = VertexBufferAttribLayout();

    vbufLayout.push<GL_FLOAT>(3);                       //position -> 0  -> 12 -> 12
    vbufLayout.pushPacked<GL_INT_2_10_10_10_REV>(4);    //normal   -> 12 -> 16 -> 16
    vbufLayout.push<GL_UNSIGNED_SHORT>(2, GL_TRUE);                         //uv       -> 16 -> 20 -> 24 ?
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
    
    int meshCount;
    if (auto[key, meshCount_, err] = p.keyInteger("meshes"); err) {
        return 1;
    } else {
        meshCount = meshCount_;
    }

    // Iterate meshes
    for(int i = 0; i < meshCount; ++i) 
    {
        std::string meshTag,
                    materialTag,
                    shaderTag;

        // mesh tag
        if (auto[key, meshTag_, err] = p.keyString("mesh"); err) {
            return 1;
        } else {
            meshTag = std::string(meshTag_);
        }

        // material tag
        if (auto[key, materialTag_, err] = p.keyString("material"); err) {
            return 1;
        } else {
            materialTag = std::string(materialTag_);
        }

        // shader tag
        if (auto[key, shaderTag_, err] = p.keyString("shader"); err) {
            return 1;
        } else {
            shaderTag = std::string(shaderTag_);            
        }
        

        //
        // TRIANGLES 
        //
        std::vector<Triangle> triangles;

        // Triangle count
        if (auto[key, triCount, err] = p.keyInteger("triangles"); err){
            return err;
        } else {

            triangles.resize(triCount);
            LOG_DEBUG("tricount: %d", triCount);
            for(int j = 0; j < triCount; ++j) 
            {
                triangles[j] = p.onlyTriangle();
            } 
        }

        // Construct mesh, buffer ElementBuffer data to GPU
        auto newMesh = newModel.m_meshes.emplace_back(
            Mesh{
                meshTag,
                ElementBuffer((unsigned int*)triangles.data(), triangles.size() * 3),
                MaterialSystem::getIdByTag(materialTag),
                ShaderSystem::copyByTag(shaderTag)
            }
        );
        newMesh.m_shaderProgram.setMaterial(MaterialSystem::getByTag(materialTag));

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


#include <set>




auto ModelSystem::loadOBJ(const tinyobj::attrib_t&                attributes,
                          const std::vector<tinyobj::shape_t>&    shapes, 
                          const std::vector<tinyobj::material_t>& materials,
                          const C::Tag&                           objTag) -> C::Err
{


    using u8 = std::uint8_t;
    using u64 = std::uint64_t;


    std::vector<Vertex> overkillVertices;
    std::vector<std::vector<Triangle>> overkillTrianglesArray;
    overkillVertices.reserve(1024);

    //
    // 1. Go through meshes and fetch indicies + indexcombos
    //
    for (const auto& shape: shapes) 
    {
        auto& objMesh           = shape.mesh;
        auto overkillTriangles = std::vector<Triangle>{};

        const u8 TriangleStride = 3;
        for (u64 triangleIndex = 0; triangleIndex < objMesh.num_face_vertices.size(); triangleIndex++)
        {
            // @note
            // This is a safety guard. We might turn this off if we can be certain that
            // the .obj files has been triangulated. - JSolsvik 2018-05-09

            if (objMesh.num_face_vertices[triangleIndex] != TriangleStride) {
                LOG_ERROR("\nobjMesh.num_face_vertices != 3. TRIANGULATE YOUR objMesh'es\n"
                          "Expected: 3"
                          "Got: %lu", static_cast<u64>( objMesh.num_face_vertices[triangleIndex]));
            }


            // Temp variables so we don't have to write dot ('.') so many times later.
            auto& objIndices   = objMesh.indices;

            // A triangle has 3 corners -> A, B and C
            auto& indexA = objIndices[triangleIndex * TriangleStride + 0];
            auto& indexB = objIndices[triangleIndex * TriangleStride + 1];
            auto& indexC = objIndices[triangleIndex * TriangleStride + 2];
            
            auto makeVertexReturnIndex = [&overkillVertices, &attributes](const tinyobj::index_t& indexSet) -> GLuint
            {
                using s8 = std::uint8_t;

                const s8 VertexStride = 3;
                const s8 NormalStride = 3;
                const s8 TextureStride = 2;

                auto vert = Vertex{
                    attributes.vertices[indexSet.vertex_index * VertexStride + 0],
                    attributes.vertices[indexSet.vertex_index * VertexStride + 1],
                    attributes.vertices[indexSet.vertex_index * VertexStride + 2],

                    Util::packNormal(attributes.normals[indexSet.normal_index * NormalStride + 0],
                                     attributes.normals[indexSet.normal_index * NormalStride + 1],
                                     attributes.normals[indexSet.normal_index * NormalStride + 2]),
                };

                if (indexSet.texcoord_index != -1)
                {
                    vert.u =  static_cast<GLushort>(65535U * attributes.texcoords[indexSet.texcoord_index * TextureStride + 0]);

                    // FLIP coordinates on Y axis
                    vert.v =  static_cast<GLushort>(65535U * (1 - attributes.texcoords[indexSet.texcoord_index * TextureStride + 1]));
                }

                overkillVertices.push_back(vert);
                return overkillVertices.size() - 1;
            };

            overkillTriangles.emplace_back( Triangle {
                makeVertexReturnIndex(indexA),
                makeVertexReturnIndex(indexB),
                makeVertexReturnIndex(indexC),
                });

        } // END FOR TRIANGLES

        overkillTrianglesArray.push_back(overkillTriangles);
    } // END FOR SHAPES(MESHES)


 
    //
    // 3. Create model with tag
    //
    auto overkillModel = Model{};
    overkillModel.m_tag = "obj/" + objTag;

    //
    // 4. Buffer vertexdata to the GPU
    //
    auto vbufLayout = VertexBufferAttribLayout();
    {
        vbufLayout.push<GL_FLOAT>(3);                       //position -> 0  -> 12 -> 12
        vbufLayout.pushPacked<GL_INT_2_10_10_10_REV>(4);    //normal   -> 12 -> 16 -> 16
        vbufLayout.push<GL_UNSIGNED_SHORT>(2, GL_TRUE);                 //uv       -> 16 -> 20 -> 24 ?
        vbufLayout.push<GL_UNSIGNED_BYTE>(4, GL_TRUE);      //color    -> 18 -> 22 -> 20

        overkillModel.m_vbo = VertexBuffer(overkillVertices.data(), 
                                           overkillVertices.size() * vbufLayout.getStride());
        overkillModel.m_vao.addBuffer(overkillModel.m_vbo, vbufLayout);
    }

    //
    // 5. Push back model
    //
    ModelSystem::m_mapModelID[overkillModel.m_tag] = ModelSystem::m_models.size();


    for (std::size_t i = 0; i < overkillTrianglesArray.size(); ++i)
    {
        //
        // 6. Create overkillmesh and buffer indexdata to GPU
        //
        auto& overkillTriangles = overkillTrianglesArray[i];
        auto& shape = shapes[i];
        C::Tag objMaterialTag = materials[shape.mesh.material_ids[0]].name;
        
        C::Tag overkillMeshtag     = "obj/" + objTag + "/" + shape.name;
        C::Tag overkillMaterialTag = "obj/"+ objTag + "/" + objMaterialTag;


        auto indiciesCount = overkillTriangles.size() * 3;

        auto overkillMesh = overkillModel.m_meshes.emplace_back(
            Mesh
            {
                overkillMeshtag,
                ElementBuffer( (unsigned int*)overkillTriangles.data(), indiciesCount),
                MaterialSystem::getIdByTag(overkillMaterialTag),
                ShaderSystem::copyByTag("objshader")
            }
        );

        //
        // 7. Bind to MaterialSystem update event
        //
        MaterialSystem::bindOnUpdate(
            MaterialSystem::getById(overkillMesh.m_materialID).m_tag,
            m_mapModelID[overkillModel.m_tag],
            i,
            [](C::ID materialID, C::ID modelID, C::ID meshID) {
                auto& model = ModelSystem::getById(modelID);
                auto& mesh = model.m_meshes[meshID];
                mesh.m_materialID = materialID;
                mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(materialID));
            }
        );

        //
        // 8. Bind to ShaderSystem update event
        //
        ShaderSystem::bindOnUpdate(
            overkillMesh.m_shaderProgram.m_tag,
            m_mapModelID[overkillModel.m_tag],
            i,
            [](C::ID shaderID, C::ID modelID, C::ID meshID) {
                auto& model = ModelSystem::getById(modelID);
                auto& mesh = model.m_meshes[meshID];
                mesh.m_shaderProgram = ShaderSystem::copyById(shaderID);
                mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(mesh.m_materialID));
            }
        );
    }

    ModelSystem::m_models.push_back(overkillModel);

    return 0;
}

} // ::overkill
