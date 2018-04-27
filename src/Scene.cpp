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
        int modelCount = 0;
        m_cameraCount = 0;

        auto filestring = Util::fileToString("assets/scenes/_default.yml");
        Parser p(filestring);

        LOG_INFO("Loading \n%s", filestring.c_str());

        //
        //  EntityCameras:
        //
        // int   cameraCount.
        if (auto[key, cameras, err] = p.keyInteger("cameras"); err) 
        {
            LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
        } 
        else 
        {
            m_cameraCount = cameras;
            LOG_INFO("%s: %d",key.data(), m_cameraCount);        
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
            float aspect = 0;
            float near = 0;
            float far = 0;

            // char  entityTag.    
            if (auto[key, camera, err] = p.keyString("camera"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                tag = camera;
                LOG_INFO("%s: %s",key.data(), tag.data());             
            }

            // vec3 position.
            if (auto[key, position, err] = p.keyVec3("position"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                pos = position;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), pos.x, pos.y, pos.z);                    
            }

            // vec3 rotation.
            if (auto[key, rotation, err] = p.keyVec3("rotation"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                rot = rotation;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), rot.x, rot.y, rot.z);                    
            }

            // vec3 velocity.
            if (auto[key, velocity, err] = p.keyVec3("velocity"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                vel = velocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), vel.x, vel.y, vel.z);                    
            }

            // vec3 rotation.
            if (auto[key, angleVelocity, err] = p.keyVec3("angleVelocity"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                angVel = angleVelocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), angVel.x, angVel.y, angVel.z);                    
            }

            // char  CameraMode.    
            if (auto[key, mode, err] = p.keyString("mode"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                if (!strcmp("freelook", mode.data()))
                {
                    camMode = FREELOOK;
                }
                else if (!strcmp("orbital", mode.data()))
                {
                    camMode = ORBITAL;
                }
                else
                {
                    LOG_ERROR("%s error on key --> %s... expected freelook or orbital.", filestring.c_str(), key.data());
                }
                LOG_INFO("%s: %s",key.data(), mode.data());                    
            }

            // float fov.
            if (auto[key, fov, err] = p.keyFloat("fov"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            }
            else 
            {
                camFov = fov;
                LOG_INFO("%s: %f",key.data(), camFov);                    
            }

            // float aspectRatio.
            if (auto[key, aspectRatio, err] = p.keyFloat("aspectRatio"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                aspect = aspectRatio;
                LOG_INFO("%s: %f",key.data(), aspect);                    
            }

            // float nearClip.
            if (auto[key, nearClip, err] = p.keyFloat("nearClip"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                near = nearClip;
                LOG_INFO("%s: %f",key.data(), near);                    
            }

            // float farClip.
            if (auto[key, farClip, err] = p.keyFloat("farClip"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                far = farClip;
                LOG_INFO("%s: %f",key.data(), far);                    
            }

            auto camEntity = new EntityCamera(tag, 
                                              count, 
                                              pos, 
                                              rot, 
                                              vel, 
                                              angVel, 
                                              camMode, 
                                              camFov, 
                                              aspect, 
                                              near, 
                                              far);
            Scene::addEntity(camEntity);   
        }

        if (m_cameraCount == 0)
        {
            LOG_ERROR("m_cameraCount == 0: A scene has to have a camera");
        } 

        Scene::m_activeCamera = (EntityCamera*) Scene::m_entities[0];     // First camera in file is default camera.

        //
        //  EntityModels:
        //
        if (auto[key, entites, err] = p.keyInteger("entities"); err) 
        {
            LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
        } 
        else 
        {
            modelCount = entites;
            LOG_INFO("%s: %d",key.data(), modelCount);        
        }


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
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                entityTag = entity;
                LOG_INFO("%s: %s",key.data(), entityTag.data());                    
            }

            // char  modelTag.    
            if (auto[key, model, err] = p.keyString("model"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                modelTag = model;
                LOG_INFO("%s: %s",key.data(), modelTag.data());                    
            }

            // vec3 position.
            if (auto[key, position, err] = p.keyVec3("position"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                pos = position;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), pos.x, pos.y, pos.z);                    
            }

            // vec3 rotation.
            if (auto[key, rotation, err] = p.keyVec3("rotation"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                rot = rotation;
                LOG_INFO("%s: (%f, %f, %f)", key.data(), rot.x, rot.y, rot.z);                    
            }

            // vec3 scale.
            if (auto[key, scale, err] = p.keyVec3("scale"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                scl = scale;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), scl.x, scl.y, scl.z);                    
            }

            // vec3 velocity.
            if (auto[key, velocity, err] = p.keyVec3("velocity"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                vel = velocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), vel.x, vel.y, vel.z);                    
            }

            // vec3 angularVelocity.
            if (auto[key, angleVelocity, err] = p.keyVec3("angleVelocity"); err) 
            {
                LOG_ERROR("%s error on key --> %s...", filestring.c_str(), key.data());
            } 
            else 
            {
                angVel = angleVelocity;
                LOG_INFO("%s: (%f, %f, %f)",key.data(), angVel.x, angVel.y, angVel.z);                    
            }
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
/*
        // for ()      // Load EntityLights. NOT IMPLEMENTED.
        // {

        // }
*/
  
/*

        for ()      // Set child-parent realtionships.
        {
            for () 
            {

            }
        }
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