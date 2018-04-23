#include <overkill/Scene.hpp>


namespace overkill
{
    Scene::Scene()
    {
        
    }

    int Scene::addEntityModel(EntityModel model)
    {
        int id = m_modelEnties.size();
        m_modelEnties.push_back(model);

        return id;
    }



}