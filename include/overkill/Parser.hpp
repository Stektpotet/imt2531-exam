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


struct KeyString    { std::string_view key; std::string_view value; int err; };
struct KeyInteger   { std::string_view key; int value;              int err; };
struct KeyFloat     { std::string_view key; float value;            int err; };
struct KeyVec3      { std::string_view key; glm::vec3 value;        int err; };
struct KeyVec4      { std::string_view key; glm::vec4 value;        int err; };
struct KeyTriangle  { std::string_view key; Triangle value;         int err; };
struct KeyVertex    { std::string_view key; Vertex value;           int err; };
struct KeyEnums     { std::string_view key; std::vector<std::string> values; int err; };

constexpr int ParserSuccess          = 0;
constexpr int ParserErr              = 1;
constexpr int ParserErr_WrongKey     = 2;
constexpr int ParserErr_Stringstream = 3;
constexpr int ParserErr_MissingSeparator = 4;
constexpr int ParserErr_MissingValue = 5;


/// <summary> Translates files into internal datastructure </summary>
class Parser
{
private:
    std::string_view strview;
    int startofLine =  0;
    int endofLine = -1;
    int lineCount = 0;
    std::string_view currentLine;

public:
    Parser(std::string_view _strview);

    auto nextLine()         -> std::string_view;

    auto keyString()-> KeyString;
    auto keyString(const std::string_view wantedKey) -> KeyString;

    auto keyInteger() -> KeyInteger;
    auto keyInteger(const std::string_view wantedKey) -> KeyInteger;

    auto keyFloat() -> KeyFloat;
    auto keyFloat(const std::string_view wantedKey) -> KeyFloat;

    auto keyVec3() -> KeyVec3;
    auto keyVec3(const std::string_view wantedKey) -> KeyVec3;

    auto keyVec4() -> KeyVec4;
    auto keyVec4(const std::string_view wantedKey) -> KeyVec4;

    auto keyEnums(const std::string_view expectedKey,
                  const std::vector<std::string_view> expectedArguments,
                  const std::size_t argumentCount = 1) -> KeyEnums;

    auto onlyVertex() -> Vertex;
    auto onlyTriangle() -> Triangle;
};


}
