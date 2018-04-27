#include <overkill/Scene.hpp>


namespace overkill
{
    // EntityCamera* Scene::m_camera;
    std::vector<Entity*> Scene::m_entities;
    std::vector<int> Scene::m_rootEntities;
    EntityCamera* Scene::m_activeCamera;
    int Scene::m_cameraCount;

    Scene::Scene()
    {

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
        LOG_DEBUG("Set %s as a child of %s", getEntity(childID)-> getTag().c_str(), getEntity(parentID)-> getTag().c_str());
    }


    void Scene::loadScene()
    {   
        // Reset values:
        m_activeCamera = nullptr;
        int count = 0;
        m_cameraCount = 0;

        // Setup entities:
        auto camera = new EntityCamera("camera0", count++, 
                                    glm::vec3(0, 0, -20),       // Pos.
                                    glm::vec3(0, 0, 0),         // Rot.
                                    glm::vec3(0, 0, 0),         // Vel.
                                    glm::vec3(0, 0, 0),         // AngVel.
                                    FREELOOK,                   // CameraMode FREELOOK, ORBITAL
                                    90,                         // FOV.
                                    1.25,                       // aspectRatio.
                                    0.1f,                       // NearClip.
                                    -100);                      // FarClip.
        m_cameraCount++;
        addEntity(camera);              // Add models to container. Any changes made after this will be lost:


        auto cameraTwo = new EntityCamera("camera1", count++, 
                                    glm::vec3(0, 0, -3),       // Pos.
                                    glm::vec3(0, 0, 0),         // Rot.
                                    glm::vec3(0, 0, 0),         // Vel.
                                    glm::vec3(0, 0, 0),         // AngVel.
                                    FREELOOK,                   // CameraMode FREELOOK, ORBITAL
                                    90,                         // FOV.
                                    1.25,                       // aspectRatio.
                                    0.1f,                       // NearClip.
                                    -100);                      // FarClip.
        m_cameraCount++;
        addEntity(cameraTwo);


        auto cameraThree = new EntityCamera("camera2", count++, 
                                    glm::vec3(0, 0, -6),       // Pos.
                                    glm::vec3(0, 0, 0),         // Rot.
                                    glm::vec3(0, 0, 0),         // Vel.
                                    glm::vec3(0, 0, 0),         // AngVel.
                                    ORBITAL,                   // CameraMode FREELOOK, ORBITAL
                                    90,                         // FOV.
                                    1.25,                       // aspectRatio.
                                    0.1f,                       // NearClip.
                                    -100);                      // FarClip.
        m_cameraCount++;
        addEntity(cameraThree);


        auto cameraFour = new EntityCamera("camera3", count++, 
                                    glm::vec3(0, 0, -6),       // Pos.
                                    glm::vec3(0, 0, 0),         // Rot.
                                    glm::vec3(0, 0, 0),         // Vel.
                                    glm::vec3(0, 0, 0),         // AngVel.
                                    ORBITAL,                   // CameraMode FREELOOK, ORBITAL
                                    90,                         // FOV.
                                    1.25,                       // aspectRatio.
                                    0.1f,                       // NearClip.
                                    -100);                      // FarClip.
        m_cameraCount++;
        addEntity(cameraFour);

        auto debugCamPos = new EntityModel("cube", "cameraPos", count++, glm::vec3(0.8, -0.8, -3), glm::vec3(0), glm::vec3(0.2f, 0.8f, 3));
        addEntity(debugCamPos);
        

        // Cube planets:
        auto modelCubeObject = new EntityModel("cube", "parentCube", count++, glm::vec3(0, 0, 0), 
                                glm::vec3(0), glm::vec3(4),
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, 10, 0));
        addEntity(modelCubeObject);    

        auto modelCubeChildObject = new EntityModel("cube", "childCube", count++, glm::vec3(2, 0, 0),
                                glm::vec3(0, 0, 0), glm::vec3(0.1f),
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, 50, 0));
        addEntity(modelCubeChildObject);

        auto modelCubeGrandChildObject = new EntityModel("cube", "grandChildCube", count++, glm::vec3(5, 0, 0),
                                glm::vec3(0, 0, 0), glm::vec3(0.3f),
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, -90, 0));
        addEntity(modelCubeGrandChildObject);



        auto modelSuzanneObject = new EntityModel("Suzanne", "Suzanne", count++, glm::vec3(4,10,1),
                                glm::vec3(45, 45, 45),glm::vec3(5, 5, 5),
                                glm::vec3(0), glm::vec3(1, 3.4f, 1.67f));
        addEntity(modelSuzanneObject);   

        auto modelFloorObject = new EntityModel("cube", "floor", count++, glm::vec3(0, -36, 0), glm::vec3(0), glm::vec3(30, 30, 30));
        addEntity(modelFloorObject);  
        
        
        // Set child-parent relationships:

        setChild(modelCubeObject -> getEntityID(), cameraTwo -> getEntityID());
        setChild(modelCubeChildObject -> getEntityID(), cameraThree -> getEntityID());
        setChild(modelCubeGrandChildObject -> getEntityID(), cameraFour -> getEntityID());

        setChild(camera->getEntityID(), debugCamPos->getEntityID());
        
        setChild(modelCubeObject-> getEntityID(), modelCubeChildObject-> getEntityID());      // Set entity with id 1 as a child of entity with id 0.
        setChild(modelCubeChildObject-> getEntityID(), modelCubeGrandChildObject-> getEntityID());      // Set entity with id 1 as a child of entity with id 0.

        m_activeCamera = (EntityCamera*) m_entities[0];     // All cameras in the beginning of scene file, first one active by default.
    }

    int Scene::addEntity(Entity* entity)
    {
        int ID = m_entities.size();
        m_rootEntities.push_back(ID);
        m_entities.push_back(entity);

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
            LOG_ERROR("Attempt to find entity that does not exist.");
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
        m_activeCamera -> checkInput();     // Only update the active camera on input.
        for (auto ID : m_rootEntities)      // all the cameras update() is still being run in here.
        {
            getEntity(ID)-> update(dt);
        }
    }

    void Scene::draw(float t)
    {
        for (Entity* entity : m_entities)
        {
            entity-> draw(t);
        }
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