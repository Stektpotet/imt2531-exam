#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/Model.hpp>

namespace overkill
{


struct Entity 
{
    int m_modelID;
};

class EntitySystem 
{
private:
    static std::vector<Model> m_models;
    static std::unordered_map<std::string, int> m_mapModelID;

public:
    // <summary> Load all models into memory </summary>
    static void   load();
    static Entity make(int modelID);
    static Entity make(const std::string& modeltag);
    static Entity clone(const Entity& entity);

    static const Model& getModel(const Entity& entity);
};

}
