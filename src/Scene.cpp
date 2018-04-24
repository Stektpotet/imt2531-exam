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

        auto modelCubeChildObject = EntityModel("cube", count++, glm::vec3(2.2f, 0, 0));

        addEntityModel(modelCubeObject);    // Add models to container. Any changes made after this will be lost.
        addEntityModel(modelCubeChildObject);
        getEntityModel(0)-> addChild(1);      // Set entity with id 1 as a child of entity with id 0.
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

    void Scene::draw()
    {
        for (auto entity : m_modelEntities)
        {
            entity.draw();
        }
    }

}