#pragma once

#include <string>
#include <string_view>
#include <iomanip>
#include <sstream>

#include <PMS/logger.h>
#include <overkill/Model.hpp>
#include <overkill/Util.hpp>


namespace overkill 
{


struct KeyColor    { std::string key; glm::vec4 color;       int parseerror; };
struct KeyInteger  { std::string key; int i;             int parseerror; };
struct KeyFloat    { std::string key; float fp;          int parseerror; };
struct KeyString   { std::string key; std::string str;   int parseerror; };
struct KeyVertex   { std::string key; Vertex vertex;     int parseerror; };
struct KeyTriangle { std::string key; Triangle triangle; int parseerror; };
constexpr int PARSE_SUCCESS = 0;
constexpr int PARSE_ERROR = 1;

/// <summary> Translates files into internal datastructure </summary>
class Parser
{

    std::string_view strview;
    int startofline =  0;
    int endofline = 9999999;
    int linecount = 0;
    std::string currentLine = "<top>";
    const std::string whitelistedCharacters = "0123456789abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ-:";

public:
    Parser(std::string_view _strview);
    auto nextLine()        -> std::string;
    auto nextKeyString()   -> KeyString;
    auto nextKeyInteger()  -> KeyInteger;
    auto nextKeyFloat()    -> KeyFloat;
    auto nextKeyVertex()   -> KeyVertex;
    auto nextKeyTriangle() -> KeyTriangle;
    auto nextKeyColor()    -> KeyColor;
};


}
