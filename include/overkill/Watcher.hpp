#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>  
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <iostream>
#include <algorithm>


namespace overkill 
{

struct FileEvent
{
    std::string event_t;          // discovered (created, deleted, modified, moved)
    std::string collection;
    std::string tag;
    std::string extension;
};

class Watcher
{
private:
    static std::vector<FileEvent> events;

public:
    static auto popEvents(std::string eventType, std::string collection)->std::vector<FileEvent>;
    static void discoverFiles();
};

}
