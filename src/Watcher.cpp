#include <overkill/Watcher.hpp>

namespace overkill
{

#ifdef WIN32
    const auto& popen = _popen;
    const auto& pclose = _pclose;
#endif


std::vector<FileEvent> Watcher::events;

auto Watcher::popEvents(std::string eventType, std::string collection)->std::vector<FileEvent>
{
    std::vector<FileEvent> result;

    // If matching event type and matching collection name
    auto pred = [eventType, collection] (const FileEvent& fevent) -> bool {
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


void Watcher::pollEvents()
{
    using namespace std::chrono_literals;

    std::string PYTHON_COMMAND = {C::PythonPath + " tool/discover.py"};

    const int DATA_SIZE = 128;
    char  data[DATA_SIZE] = "\n";

    FILE* pipe = popen(PYTHON_COMMAND.data(), "r");
    if (!pipe) {
        LOG_ERROR("FAILED OPENING python pipe");
    }
    LOG_INFO("OPENING python pipe %s", PYTHON_COMMAND.data());


    char* msg;
    while ((msg = fgets(data, DATA_SIZE, pipe))) {

        // zero out newline character for prettyer print out
        msg[strlen(msg)-1] = '\0'; 
        LOG_INFO("event %s", msg);


        std::stringstream ss;
        FileEvent fevent;
        ss << msg;
        ss >> fevent.event_t >> fevent.collection >> fevent.tag >> fevent.extension;

        events.push_back(fevent);
    }

    if (int notclosed = pclose(pipe); notclosed) {
        LOG_ERROR("FAILED CLOSING python pipe");
        return;
    }
    LOG_INFO("CLOSING python pipe");
}

}


