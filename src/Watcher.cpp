#include <overkill/Watcher.hpp>

namespace overkill
{

#ifdef WIN32
    const auto& popen = _popen;
    const auto& pclose = _pclose;
    constexpr char PYTHON_COMMAND[] = "C:\\tools\\Python36\\python.exe discover.py";
#else
    constexpr char PYTHON_COMMAND[] = "/usr/bin/python3 discover.py";
#endif


std::vector<FileEvent> Watcher::events;

auto Watcher::popEvents(std::string eventType, std::string collection)->std::vector<FileEvent>
{
    std::vector<FileEvent> result;

    // If matching event type and matching collection name
    auto pred = [eventType, collection](const FileEvent& fevent) -> bool {
        return fevent.event_t == eventType && fevent.collection == collection;
    };

    auto foundit = std::find_if(Watcher::events.begin(), Watcher::events.end(), pred);
    while ( foundit != Watcher::events.end() )
    {
        auto erasedCopy = *foundit;
        Watcher::events.erase(foundit);
        result.push_back(erasedCopy);

        foundit = std::find_if(Watcher::events.begin(), Watcher::events.end(), pred);
    }

    return result;
}

void Watcher::discoverFiles()
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

}


