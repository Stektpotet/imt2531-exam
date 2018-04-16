#include <overkill/Material.hpp>

#include <overkill/Util.hpp>
#include <overkill/Parser.hpp>
#include <overkill/TextureSystem.hpp>
#include <iostream>

namespace overkill 
{


Material::Material(const std::string& filepath)
{

    
    auto file = Util::fileToString(filepath);

    LOG_DEBUG("Parsing: %s", filepath.data());
    auto p = Parser(file);

    // Iterating through maps from material file
    auto[mapcountkey, mapcount] = p.nextKeyInteger();
    for (size_t i = 0; i < mapcount; ++i) {
        auto[uniformtag, maptag] = p.nextKeyString();
    
        m_unimaps.emplace_back(
            UniformTexture {
                uniformtag,
                TextureSystem::copyByTag(maptag)
            }
        );
    }

    // Iterating through values from material file
    auto[valueskey, valuescount] = p.nextKeyInteger();
    for (size_t i = 0; i < valuescount; ++i) {
        auto[uniformtag, value] = p.nextKeyFloat();

        m_univalues.emplace_back(
            UniformFloat{ uniformtag, value }
        );
    }
}


}
