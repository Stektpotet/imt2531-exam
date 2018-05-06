#include <overkill/Parser.hpp>
#include <cctype>

namespace overkill 
{
Parser::Parser(std::string_view strview_)
:strview(strview_)
{}


auto Parser::nextLine() -> std::string_view
{
    // endofLine - the position of the next new-line character
    // startofLine - The position of the beginning of a line. This position is the same as the 
    //               previos end of line + 1.
    // 
    //            Initially, start of line is the same as start of string. Therefore end of 
    //               line must initially be -1 to keep the relationship between startofLine and 
    //               endofLine consistent through all iterations.
    //               Note that string::npos is also -1. Be carefull to not confuse these two values.
    //               After every strview.find() there is a chance that the result is string::npos,
    //               which means "not found".

    for (;;) {
        this->lineCount += 1;
        // After every iteration startofLine is set to the current value of endofLine +1.
        startofLine = endofLine + 1;
        if (startofLine >= strview.size()) {
            LOG_WARN("Trying to read a line which begins after the end of the file. Check your that your file has the correct amount of lines.");
            return "";
        }

        endofLine = strview.find('\n', startofLine);
        if (size_t(endofLine) == std::string::npos) {
            LOG_WARN("End of line character (\n) not found. Are you missing an end of line character at the end of file?");
            return "";
        }

        // Eat spaces and tabs. 
        for (;; startofLine += 1)
        {
            if (strview[startofLine] != ' '
            &&  strview[startofLine] != '\t')
            {
                break;
            }
        }
        // At this point we already know there has to be a new-line character further down the string.
        // Therefore we know that startofline can never be greater than end of line.
        // If start of line is less than end of line, it means that it found something other than a new
        // line character, space or tab.
        // If start of line equals end of line, it means that the line was empty, and we want to
        // continue reading lines.
        if (startofLine == endofLine) {
            continue;
        }
     
        // Skip comments
        if (strview[startofLine] == '#') {
            continue;
        }

        // In every other case just break out of the for-loop.
        // This means that have found the beginning and end of a proper line
        break;
    }


    // What we know at this point
    // 1. StartOfLine is pointing at the beginning of text
    // 2. This text is not a comment
    // 3. StartofLine i less than endOfLine
    // 4. Both startofLine and endOfline is less than end of file

    // Whitespace is trimmed in front but not in the back of the line
    // The question is, does it matter? Scanf will just ignore whitespace anyways right?
    // Since scanf() ignores whitespace we can safely return the current line
    return strview.substr(startofLine, endofLine - startofLine);
};

//
// New API 2.0
// 

auto Parser::keyString(const std::string_view wantedKey) -> KeyString
{
    auto line = nextLine();

    if (line.size() == 0) {
        LOG_WARN("Failed getting the line. Got an empty line.");
        return KeyString{ line, {}, PARSE_ERROR };
    }

    auto separatorPosition = line.find(":");
    if (separatorPosition == std::string::npos) {
        LOG_WARN("Could not find the ':' separator on line. Every line should have the syntax <key>: <value>. Line: %s", line.data());
        return KeyString{ line, {}, PARSE_ERROR };
    }

    std::string_view key = line.substr(0, separatorPosition);

    if (separatorPosition + 2 >= line.size()) {
        LOG_WARN("Could not find any <value> in <key>: <value> line. Line: %s", line.data());
        return KeyString{ key, {}, PARSE_ERROR };
    }
    std::string_view value = line.substr(separatorPosition + 2);

    if (key != wantedKey) {
        LOG_WARN("The key wanted was different from the key found,  key: %s vs wantedKey: %s", key.data(), wantedKey.data());
        return KeyString{ key, {}, ParserErr_WrongKey };
    }
    return KeyString{ key, value, ParserSuccess };
}

auto Parser::keyString() -> KeyString
{
    auto line = nextLine();

    if (line.size() == 0) {
        LOG_WARN("Failed getting the line. Got an empty line.");
        return KeyString{ line,{}, PARSE_ERROR };
    }

    auto separatorPosition = line.find(":");
    if (separatorPosition == std::string::npos) {
        LOG_WARN("Could not find the ':' separator on line. Every line should have the syntax <key>: <value>. Line: %s", line.data());
        return KeyString{ line,{}, PARSE_ERROR };
    }

    std::string_view key = line.substr(0, separatorPosition);

    if (separatorPosition + 2 >= line.size()) {
        LOG_WARN("Could not find any <value> in <key>: <value> line. Line: %s", line.data());
        return KeyString{ key,{}, PARSE_ERROR };
    }
    std::string_view value = line.substr(separatorPosition + 2);

    return KeyString{ key, value, ParserSuccess };
}


auto Parser::keyInteger(const std::string_view wantedKey) -> KeyInteger
{
    auto[key, valueString, err] = keyString(wantedKey);

    if (err)
        return KeyInteger{key, {}, err};

    std::stringstream ss;
    ss << valueString;

    int myint;
    ss >> myint;
    if (ss.fail()) {
        return KeyInteger{key, {}, ParserErr_Stringstream};
    }

    return KeyInteger{key, myint, ParserSuccess};
}


auto Parser::keyVec3(const std::string_view wantedKey) -> KeyVec3
{
    auto[key, valueString, err] = keyString(wantedKey);

    if (err)
        return KeyVec3{key, {}, err};

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

auto Parser::keyFloat(const std::string_view wantedKey) -> KeyFloat
{
    auto[key, valueString, err] = keyString(wantedKey);
    if (err)
        return KeyFloat{key, {}, err};


    std::stringstream ss;
    ss << valueString;

    float fp;
    
    ss >> fp;
    if (ss.fail()) {
        return KeyFloat{key, {}, ParserErr_Stringstream};        
    }
    return KeyFloat{key, fp, ParserSuccess};    
}

//
// DEPRECATED FUNCTIONS
//
auto Parser::nextKeyString() -> KeyString
{
    auto line = nextLine();

    //LOG_DEBUG("%s", line.data());

    // ERROR If there was an error in the nextLine function()
    if (line == "")
        return KeyString{ "","", PARSE_ERROR };


    // ERROR If there are no : separator on the line
    if (line.find(":") == std::string::npos)
        return KeyString{ "","", PARSE_ERROR };


    auto key = line.substr(0, line.find(":"));
    std::string_view valueString = "";

    // If the value is not empty
    if (line.find(":") + 2 < line.size())
        valueString = line.substr(line.find(":") + 2);

    return KeyString{ key, valueString, PARSE_SUCCESS };
};


auto Parser::nextKeyInteger() -> KeyInteger
{
    auto[key, valueString, err] = nextKeyString();
    if (err)
        return KeyInteger{key, 0, PARSE_ERROR };

    // @doc https://stackoverflow.com/a/18534114 - 15.04.2018
    try {

        int integer = std::stoi(std::string(valueString));
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
    return KeyInteger{ "",0, PARSE_ERROR };
}


auto Parser::nextKeyFloat() -> KeyFloat
{
    auto[key, valueString, err] = nextKeyString();
    if (err == PARSE_ERROR)
        return KeyFloat{ key, 0, PARSE_ERROR };

    // @doc https://stackoverflow.com/a/18534114 - 15.04.2018
    try {
        float fp = std::stof(std::string(valueString));
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
    return KeyFloat{ key, 0, PARSE_ERROR };
}

// 128 bytes in total, two L1 cache lines
char globalScanfTempStorage[96];
char globalScanfTempStorageSmall[32];

auto Parser::nextKeyVertex() -> KeyVertex {


    auto[key, vertexString, err] = keyString();
    if (err == PARSE_ERROR)
        return KeyVertex{ key, {}, PARSE_ERROR };

    Vertex vert{};
    float u, v;
    float nx, ny, nz;
    //   1.351676 0.301667  0.100000    0.000  0.000  1.000    0.000  0.000   255 255 255 255

    /*
    strncpy(globalScanfTempStorage, vertexString.data(), vertexString.size());
    globalScanfTempStorage[vertexString.size()] = '\0';
    auto changecount = sscanf(globalScanfTempStorage, "%f  %f  %f    %f  %f  %f    %f  %f   %c %c %c %c",
        &vert.x, &vert.y, &vert.z, 
        &nx, &ny, &nz, 
        &u, &v, 
        &vert.r, &vert.g, &vert.b, &vert.a);

    if (changecount != 12) {
        LOG_WARN("A vertex should have 12 values. %d found", changecount);
        return KeyVertex{ {}, {}, PARSE_ERROR };
    }
    */

    const char* it = vertexString.data();
    char* end;
    
    // Position
    vert.x = strtof(it, &end);
    it = end;
    vert.y = strtof(it, &end);
    it = end;
    vert.z = strtof(it, &end);
    it = end;

    // Normal
    nx = strtof(it, &end);
    it = end;
    ny = strtof(it, &end);
    it = end;
    nz = strtof(it, &end);
    it = end;

    // uv
    u = strtof(it, &end);
    it = end;
    v = strtof(it, &end);
    it = end;

    // Colors
    vert.r = (GLubyte)strtoul(it, &end, 10);
    it = end;
    vert.g = (GLubyte)strtoul(it, &end, 10);
    it = end;
    vert.b = (GLubyte)strtoul(it, &end, 10);
    it = end;
    vert.a = (GLubyte)strtoul(it, &end, 10);
    it = end;

    // Packing and normalizing
    vert.n = Util::packNormal(nx, ny, nz);
    vert.u = 65535U * u;//vert.uv = Util::packUV(u, v);
    vert.v = 65535U * v;

    return KeyVertex{ key, vert, PARSE_SUCCESS };
}

auto Parser::nextKeyTriangle() -> KeyTriangle
{
    auto[key, triangleString, err] = keyString();
    if (err == PARSE_ERROR)
        return KeyTriangle{ key,{}, PARSE_ERROR };

    Triangle tri{};

    const char* it = triangleString.data();
    char* end;
    
    tri.a = strtol(it, &end, 10);
    it = end;
    tri.b = strtol(it, &end, 10);
    it = end;
    tri.c = strtol(it, &end, 10);

    return KeyTriangle{ key, tri, PARSE_SUCCESS };
}


auto Parser::nextKeyColor() -> KeyColor
{
    auto[key, valueString, err] = nextKeyString();

    //LOG_ERROR("%s", valueString.data());

    if (err == PARSE_ERROR)
        return KeyColor{ key,{}, PARSE_ERROR };

    std::stringstream ss;
    ss << valueString;

    glm::vec4 color{};

    ss >> color.r;
    if (ss.fail()) {
        return KeyColor{ key,{}, PARSE_ERROR };
    }
    ss >> color.g;
    if (ss.fail()) {
        return KeyColor{ key,{}, PARSE_ERROR };
    }
    ss >> color.b;
    if (ss.fail()) {
        return KeyColor{ key,{}, PARSE_ERROR };
    }
    ss >> color.a;
    if (ss.fail()) {
        return KeyColor{ key,{}, PARSE_ERROR };
    }
    return KeyColor{ key, color, PARSE_SUCCESS };

}


auto Parser::nextKeyVec3() -> KeyVec3
{
    auto[key, valueString, err] = nextKeyString();

    if (err == PARSE_ERROR)
        return KeyVec3{ key,{}, PARSE_ERROR };

    std::stringstream ss;
    ss << valueString;

    glm::vec3 vec3{};

    ss >> vec3.x;
    if (ss.fail()) {
        return KeyVec3{ key,{}, PARSE_ERROR };
    }
    ss >> vec3.y;
    if (ss.fail()) {
        return KeyVec3{ key,{}, PARSE_ERROR };
    }
    ss >> vec3.z;
    if (ss.fail()) {
        return KeyVec3{ key,{}, PARSE_ERROR };
    }
    return KeyVec3{ key, vec3, PARSE_SUCCESS };

}


}
