#pragma once

#include <sstream>
#include <fstream>
#include <string>

#include <PMS/logger.h>

namespace overkill::Util 
{


auto fileToString(const std::string& filepath) -> std::string;  


}