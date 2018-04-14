#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/Config.hpp>
#include <overkill/Texture.hpp>

namespace overkill
{

class TextureSystem 
{
private:
    static std::vector<Texture> m_textures;
    static std::unordered_map<C::Tag, C::ID> m_mapTextureID;

public:
    // <summary> Load all texture data onto GPU memory. 
    //           Keep model metadata on CPU </summary>
    static void load();
    static auto getIdByTag(const C::Tag& textureTag) -> C::ID;
    static auto getByTag(const C::Tag& textureTag) -> const Texture&;
    static auto getById(const C::ID textureID) -> const Texture&;
};

}
