#pragma once

#include <string>
#include <string_view>
#include <iomanip>
#include <sstream>

#include <PMS/logger.h>
#include <overkill/Model.hpp>
#include <overkill/Util.hpp>

#define DEBUG 0

namespace overkill 
{

struct KeyInteger  { std::string key; int i;             int parseerror=0; };
struct KeyFloat    { std::string key; float fp;          int parseerror=0; };
struct KeyString   { std::string key; std::string str;   int parseerror=0; };
struct KeyVertex   { std::string key; Vertex vertex;     int parseerror=0; };
struct KeyTriangle { std::string key; Triangle triangle; int parseerror=0; };
const int PARSE_SUCCESS = 0;
const int PARSE_ERROR = 1;

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
    auto nextLine()        -> std::string;
    auto nextKeyString()   -> KeyString;
    auto nextKeyInteger()  -> KeyInteger;
    auto nextKeyFloat()    -> KeyFloat;
    auto nextKeyVertex()   -> KeyVertex;
    auto nextKeyTriangle() -> KeyTriangle;
    auto nextKeyFilepath() -> KeyString;
};


}
