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


    std::stringstream ss;
    ss << valueString;

    Vertex vert{};

    // Parsing position
    ss >> vert.x;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }
    ss >> vert.y;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }
    ss >> vert.z;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }



    // Parsing normal
    //float nx, ny, nz;
    ss >> vert.nx >> vert.ny >> vert.nz;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }
   // vert.normal = Util::packNormal(nx, ny, nz);




    // Parsing UV
 //   float u, v;
    ss >> vert.u >> vert.v;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }
    //vert.uv = Util::packUV(u, v);



    // Parsing RGBA
    unsigned int color;

    ss >> color;  
    vert.r = color;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }
    ss >> color; 
    vert.g = color;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }
    ss >> color;
    vert.b = color;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }
    ss >> color; 
    vert.a = color;
    if(ss.fail()){
        return KeyVertex{"", {}, PARSE_ERROR};
    }

    return KeyVertex{key, vert, PARSE_SUCCESS };
}
auto Parser::nextKeyTriangle() -> KeyTriangle
{ 
    auto[key, valueString, err] = nextKeyString();
    if (err == PARSE_ERROR)
        return KeyTriangle{"", {}, PARSE_ERROR};

    std::stringstream ss;
    ss << valueString;

    Triangle tri{};
    ss >> tri.a;
    if(ss.fail()){
        return KeyTriangle{"", {}, PARSE_ERROR};
    }
    ss >> tri.b;
    if(ss.fail()){
        return KeyTriangle{"", {}, PARSE_ERROR};
    }
    ss >> tri.c;
    if(ss.fail()){
        return KeyTriangle{"", {}, PARSE_ERROR};
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

}
