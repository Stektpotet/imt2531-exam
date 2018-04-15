#include <overkill/Material.hpp>

#include <overkill/Util.hpp>
#include <overkill/Parser.hpp>
#include <overkill/TextureSystem.hpp>
#include <iostream>

namespace overkill 
{


Material::Material(const std::string& tag)
{


    auto file = Util::fileToString("assets/materials/"+tag+".yml");
    auto parser = Parser(file);



    std::cout << file;

    m_unimaps.emplace_back(
        UniformTexture
        {
            "mainTex",
            TextureSystem::copyByTag("brickwall")
        }
    );
    m_unimaps.emplace_back(
        UniformTexture
        {
            "specularTex",
            TextureSystem::copyByTag("brickwall-nrm")        
        }
    );

    m_univalues.emplace_back(
        UniformFloat{ "test", 0.5f }
    );
}


}
