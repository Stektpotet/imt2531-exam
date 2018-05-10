#include <overkill/Scene.hpp>


namespace overkill
{


// EntityCamera* Scene::m_camera;
std::string Scene::m_sceneLoaded;
std::vector<Entity*> Scene::m_entities;
std::vector<int>     Scene::m_rootEntities;
EntityCamera*        Scene::m_activeCamera;
int                  Scene::m_cameraCount;


DirectionalLight Scene::m_sun;

UniformBuffer Scene::m_matrixBuffer;
UniformBuffer Scene::m_lightBuffer;
GLuint Scene::m_projectionGLindex;
GLuint Scene::m_pointLightGLindex;
GLuint Scene::m_sunGLindex;

int Scene::m_entitiesOffset;
int Scene::m_lightsOffset;
int Scene::m_lightsCount;


void Scene::load(std::string sceneFile)
{
    // Reset values:
    int count = 0;
    int modelCount = 0;
    int relationsCount = 0;
    m_activeCamera = nullptr;
    m_cameraCount = 0;
    m_sceneLoaded = sceneFile;

    auto filestring = Util::fileToString(sceneFile);
    Parser p(filestring);

    LOG_INFO("Loading \n%s", sceneFile.c_str());

    //
    //  EntityCameras:
    //
    // int   cameraCount.
    if (auto[key, cameras, err] = p.keyInteger("cameras"); err)
    {
        LOG_ERROR("%s error reading cameraCount on key --> %s...", filestring.c_str(), key.data());
    }
    else
    {
        m_cameraCount = cameras;
        LOG_INFO("%s: %d",std::string(key).data(), m_cameraCount);
    }

    // Load EntityCameras.
    for (; count < m_cameraCount; count++)
    {
        C::Tag tag;
        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 vel;
        glm::vec3 angVel;
        CameraMode camMode = FREELOOK;
        float camFov = 0;
        float nearClip = 0;
        float farClip = 0;

        // char  entityTag.
        if (auto[key, camera, err] = p.keyString("camera"); err)
        {
            LOG_ERROR("%s error on cameraTag key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            tag = camera;
            LOG_INFO("%s: %s",std::string(key).data(), tag.data());
        }

        // vec3 position.
        if (auto[key, position, err] = p.keyVec3("position"); err)
        {
            LOG_ERROR("%s error on camera position key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            pos = position;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), pos.x, pos.y, pos.z);
        }

        // vec3 rotation.
        if (auto[key, rotation, err] = p.keyVec3("rotation"); err)
        {
            LOG_ERROR("%s error on camera rotation key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            rot = rotation;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), rot.x, rot.y, rot.z);
        }

        // vec3 velocity.
        if (auto[key, velocity, err] = p.keyVec3("velocity"); err)
        {
            LOG_ERROR("%s error on camera velocity key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            vel = velocity;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), vel.x, vel.y, vel.z);
        }

        // vec3 rotation.
        if (auto[key, angleVelocity, err] = p.keyVec3("angleVelocity"); err)
        {
            LOG_ERROR("%s error on camera angle velocity key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            angVel = angleVelocity;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), angVel.x, angVel.y, angVel.z);
        }

        // char  CameraMode.
        if (auto[key, modes, err] = p.keyEnums("mode", {"freelook", "orbital"}); err)
        {
            LOG_ERROR("%s error on camera mode key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            if ("freelook" == modes[0])
            {
                camMode = FREELOOK;
            }
            else if ("orbital" == modes[0])
            {
                camMode = ORBITAL;
            }
            else
            {
                LOG_ERROR("%s error on key --> %s... expected freelook or orbital.", filestring.c_str(), key.data());
            }
            LOG_INFO("%s: %s", std::string(key).data(), std::string(modes[0]).data());
        }

        // float fov.
        if (auto[key, fov, err] = p.keyFloat("fov"); err)
        {
            LOG_ERROR("%s error on camera fov key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            camFov = fov;
            LOG_INFO("%s: %f",std::string(key).data(), camFov);
        }

        // float nearClip.
        if (auto[key, _nearClip, err] = p.keyFloat("nearClip"); err)
        {
            LOG_ERROR("%s error on camera nearclip key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            nearClip = _nearClip;
            LOG_INFO("%s: %f",std::string(key).data(), nearClip);
        }

        // float farClip.
        if (auto[key, _farClip, err] = p.keyFloat("farClip"); err)
        {
            LOG_ERROR("%s error on camera farclip key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            farClip = _farClip;
            LOG_INFO("%s: %f",std::string(key).data(), farClip);
        }

        auto camEntity = new EntityCamera(tag,
                                          count,
                                          pos,
                                          rot,
                                          vel,
                                          angVel,
                                          camMode,
                                          camFov,
                                          C::AspectRatio,
                                          nearClip,
                                          farClip);
        Scene::addEntity(camEntity);
    }

    if (m_cameraCount == 0)
    {
        LOG_ERROR("m_cameraCount == 0: A scene has to have at least one camera");
    }

    Scene::m_activeCamera = (EntityCamera*) Scene::m_entities[0];     // First camera in file is default camera.

    //
    //  EntityModels:
    //
    if (auto[key, entites, err] = p.keyInteger("entities"); err)
    {
        LOG_ERROR("%s error on entityCount key --> %s...", filestring.c_str(), key.data());
    }
    else
    {
        modelCount = entites;
        LOG_INFO("%s: %d",std::string(key).data(), modelCount);
    }

    m_entitiesOffset = count;

    for (; count < m_cameraCount +  modelCount; count++)
    {
        C::Tag entityTag;
        C::Tag modelTag;
        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 scl;      // Scale.
        glm::vec3 vel;
        glm::vec3 angVel;

        // char  entityTag.
        if (auto[key, entity, err] = p.keyString("entity"); err)
        {
            LOG_ERROR("%s error on entityTag key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            entityTag = entity;
            LOG_INFO("%s: %s",std::string(key).data(), entityTag.data());
        }

        // char  modelTag.
        if (auto[key, model, err] = p.keyString("model"); err)
        {
            LOG_ERROR("%s error on entityModelTag key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            modelTag = model;
            LOG_INFO("%s: %s",std::string(key).data(), modelTag.data());
        }

        // vec3 position.
        if (auto[key, position, err] = p.keyVec3("position"); err)
        {
            LOG_ERROR("%s error on entity position key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            pos = position;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), pos.x, pos.y, pos.z);
        }

        // vec3 rotation.
        if (auto[key, rotation, err] = p.keyVec3("rotation"); err)
        {
            LOG_ERROR("%s error on entity rotation key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            rot = rotation;
            LOG_INFO("%s: (%f, %f, %f)", std::string(key).data(), rot.x, rot.y, rot.z);
        }

        // vec3 scale.
        if (auto[key, scale, err] = p.keyVec3("scale"); err)
        {
            LOG_ERROR("%s error on entity scale key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            scl = scale;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), scl.x, scl.y, scl.z);
        }

        // vec3 velocity.
        if (auto[key, velocity, err] = p.keyVec3("velocity"); err)
        {
            LOG_ERROR("%s error on entity velocity key --> %s...", filestring.c_str(), key.data());
        }
        else
         {
            vel = velocity;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), vel.x, vel.y, vel.z);
        }

        // vec3 angularVelocity.
        if (auto[key, angleVelocity, err] = p.keyVec3("angleVelocity"); err)
        {
            LOG_ERROR("%s error on entity angle velocity key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            angVel = angleVelocity;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), angVel.x, angVel.y, angVel.z);
        }


        if (modelTag == "NULL" || modelTag == "null" || modelTag == "none" || modelTag == "_")
        {
            auto nodeEntity =  new EntityNode(entityTag,
                                count,
                                pos,
                                rot,
                                scl,
                                vel,
                                angVel);
            addEntity((Entity*) nodeEntity);
        }
        else
        {
            auto modelEntity = new EntityModel(modelTag,
                                entityTag,
                                count,
                                pos,
                                rot,
                                scl,
                                vel,
                                angVel);
            addEntity((Entity*) modelEntity);
        }

    }


    //
    //  PointLights:
    //
    if (auto[key, lights, err] = p.keyInteger("pointlights"); err)
    {
        LOG_ERROR("%s error on point light count key --> %s...", filestring.c_str(), key.data());
    }
    else
    {
        m_lightsCount = lights;
        LOG_INFO("%s: %d", std::string(key).data(), m_lightsCount);
    }

    m_lightsOffset = count;

    for (; count < m_lightsOffset + m_lightsCount; count++ )
    {
        C::Tag lightTag;
        glm::vec3 pos;
        glm::vec3 vel;
        glm::vec3 intensities;
        glm::vec3  falloff;


        //position: -15 2 10
        //velocity : 0 0 0
        //intensities : 1 0 0
        //falloff : 1.0 0.03125 0.0625

        // char  entityTag.
        if (auto[key, light, err] = p.keyString("pointlight"); err)
        {
            LOG_ERROR("%s error on point light tag key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            lightTag = light;
            LOG_INFO("%s: %s", std::string(key).data(), lightTag.data());
        }

        // vec3 position.
        if (auto[key, position, err] = p.keyVec3("position"); err)
        {
            LOG_ERROR("%s error on point light position key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            pos = position;
            LOG_INFO("%s: (%f, %f, %f)", std::string(key).data(), pos.x, pos.y, pos.z);
        }

        // vec3 velocity.
        if (auto[key, velocity, err] = p.keyVec3("velocity"); err)
        {
            LOG_ERROR("%s error on point light velocity key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            vel = velocity;
            LOG_INFO("%s: (%f, %f, %f)", std::string(key).data(), vel.x, vel.y, vel.z);
        }

        // vec3 intensity.
        if (auto[key, intensity, err] = p.keyVec3("intensities"); err)
        {
            LOG_ERROR("%s error on point light intencities key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            intensities = intensity;
            LOG_INFO("%s: (%f, %f, %f)", std::string(key).data(), intensity.x, intensity.y, intensity.z);
        }
        // vec3 falloff.
        if (auto[key, foff, err] = p.keyVec3("falloff"); err)
        {
            LOG_ERROR("%s error on point light falloff key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            falloff = foff;
            LOG_INFO("%s: (%f, %f, %f)", std::string(key).data(), foff.x, foff.y, foff.z);
        }
        auto lightEntity = new EntityPointLight(lightTag,
            count,
            pos,
            vel,
            intensities,
            falloff.x,
            falloff.y,
            falloff.z);
        addEntity((Entity*)lightEntity);
    }


    //
    //  DirectionalLights:
    //
    bool hasSun;
    if (auto[key, sunToggle, err] = p.keyInteger("hasSun"); err)
    {
        LOG_ERROR("%s error on hasSun key --> %s...", filestring.c_str(), key.data());
    }
    else
    {
        hasSun = (sunToggle != 0);
        LOG_INFO("%s: %i", std::string(key).data(), hasSun);
    }
    EntityDirectionalLight * sunEntity;
    if (hasSun) {
        C::Tag lightTag;
        glm::vec3 rot;
        glm::vec3 angVel;
        glm::vec3 intensities;


        // char  entityTag.
        if (auto[key, light, err] = p.keyString("dirlight"); err)
        {
            LOG_ERROR("%s error dirLight tag on key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            lightTag = light;
            LOG_INFO("%s: %s", std::string(key).data(), lightTag.data());
        }

        // vec3 rotation.
        if (auto[key, rotation, err] = p.keyVec3("rotation"); err)
        {
            LOG_ERROR("%s error dirlight rotation on key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            rot = rotation;
            LOG_INFO("%s: (%f, %f, %f)", std::string(key).data(), rotation.x, rotation.y, rotation.z);
        }
        // vec3 angularVelocity.
        if (auto[key, angleVelocity, err] = p.keyVec3("angleVelocity"); err)
        {
            LOG_ERROR("%s error on DirectionalLight angle velocity key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            angVel = angleVelocity;
            LOG_INFO("%s: (%f, %f, %f)",std::string(key).data(), angVel.x, angVel.y, angVel.z);
        }
        // vec3 intensity.
        if (auto[key, intensity, err] = p.keyVec3("intensities"); err)
        {
            LOG_ERROR("%s error on DirectionalLight intensities key --> %s...", filestring.c_str(), key.data());
        }
        else
        {
            intensities = intensity;
            LOG_INFO("%s: (%f, %f, %f)", std::string(key).data(), intensity.x, intensity.y, intensity.z);
        }

        sunEntity = new EntityDirectionalLight(lightTag,
            count,
            rot,
            angVel,
            intensities
        );
    }
    else {
        sunEntity = new EntityDirectionalLight("NOT_SUN",
            count,
            glm::vec3(0),
            glm::vec3(0),
            glm::vec3(0)
        );
    }
    addEntity((Entity*)sunEntity);

    m_matrixBuffer      = ShaderSystem::getUniformBuffer("OK_Matrices");
    m_lightBuffer       = ShaderSystem::getUniformBuffer("OK_Lights");
    m_projectionGLindex = m_matrixBuffer.getUniformIndex("projection");
    m_pointLightGLindex = m_lightBuffer.getUniformIndex("light[0].position");
    m_sunGLindex        = m_lightBuffer.getUniformIndex("sun.direction");
/*
    m_sun = DirectionalLight {
            -glm::vec4{ 10, 9, 8, 7 },
            { 1.0f, 0.756862745f, 0.552941176f,0.0f }
    };
*/




    if (auto[key, _relationsCount, err] = p.keyInteger("relations"); err )
    {
        LOG_ERROR("%s error on relationsCount key --> %s...", filestring.c_str(), key.data());
    }
    else
    {
        LOG_DEBUG("relations: %d", _relationsCount);
        relationsCount = _relationsCount;
    }

    for (int i = 0; i < relationsCount; ++i)      // Set child-parent realtionships.
    {
        int childCount = 0;
        std::string parentTag;
        if (auto[parent, _childCount, err] = p.keyInteger(); err)
        {
            LOG_ERROR("%s error on childCount key --> %s...", filestring.c_str(), std::string(parent).data());
        }
        else
        {
            childCount = _childCount;
            parentTag = parent;
        }

        for (int j = 0; j < childCount; ++j)
        {
            auto childTag = p.nextLine();
            Scene::setChild(
                Scene::getEntityByTag(parentTag)->getEntityID(),
                Scene::getEntityByTag(std::string(childTag))->getEntityID());
        }
    }

/*
        setChild(modelCubeObject -> getEntityID(), cameraTwo -> getEntityID());
        setChild(modelCubeChildObject -> getEntityID(), cameraThree -> getEntityID());
        setChild(modelCubeGrandChildObject -> getEntityID(), cameraFour -> getEntityID());

        setChild(camera->getEntityID(), debugCamPos->getEntityID());

        setChild(modelCubeObject-> getEntityID(), modelCubeChildObject-> getEntityID());      // Set entity with id 1 as a child of entity with id 0.
        setChild(modelCubeChildObject-> getEntityID(), modelCubeGrandChildObject-> getEntityID());      // Set entity with id 1 as a child of entity with id 0.


        m_activeCamera = (EntityCamera*) m_entities[0];
*/
             // All cameras in the beginning of scene file, first one active by default.
    //

}



    void Scene::reload()
    {
        Init::Config();
        Init::OpenGL(C::ClearColor);
        glfwSetWindowSize(C::window, C::WinWidth, C::WinHeight);
        Scene::clean();
        Scene::m_entities.clear();
        Scene::m_rootEntities.clear();
        Scene::m_cameraCount = 0;
        Scene::m_activeCamera = nullptr;
        Scene::load(m_sceneLoaded);
    }


    void Scene::setChild(int parentID, int childID)
    {
        auto rootToRemove = std::find(m_rootEntities.begin(), m_rootEntities.end(), childID);
        if (rootToRemove == m_rootEntities.end())
        {
            LOG_ERROR("Attempt to remove a root entity that does not exist(entiryID %d).", childID);
        }
        m_rootEntities.erase(rootToRemove);

        getEntity(parentID)-> addChild(childID);

        LOG_DEBUG("parent: %s --> child: %s",
            getEntity(parentID)-> getTag().data(),
            getEntity(childID)-> getTag().data());
    }


    bool Scene::entityExist(const C::Tag tag)
    {
        bool exist = true;

        auto comp = [tag](Entity* entity)       // Lambda to compare two entities by tag.
        {
            return (entity-> getTag() == tag);
        };

        auto entityToGet = std::find_if(m_entities.begin(), m_entities.end(), comp);
        if (entityToGet == m_entities.end())
        {
            exist = false;
        }
        return exist;
    }


    int Scene::addEntity(Entity* entity)
    {
        int ID = -1;

        if (!entityExist(entity-> getTag()))  // If entity with this tag does not exist yet.
        {
            ID = m_entities.size();
            m_rootEntities.push_back(ID);
            m_entities.push_back(entity);
        }
        else                                                            // Entity with tag already exist.
        {
            LOG_ERROR("Attempt to create duplicate entity with tag: %s"
                    "\n\t\t\t\t  Check that your scene file does not contain two entities with the tag(%s).",
                    entity-> getTag().c_str(), entity-> getTag().c_str());
        }

        return ID;
    }

    Entity* Scene::getEntity(int ID)
    {
        return m_entities[ID];
    }

    Entity* Scene::getEntityByTag(const C::Tag tag)
    {
        auto comp = [tag](Entity* entity)       // Lambda to compare two entities by tag.
        {
            return (entity-> getTag() == tag);
        };

        auto entityToGet = std::find_if(m_entities.begin(), m_entities.end(), comp);
        if (entityToGet == m_entities.end())
        {
            LOG_ERROR("Attempt to find entity that does not exist. Tried to find entity with tag: %s", tag.data());
        }
        return *entityToGet;
    }

    EntityCamera* Scene::getActiveCamera()
    {
        return m_activeCamera;
    }

    void Scene::cycleCameras()
    {
        m_activeCamera = (EntityCamera*) m_entities[(m_activeCamera->getEntityID() + 1) % m_cameraCount];
    }


    void Scene::update(float dt)
    {

        // Update active camera
        m_activeCamera -> checkInput();     // Only update the active camera on input.

        // Update entities
        for (auto ID : m_rootEntities)      // all the cameras update() is still being run in here.
        {
            getEntity(ID)-> update(dt);
        }
    }

    void Scene::draw(float t)
    {
        // Buffer camera data
        Scene::m_matrixBuffer.update(m_projectionGLindex,
                              sizeof(CameraTransform),
                              &(m_activeCamera -> m_cameraTransform));

        // Buffer light data
        Scene::bufferPointLights();
        auto pointLight = (((EntityDirectionalLight*)m_entities[m_lightsOffset + m_lightsCount])->pack());
        m_lightBuffer.update(m_sunGLindex, sizeof(DirectionalLightBO), &pointLight);


        for (Entity* entity : m_entities)
        {
            entity-> draw(t);
        }
    }

    void Scene::bufferPointLights()
    {
        std::vector<PointLightBO> pbos;
        pbos.resize(C::MAX_LIGHTS);

        int count = 0;
        for(int off = m_lightsOffset; off < m_lightsOffset + m_lightsCount; ++off)
        {
            pbos[count++] = (((EntityPointLight*)m_entities[off])->pack());
        }

        m_lightBuffer.update(m_pointLightGLindex, sizeof(PointLightBO) * C::MAX_LIGHTS, pbos.data());
    }





    void Scene::clean()
    {
        // delete m_camera;
        for (Entity* entity : m_entities)
        {
            delete entity;
        }
    }

}
