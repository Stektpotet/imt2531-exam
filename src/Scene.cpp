#include <overkill/Scene.hpp>


namespace overkill
{
    std::vector<EntityModel> Scene::m_modelEntities;
    std::vector<int> Scene::m_rootEntities;

    Scene::Scene()
    {

    }

    void Scene::loadScene()
    {   
        int count = 0;
        auto modelCubeObject = EntityModel("cube", count++, glm::vec3(0, 0, 0), 
                                glm::vec3(0), glm::vec3(1),
                                glm::vec3(0.8f, 0, 0), 
                                glm::vec3(1, 3.4f, 1.67f));

        auto modelSuzanneObject = EntityModel("Suzanne", count++, glm::vec3(4,10,1),
                                glm::vec3(45, 45, 45),glm::vec3(10, 10, 10),
                                glm::vec3(0), glm::vec3(1, 3.4f, 1.67f));

        auto modelFloorObject = EntityModel("cube", count++, glm::vec3(0, -3, 0), glm::vec3(0), glm::vec3(20, 0.5f, 20));
        
        auto modelCubeChildObject = EntityModel("cube", count++, glm::vec3(2.2f, 0, 0));

        addEntityModel(modelCubeObject);    // Add models to container. Any changes made after this will be lost.
        addEntityModel(modelCubeChildObject);
        addEntityModel(modelSuzanneObject);    // Add models to container. Any changes made after this will be lost.
        addEntityModel(modelFloorObject);    // Add models to container. Any changes made after this will be lost.

        getEntityModel(0)-> addChild(1);      // Set entity with id 1 as a child of entity with id 0.

        // Doing this loop to set material for each mesh in each model should not be nessecary, because its already done in model system.
        for (auto entityModel : m_modelEntities) 
        {
            for (auto mesh: ModelSystem::getById(entityModel.getModel() ).m_meshes) 
            {   
                auto& shader = mesh.m_shaderProgram;
                shader.setMaterial(MaterialSystem::getById(mesh.m_materialID));
            }

        }

    }

    int Scene::addEntityModel(EntityModel & model)
    {
        int ID = m_modelEntities.size();
        m_rootEntities.push_back(ID);
        m_modelEntities.push_back(model);

        return ID;
    }

    void Scene::removeRoot(int ID)
    {
        auto rootToRemove = std::find(m_rootEntities.begin(), m_rootEntities.end(), ID);
        if (rootToRemove == m_rootEntities.end())
        {
            LOG_ERROR("Attempt to remove a root entity that does not exist(entiryID %d).", ID);
        }
        m_rootEntities.erase(rootToRemove);
    }

    EntityModel* Scene::getEntityModel(int ID)
    {
        return & m_modelEntities[ID];
    }

    void Scene::update(float dt)
    {
        for (auto ID : m_rootEntities)
        {
            getEntityModel(ID)-> update(dt);
        }
    }

    void Scene::draw(float t)
    {
        for (auto entity : m_modelEntities)
        {
            entity.draw(t);
        }
    }

}