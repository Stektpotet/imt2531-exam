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


struct KeyColor     { std::string_view key; glm::vec4 color;     int parseerror; };
struct KeyInteger   { std::string_view key; int i;               int parseerror; };
struct KeyFloat     { std::string_view key; float fp;            int parseerror; };
struct KeyString    { std::string_view key; std::string_view str; int parseerror; };
struct KeyVertex    { std::string_view key; Vertex vertex;       int parseerror; };
struct KeyTriangle  { std::string_view key; Triangle triangle;   int parseerror; };
struct KeyVec3      { std::string_view key; glm::vec3 vector;    int parseerror; };


constexpr int PARSE_SUCCESS          = 0;
constexpr int PARSE_ERROR            = 1;

constexpr int ParserSuccess          = 0;
constexpr int ParserErr              = 1;
constexpr int ParserErr_WrongKey     = 2;
constexpr int ParserErr_Stringstream = 3;


/// <summary> Translates files into internal datastructure </summary>
class Parser
{

    std::string_view strview;
    int startofLine =  0;
    int endofLine = -1;
    int lineCount = 0;
    std::string_view currentLine;
    const char whitelistedCharacters[80] = "abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ0123456789-:";

public:
    Parser(std::string_view _strview);
    auto nextLine()         -> std::string_view;

    auto keyString()->KeyString;
    auto keyString(const std::string_view wantedKey)->KeyString;
    auto keyInteger(const std::string_view wantedKey) -> KeyInteger;
    auto keyVec3(const std::string_view wantedKey) -> KeyVec3;
    auto keyFloat(const std::string_view wantedKey) -> KeyFloat;


    auto nextVertex()->    Vertex;
    auto nextTriangle()-> Triangle;

    // DEPRECATED FUNCTIONS
    auto nextKeyString()->KeyString;
    auto nextKeyInteger()->KeyInteger;
    auto nextKeyFloat()->KeyFloat;
    auto nextKeyColor()->KeyColor;
    auto nextKeyVec3()->KeyVec3;


};


}
