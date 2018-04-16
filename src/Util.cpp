#include <overkill/Util.hpp>

namespace overkill 
{

auto Util::fileToString(const std::string& filepath) -> std::string
{
    std::ifstream infile(filepath);

    if (!infile) {
        LOG_ERROR("file not found: %s", filepath.data());
    }
    std::stringstream ss;
    ss << infile.rdbuf();
    return ss.str();
}


}
