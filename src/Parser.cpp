#include <overkill/Parser.hpp>


namespace overkill 
{
Parser::Parser(std::string_view _strview)
:strview(_strview)
{}


auto Parser::nextLine() -> std::string
{
    endofline = strview.find('\n', startofline);
    startofline = strview.find_first_of(whitelistedCharacters, startofline);

    if (startofline == std::string::npos) {
        std::cout << "\nPARSER ERROR ---->>> (startofline == std::string::npos)\n\n"
            << "Line: " << linecount;
        LOG_ERROR("No whitelisted characters found after new-line character!! Line:");
    }
    if (endofline == std::string::npos) {
        std::cout << "\nPARSER ERROR ---->>> (endofline == std::string::npos)\n\n"
            << "Line: " << linecount;
        LOG_ERROR("No end of file (new-line) character found! Line:");
    }

    linecount++;
    // Skip empty lines in between and eat whitespace
    while (endofline - startofline <= 0) {

        startofline = endofline + 1;
        endofline = strview.find('\n', startofline);
        startofline = strview.find_first_of(whitelistedCharacters, startofline);

        if (startofline == std::string::npos) {
            std::cout << "\nPARSER ERROR ---->>> (startofline == std::string::npos)\n\n"
                << "Line: " << linecount;
            LOG_ERROR("No whitelisted characters found after new-line character!! Line:");
        }
        if (endofline == std::string::npos) {
            std::cout << "PARSER ERROR ---->>> (endofline == std::string::npos)"
                << "Line: " << linecount;
            LOG_ERROR("No end of file (new-line) character found! Line:");
        }
        linecount++;
    }

    auto line = std::string{
        strview.substr(startofline, endofline - startofline)
    };
    auto lastCharacter = (endofline - startofline) - 1;
    auto lastValidCharacter = line.find_last_of(whitelistedCharacters);
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
        LOG_ERROR("Non-whitelisted character found after the end of line !!");
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

    std::string key = line.substr(0, line.find(":"));
    std::string valueString = "";

    if (line.find(":") + 2 < line.size())
        valueString = line.substr(line.find(":") + 2);

#ifdef DEBUG
    std::stringstream ss;
    ss  << " key: "
        << std::setw(18) << std::left
        << key

        << " val: "
        << valueString;

    LOG_DEBUG("%s", ss.str().c_str());
#endif

    return KeyString{ key, valueString };
};


auto Parser::nextKeyInteger() -> KeyInteger
{
    auto[key, valueString] = nextKeyString();

    // @doc https://stackoverflow.com/a/18534114 - 15.04.2018
    try {

        int integer = std::stoi(valueString);
        return KeyInteger{ key, integer };
    }
    catch (std::invalid_argument& e) {
        // if no conversion could be performed
        LOG_ERROR("std::stoi catch (std::invalid_argument& e) - no conversion could be performed");
    }
    catch (std::out_of_range& e) {
        // if the converted value would fall out of the range of the result type 
        // or if the underlying function (std::strtol or std::strtoull) sets errno 
        // to ERANGE.
        LOG_ERROR("std::stoi catch (std::out_of_range& e) - the converted value would fall out of the range of the result type");

    }
}


auto Parser::nextKeyFloat() -> KeyFloat
{
    auto[key, valueString] = nextKeyString();

    // @doc https://stackoverflow.com/a/18534114 - 15.04.2018
    try {
        float fp = std::stof(valueString);
        return KeyFloat{ key, fp };
    }
    catch (std::invalid_argument& e) {
        // if no conversion could be performed
        LOG_ERROR("std::stof catch (std::invalid_argument& e) - no conversion could be performed");
    }
    catch (std::out_of_range& e) {
        // if the converted value would fall out of the range of the result type 
        // or if the underlying function (std::strtol or std::strtoull) sets errno 
        // to ERANGE.
        LOG_ERROR("std::stof catch (std::out_of_range& e) - the converted value would fall out of the range of the result type");
    }
}

auto Parser::nextKeyVertex() -> KeyVertex { return KeyVertex{}; }
auto Parser::nextKeyTriangle() -> KeyTriangle { return KeyTriangle{};  }
auto Parser::nextKeyFilepath() -> KeyString { return KeyString{}; }

}
