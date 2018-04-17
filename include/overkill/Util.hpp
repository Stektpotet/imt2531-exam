#pragma once

#include <sstream>
#include <fstream>
#include <string>

#include <PMS/logger.h>
#include <overkill/MaterialSystem.hpp>
#include <overkill/Watcher.hpp>

namespace overkill::Util 
{


auto fileToString(const std::string& filepath) -> std::string;  
void everyTwoSeconds(float t);

}