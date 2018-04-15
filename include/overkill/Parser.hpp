#pragma once

#include <string>
#include <string_view>

#include <overkill/Model.hpp>

namespace overkill 
{

using Line = std::string;
struct KeyInteger  { std::string key; int i;             };
struct KeyFloat    { std::string key; float fp;          };
struct KeyString   { std::string key; std::string str;   };
struct KeyVertex   { std::string key; Vertex vertex;     };
struct KeyTriangle { std::string key; Triangle triangle; };

/// <summary> Translates files into internal datastructure </summary>
class Parser
{
    std::string_view strview;
    int startofline =  0;
    int endofline = 9999999;
    int linecount = 0;
    const std::string whitelistedCharacters = "0123456789abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ-:";

public:
    Parser(std::string_view _strview);
    auto nextLine()        -> Line;
    auto nextKeyString()   -> KeyString;
    auto nextKeyInteger()  -> KeyInteger;
    auto nextKeyFloat()    -> KeyFloat;
    auto nextKeyVertex()   -> KeyVertex;
    auto nextKeyTriangle() -> KeyTriangle;
};


}