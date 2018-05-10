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

    for (;;) 
    {
        lineCount += 1;
        // After every iteration startofLine is set to the current value of endofLine +1.
        startofLine = endofLine + 1;
        if (startofLine >= (int)strview.size()) {
            LOG_ERROR("Trying to read a line which begins after the end of the file. Check your that your file has the correct amount of lines.");
        }

        endofLine = strview.find('\n', startofLine);
        if (std::size_t(endofLine) == std::string::npos) {
            LOG_ERROR("End of line character (\n) not found. Are you missing an end of line character at the end of file?");
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
// <KEY>: <STRING>
//
auto Parser::keyString() -> KeyString
{
    auto line = nextLine();

    auto separatorPosition = line.find(":");
    if (separatorPosition == std::string::npos) {
        LOG_WARN("Could not find the ':' separator on line. Every line should have the syntax <key>: <value>. Line: %s", std::string(line).data());
        return KeyString{ line,{}, ParserErr_MissingSeparator };
    }

    std::string_view key = line.substr(0, separatorPosition);

    if (separatorPosition + 2 >= line.size()) {
        LOG_WARN("Could not find any <value> in <key>: <value> line. Line: %s", std::string(line).data());
        return KeyString{ key,{}, ParserErr_MissingValue };
    }
    std::string_view value = line.substr(separatorPosition + 2);

    return KeyString{ key, value, ParserSuccess };
}

auto Parser::keyString(const std::string_view wantedKey) -> KeyString
{
    auto keyString_ = keyString();

    if (keyString_.key != wantedKey) {
        LOG_ERROR("The key wanted was different from the key found,  key: %s vs wantedKey: %s", std::string(keyString_.key).data(), wantedKey.data());
        return KeyString{ keyString_.key, keyString_.value, ParserErr_WrongKey };
    }
    return keyString_;
}






//
// <KEY>: <INTEGER>
//
auto Parser::keyInteger() -> KeyInteger
{
    auto[key, value, err] = keyString();
    if (err)
        return KeyInteger{key, 0, err };

    // @doc https://stackoverflow.com/a/18534114 - 15.04.2018
    
    std::stringstream ss;
    ss << value;

    int integer;
    ss >> integer;
    if (ss.fail()) {
        return KeyInteger{ key, 0, ParserErr_Stringstream };
    }

    return KeyInteger{ key, integer, ParserSuccess };
}

auto Parser::keyInteger(const std::string_view wantedKey) -> KeyInteger
{
    auto keyInteger_ = keyInteger();
    if (keyInteger_.err)
        return keyInteger_;

    if (keyInteger_.key != wantedKey) {
        LOG_ERROR("The key wanted was different from the key found,  key: %s vs wantedKey: %s", std::string(keyInteger_.key).data(), wantedKey.data());
        return KeyInteger{ keyInteger_.key, keyInteger_.value, ParserErr_WrongKey };
    }
    return keyInteger_;
}




//
// <KEY>: <FLOAT>
//
auto Parser::keyFloat() -> KeyFloat
{
    auto[key, value, err] = keyString();
    if (err)
        return KeyFloat{key, 0.0f, err};

    std::stringstream ss;
    ss << value;

    float fp;
    ss >> fp;
    if (ss.fail()) {
        return KeyFloat{key, 0.0f, ParserErr_Stringstream};        
    }
    return KeyFloat{key, fp, ParserSuccess};    
}

auto Parser::keyFloat(const std::string_view wantedKey) -> KeyFloat 
{
    auto keyFloat_ = keyFloat();
    if (keyFloat_.err)
        return keyFloat_;

    if (keyFloat_.key != wantedKey) {
        LOG_ERROR("The key wanted was different from the key found,  key: %s vs wantedKey: %s", std::string(keyFloat_.key).data(), wantedKey.data());
        return KeyFloat{ keyFloat_.key, keyFloat_.value, ParserErr_WrongKey };
    }
    return keyFloat_;
}






//
// <KEY>: <VEC3>
// <KEY>: <float> <float> <float>
//
auto Parser::keyVec3() -> KeyVec3
{
    auto[key, value, err] = keyString();

    if (err)
        return KeyVec3{key, {}, err};

    std::stringstream ss;
    ss << value;

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

auto Parser::keyVec3(const std::string_view wantedKey) -> KeyVec3 
{
    auto keyVec3_ = keyVec3();
    if (keyVec3_.err)
        return keyVec3_;

    if (keyVec3_.key != wantedKey) {
        LOG_ERROR("The key wanted was different from the key found,  key: %s vs wantedKey: %s", std::string(keyVec3_.key).data(), wantedKey.data());
        return KeyVec3{ keyVec3_.key, {}, ParserErr_WrongKey };
    }

    return keyVec3_;
}




//
// <KEY>: <VEC4>
// <KEY>: <FLOAT> <FLOAT> <FLOAT> <FLOAT>
//
auto Parser::keyVec4() -> KeyVec4
{
    auto[key, value, err] = keyString();

    if (err)
        return KeyVec4{ key, {}, err };

    std::stringstream ss;
    ss << value;

    glm::vec4 vector4{};

    ss >> vector4.r;
    if (ss.fail()) {
        return KeyVec4{ key,{}, ParserErr_Stringstream };
    }
    ss >> vector4.g;
    if (ss.fail()) {
        return KeyVec4{ key,{}, ParserErr_Stringstream };
    }
    ss >> vector4.b;
    if (ss.fail()) {
        return KeyVec4{ key,{}, ParserErr_Stringstream };
    }
    ss >> vector4.a;
    if (ss.fail()) {
        return KeyVec4{ key,{}, ParserErr_Stringstream };
    }
    return KeyVec4{ key, vector4, ParserSuccess };
}

auto Parser::keyVec4(const std::string_view wantedKey) -> KeyVec4
{
    auto keyVec4_ = keyVec4();
    if (keyVec4_.err)
        return keyVec4_;

    if (keyVec4_.key != wantedKey) {
        LOG_ERROR("The key wanted was different from the key found,  key: %s vs wantedKey: %s", std::string(keyVec4_.key).data(), wantedKey.data());
        return KeyVec4{ keyVec4_.key, keyVec4_.value, ParserErr_WrongKey };
    }
    return keyVec4_;
}





auto Parser::keyEnums(const std::string_view expectedKey,
                      const std::vector<std::string_view> expectedArguments,
                      const std::size_t argumentCount) -> KeyEnums
{

    auto toUpperString = [](const std::string_view str) -> std::string
    {
        std::string res;
        res.resize(str.size()); 
        for (std::size_t i = 0; i < str.size(); i++) 
        {
            res[i] = std::toupper(str[i]);
        }
        return res;
    };


    //
    // 0. Parse next key-value strings
    //
    auto[parsedKey, parsedString, err] = keyString();

    if (err)
        return KeyEnums{ parsedKey, {}, err };


    //
    // 1. Parse the input string argument by argument
    //
    std::stringstream wordEater;
    std::vector<std::string> parsedArguments;
    
    wordEater << parsedString;
    std::string arg;
    wordEater >> arg;
    
    while(!wordEater.fail()) {
        parsedArguments.push_back(arg);
        wordEater >> arg;
    }

    //
    // 2. Check that number of parsedArguments match expected
    //
    if (parsedArguments.size() < argumentCount) {
        LOG_WARN("ERROR:     Too few arguments.");
        LOG_WARN("Expected:  %lu", argumentCount);
        LOG_WARN("Got:       %lu", parsedArguments.size());  
        return KeyEnums{ parsedKey, parsedArguments, ParserErr};
    }


    if (parsedArguments.size() > argumentCount) {
        LOG_WARN("ERROR:     Too many arguments.");
        LOG_WARN("Expected:  %lu",   argumentCount);
        LOG_WARN("Got:       %lu", parsedArguments.size());
        return KeyEnums{ parsedKey, parsedArguments, ParserErr};
    }


    //
    // 3. Check if expected key equals parsed key
    //
    if (toUpperString(expectedKey) != toUpperString(parsedKey)) 
    {
        LOG_WARN("\nERROR:  Parsedkey not equal expected key");
        LOG_WARN("Expected: %s\\n", expectedKey.data());
        LOG_WARN("Got:      %s\\n", parsedKey.data());
        return KeyEnums{parsedKey, parsedArguments, ParserErr};
    }

    //
    // 4. Validate n arguments
    //
    for (const auto& arg: parsedArguments) 
    {
        bool found = false;        
        for(auto& expectedarg: expectedArguments) 
        {
            if (toUpperString(arg) == toUpperString(expectedarg)) {
                found = true;
                break;
            }
        }

        //
        // 5. If parsedArgument was not in expectedArguments
        //
        if (!found) 
        {
            LOG_WARN("\nERROR: Invalid enum argument\n");
            LOG_WARN("Expected one of:\n  [");
            int count = 0;
            for(auto& expectedArg: expectedArguments) {

                LOG_WARN("  %s", expectedArg.data());
                if (count < expectedArguments.size() - 1) {
                    LOG_WARN("   ,");
                }

                count++;
            }
            LOG_WARN("\n  ];");
            LOG_WARN("Got: %s", arg.data());
            return KeyEnums{ parsedKey, parsedArguments, ParserErr};
        }
    }

    return KeyEnums{ parsedKey, parsedArguments, ParserSuccess };
}




//
//  v: <float> <float> <float>  <float> <float> <float>  <float> <float>  <byte> <byte> <byte> <byte>
//
auto Parser::onlyVertex() -> Vertex 
{
    // @doc Reading material for optimizing this function https://tinodidriksen.com/2011/05/cpp-convert-string-to-double-speed/ - 06.05.2018
    // @note This is not a 'correct' implementation of string to float, and neither is it intended that way.
    // This is designed to parse the floats which are present in this system only, and ONLY in the model files.
    // The function concerns performance only.
    // This function replaces strtof() - which is a much more correct implemetation, but was a performance bottleneck loading 
    // bigger models - JSolsvik 06.05.2018
    // @optmization Function will only work here. Not genral at all
    auto __OPTMIZED__StringToFloat = [](const char *p, const char **end) -> float 
    {
        float res = 0.0f;
        // Eat spaces
        while (*p == ' ') ++p;

        // If negative
        bool neg = false;
        if (*p == '-') {
            neg = true;
            ++p;
        }
        while (*p >= '0' && *p <= '9') {
            res = (res*10.0f) + (*p - '0');
            ++p;
        }
     

        // @NOTE I tried to comment out this if-check, making it an assumption that all floats
        //       has to be formatted with a '.' separating the two parts of the float.
        //       This has no noticable affect on performance so I decided it was not worth
        //       using it for the reduced flexibility. IMHO you have to gain quite huge performance
        //       improvements to justify reductions in flexibility and robustness/safety - JSolsvik 07.05.2018
        if (*p == '.') {
     
            float f = 0.0f;
            int n = 0;
            ++p;
            while (*p >= '0' && *p <= '9') {
                f = (f*10.0f) + (*p - '0');
                ++p;
                ++n;
            }

            // @note Trying to optimize pow below 
            //          
            //        res += f / std::pow(10.0, n);
            //
            //       Results: The following is 10-20x faster compared to using the std::pow function
            //                Totally worth throwing it away. It seems that the pow function is too general.
            //                x * x * x * x .... beats it any day - JSolsvik 07.05.2018
            float tenToThePowerOfN = 1.0;
            while (n) {
                tenToThePowerOfN *= 10.0;
                --n;
            }
            res += f / tenToThePowerOfN;
       
        }
       
        if (neg) {
            res = -res;
        }
        *end = p;
        return res;
    };

    const auto line = nextLine();
    const char* it = line.data();
    const char* end;

    // Find the separtor, the plus 1
    while(*it != ':') ++it;
    ++it;


    // Position
    const float x = __OPTMIZED__StringToFloat(it, &end);
    it = end;
    const float y = __OPTMIZED__StringToFloat(it, &end);
    it = end;
    const float z = __OPTMIZED__StringToFloat(it, &end);
    it = end;

    // Normal
    const float nx = __OPTMIZED__StringToFloat(it, &end);
    it = end;
    const float ny = __OPTMIZED__StringToFloat(it, &end);
    it = end;
    const float nz = __OPTMIZED__StringToFloat(it, &end);
    it = end;

    const GLint n = Util::packNormal(nx,ny,nz);


    // uv
    const GLushort u =  static_cast<GLushort>(65535U * __OPTMIZED__StringToFloat(it, &end));
    it = end;
    const GLushort v = static_cast<GLushort>(65535U *  __OPTMIZED__StringToFloat(it, &end));
    it = end;



    // @optmization Function will only work here. Not genral at all
    auto __OPTIMIZED__StringToGLubyte = [](const char *p, const char **end) -> GLubyte 
    {
        while (*p == ' ') ++p;

        GLubyte x = 0;
        while (*p >= '0' && *p <= '9') {
            x = (x*10) + (*p - '0');
            ++p;
        }
        *end = p;
        return x;
    };

    // Colors
    const GLubyte r = __OPTIMIZED__StringToGLubyte(it, &end);
    it = end;
    const GLubyte g = __OPTIMIZED__StringToGLubyte(it, &end);
    it = end;
    const GLubyte b = __OPTIMIZED__StringToGLubyte(it, &end);
    it = end;
    const GLubyte a = __OPTIMIZED__StringToGLubyte(it, &end);

    return Vertex {
        x,y,z, n, u,v, r,g,b,a
    };
}




//
// t: <GLuint> <GLuint> <GLuint>
//
auto Parser::onlyTriangle() -> Triangle
{

    // @optmization Function will only work here. Not genral at all
    auto __OPTIMIZED__StringToGLuint = [](const char *p, const char **end) -> GLuint 
    {
        while (*p == ' ') ++p;

        GLuint x = 0;
        while (*p >= '0' && *p <= '9') {
            x = (x*10) + (*p - '0');
            ++p;
        }
        *end = p;
        return x;
    };



    const auto line = nextLine();

    const char* it = line.data();
    const char* end;

    // Find the separtor, the plus 1
    while(*it != ':') ++it;
    ++it;
    
    Triangle tri{};
    tri.a = __OPTIMIZED__StringToGLuint(it, &end);
    it = end;
    tri.b = __OPTIMIZED__StringToGLuint(it, &end);
    it = end;
    tri.c = __OPTIMIZED__StringToGLuint(it, &end);

    return tri;
}

}
