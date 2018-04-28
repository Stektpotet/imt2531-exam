#include <overkill/Parser.hpp>


namespace overkill 
{
Parser::Parser(std::string_view _strview)
:strview(_strview)
{}


auto Parser::nextLine() -> std::string
{


    do {
        if (startofline != 0) {
            startofline = endofline + 1;
        }

        endofline   = strview.find('\n', startofline);
        startofline = strview.find_first_of(whitelistedCharacters, startofline);

        if (size_t(startofline) == std::string::npos) {
            std::cout << "\nPARSER ERROR ---->>> (startofline == std::string::npos)\n\n"
                << "Line: " << linecount;
            LOG_WARN("No whitelisted characters found after new-line character!! Line:%s", currentLine.data());
            return "";
        }
        if (size_t(endofline) == std::string::npos) {
            std::cout << "\nPARSER ERROR ---->>> (endofline == std::string::npos)\n\n"
                << "Line: " << linecount;
            LOG_WARN("No end of file (new-line) character found! Line:%s", currentLine.data());
            return "";        
        }

        linecount++;
    } while (endofline - startofline <= 0);


    auto line = std::string{
        strview.substr(startofline, endofline - startofline)
    };
    currentLine = line;

    size_t lastCharacter = (endofline - startofline) - 1;
    size_t lastValidCharacter = line.find_last_of(whitelistedCharacters);
    if (lastValidCharacter != lastCharacter) {

        std::cout << "\nPARSER ERROR ---->>> (lastValidCharacter != lastCharacter)\n\n"
            << "Line: " << linecount << '\n'
            << "Linetext: " << line << '\n'
            << "index lastValidCharacter: " << lastValidCharacter << '\n'
            << "index lastCharacter: " << lastCharacter << '\n'
            << "line[lastValidCharacter]: " << line[lastValidCharacter] << '\n'
            << "line[lastCharacter]: " << line[lastCharacter] << '\n'
            << "int(line[lastValidCharacter]): " << int(line[lastValidCharacter]) << '\n'
            << "int(line[lastCharacter]): " << int(line[lastCharacter]) << '\n';
        LOG_WARN("Non-whitelisted character found after the end of line !!");
        return "";    
    }

    /*   std::cout << std::setw(40) << std::left << line
    << " | start: "  << startofline
    << " | end: "    << endofline
    << " | DIFF: "   << endofline-startofline << '\n';
    */

    startofline = endofline + 1;
    return line;
};

auto Parser::nextKeyString() -> KeyString
{
    auto line = nextLine();
        
    //LOG_DEBUG("%s", line.data());

    // ERROR If there was an error in the nextLine function()
    if (line == "")
        return KeyString{"","", PARSE_ERROR};


    // ERROR If there are no : separator on the line
    if (line.find(":") == std::string::npos) 
        return KeyString{"","", PARSE_ERROR};
    

    std::string key = line.substr(0, line.find(":"));
    std::string valueString = "";

    // If the value is not empty
    if (line.find(":") + 2 < line.size())
        valueString = line.substr(line.find(":") + 2);

    return KeyString{ key, valueString, PARSE_SUCCESS };
};


auto Parser::nextKeyInteger() -> KeyInteger
{
    auto[key, valueString, err] = nextKeyString();
    if (err)
        return KeyInteger{"",0, PARSE_ERROR};

    // @doc https://stackoverflow.com/a/18534114 - 15.04.2018
    try {

        int integer = std::stoi(valueString);
        return KeyInteger{ key, integer, PARSE_SUCCESS };
    }
    catch (std::invalid_argument& e) {
        // if no conversion could be performed
        LOG_WARN("std::stoi catch (std::invalid_argument& e) - no conversion could be performed");
    }
    catch (std::out_of_range& e) {
        // if the converted value would fall out of the range of the result type 
        // or if the underlying function (std::strtol or std::strtoull) sets errno 
        // to ERANGE.
        LOG_WARN("std::stoi catch (std::out_of_range& e) - the converted value would fall out of the range of the result type");

    }
    return KeyInteger{"",0, PARSE_ERROR};
}


auto Parser::nextKeyFloat() -> KeyFloat
{
    auto[key, valueString, err] = nextKeyString();
    if (err == PARSE_ERROR)
        return KeyFloat{"",0, PARSE_ERROR};

    // @doc https://stackoverflow.com/a/18534114 - 15.04.2018
    try {
        float fp = std::stof(valueString);
        return KeyFloat{ key, fp, PARSE_SUCCESS };
    }
    catch (std::invalid_argument& e) {
        // if no conversion could be performed
        LOG_WARN("std::stof catch (std::invalid_argument& e) - no conversion could be performed");
    }
    catch (std::out_of_range& e) {
        // if the converted value would fall out of the range of the result type 
        // or if the underlying function (std::strtol or std::strtoull) sets errno 
        // to ERANGE.
        LOG_WARN("std::stof catch (std::out_of_range& e) - the converted value would fall out of the range of the result type");
    }
    return KeyFloat{"", 0, PARSE_ERROR};    
}

auto Parser::nextKeyVertex() -> KeyVertex { 
    

    auto[key, valueString, err] = nextKeyString();
    if (err == PARSE_ERROR)
        return KeyVertex{"", {}, PARSE_ERROR};

    Vertex vert{};
    float u, v;
    float nx, ny, nz;
                                           //   1.351676 0.301667  0.100000    0.000  0.000  1.000    0.000  0.000   255 255 255 255
    auto changecount = sscanf(valueString.data(), "%f  %f  %f    %f  %f  %f    %f  %f   %c %c %c %c", 

                 &vert.x, &vert.y, &vert.z, &nx, &ny, &nz, &u, &v, &vert.r, &vert.g, &vert.b, &vert.a);

    if (changecount != 12) {
        return KeyVertex{ "",{}, PARSE_ERROR };
    }

    vert.n = Util::packNormal(nx, ny, nz);
    vert.uv = Util::packUV(u, v);

    return KeyVertex{ key, vert, PARSE_SUCCESS };
}

auto Parser::nextKeyTriangle() -> KeyTriangle
{ 
    auto[key, valueString, err] = nextKeyString();
    if (err == PARSE_ERROR)
        return KeyTriangle{ key, {}, PARSE_ERROR};

    Triangle tri{};
    
    auto changecount = sscanf(valueString.data(), "%u %u %u", &tri.a, &tri.b, &tri.c);
    if (changecount != 3) {
        return KeyTriangle{ key,{}, PARSE_ERROR };
    }
    return KeyTriangle{key, tri, PARSE_SUCCESS };
}


auto Parser::nextKeyColor() -> KeyColor 
{
    auto[key, valueString, err] = nextKeyString();
    
    //LOG_ERROR("%s", valueString.data());

    if (err == PARSE_ERROR)
        return KeyColor{key, {}, PARSE_ERROR};

    std::stringstream ss;
    ss << valueString;

    glm::vec4 color{};

    ss >> color.r;
    if(ss.fail()){
        return KeyColor{key, {}, PARSE_ERROR};
    }
    ss >> color.g;
    if(ss.fail()){
        return KeyColor{key, {}, PARSE_ERROR};
    }
    ss >> color.b;
    if(ss.fail()){
        return KeyColor{key, {}, PARSE_ERROR};
    }
    ss >> color.a;
    if(ss.fail()){
        return KeyColor{key, {}, PARSE_ERROR};
    }
    return KeyColor{key, color, PARSE_SUCCESS };

}


auto Parser::nextKeyVec3() -> KeyVec3 
{
    auto[key, valueString, err] = nextKeyString();
    
    if (err == PARSE_ERROR)
        return KeyVec3{key, {}, PARSE_ERROR};

    std::stringstream ss;
    ss << valueString;

    glm::vec3 vec3{};

    ss >> vec3.x;
    if(ss.fail()){
        return KeyVec3{key, {}, PARSE_ERROR};
    }
    ss >> vec3.y;
    if(ss.fail()){
        return KeyVec3{key, {}, PARSE_ERROR};
    }
    ss >> vec3.z;
    if(ss.fail()){
        return KeyVec3{key, {}, PARSE_ERROR};
    }
    return KeyVec3{key, vec3, PARSE_SUCCESS };

}

//
// New API 2.0
// 
auto Parser::keyInteger(const std::string& wantedKey) -> KeyInteger
{
    auto[key, valueString, err] = nextKeyString();

    if (err)
        return KeyInteger{key, {}, err};

    if (key != wantedKey)
        return KeyInteger{key, {}, ParserErr_WrongKey};


    std::stringstream ss;
    ss << valueString;

    int myint;
    ss >> myint;
    if (ss.fail()) {
        return KeyInteger{key, {}, ParserErr_Stringstream};
    }

    return KeyInteger{key, myint, ParserSuccess};
}

auto Parser::keyString(const std::string& wantedKey) -> KeyString
{
    auto[key, valueString, err] = nextKeyString();

    if (err)
        return KeyString{key, {}, err};
    
    if (key != wantedKey)
        return KeyString{key, {}, ParserErr_WrongKey};

    return KeyString{ key, valueString, ParserSuccess };
}

auto Parser::keyVec3(const std::string& wantedKey) -> KeyVec3
{
    auto[key, valueString, err] = nextKeyString();

    if (err)
        return KeyVec3{key, {}, err};
    
    if (key != wantedKey)
        return KeyVec3{key, {}, ParserErr_WrongKey};

    std::stringstream ss;
    ss << valueString;

    glm::vec3 vec3{};

    ss >> vec3.x;
    if(ss.fail()){
        return KeyVec3{key, {}, ParserErr_Stringstream};
    }
    ss >> vec3.y;
    if(ss.fail()){
        return KeyVec3{key, {}, ParserErr_Stringstream};
    }
    ss >> vec3.z;
    if(ss.fail()){
        return KeyVec3{key, {}, ParserErr_Stringstream};
    }

    return KeyVec3{key, vec3, ParserSuccess };
}

auto Parser::keyFloat(const std::string& wantedKey) -> KeyFloat
{
    auto[key, valueString, err] = nextKeyString();
    if (err)
        return KeyFloat{key, {}, err};

    if (key != wantedKey)
        return KeyFloat{key, {}, ParserErr_WrongKey};


    std::stringstream ss;
    ss << valueString;

    float fp;
    
    ss >> fp;
    if (ss.fail()) {
        return KeyFloat{key, {}, ParserErr_Stringstream};        
    }
    return KeyFloat{key, fp, ParserSuccess};    
}


}
