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

#ifdef WIN32
const auto& popen = _popen;
const auto& pclose = _pclose;
constexpr char PYTHON_COMMAND[] = "C:\\tools\\Python36\\python.exe discover.py";
#else
constexpr char PYTHON_COMMAND[] = "/usr/bin/python3 discover.py";
#endif

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
    static void pollEvents();
};


std::vector<FileEvent> Watcher::events;

auto Watcher::popEvents(std::string eventType, std::string collection)->std::vector<FileEvent>
{
    std::vector<FileEvent> result;

    auto pred = [eventType, collection](const FileEvent& fevent) -> bool {
        return fevent.event_t == eventType && fevent.collection == collection;
    };

    auto foundit = events.begin();
    while ((foundit = std::find_if(Watcher::events.begin(), Watcher::events.end(), pred)) != Watcher::events.end())
    {
        auto erasedcopy = *foundit;
        Watcher::events.erase(foundit);

        result.push_back(erasedcopy);
    }

    return result;
}

void Watcher::pollEvents()
{
    using namespace std::chrono_literals;

    const int DATA_SIZE = 128;
    char  data[DATA_SIZE] = "\n";

    FILE* pipe = popen(PYTHON_COMMAND, "r");
    if (!pipe) {
        printf("FAILED OPENING the pipe\n");
        return;
    }

    char* msg;
    while (msg = fgets(data, DATA_SIZE, pipe)) {
        printf("msg %s", msg);


        std::stringstream ss;
        FileEvent fevent;
        ss << msg;
        ss >> fevent.event_t >> fevent.collection >> fevent.tag >> fevent.extension;

        events.push_back(fevent);
    }

    if (int notclosed = pclose(pipe); notclosed) {
        printf("FAILED CLOSING the pipe\n");
        return;
    }
    printf("CLOSING the pipe\n");
}
int main()
{
    Watcher::pollEvents();
    auto tevents = Watcher::popEvents("discovered", "textures");
    auto mevents = Watcher::popEvents("discovered", "shaders");
    auto matevents = Watcher::popEvents("discovered", "materials");
    auto modevents = Watcher::popEvents("discovered", "models");
    auto sceneevents = Watcher::popEvents("discovered", "scenes");


    std::cin.get();
    return 0;
}
