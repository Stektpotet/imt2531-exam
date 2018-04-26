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
constexpr char PYTHON_DISCOVER[] = "C:\\tools\\Python36\\python.exe tool\\tool\\discover.py";
constexpr char PYTHON_MODIFIED[] = "C:\\tools\\Python36\\python.exe tool\\tool\\modified.py";
#else
constexpr char PYTHON_DISCOVER[] = "/usr/bin/python3 tool/discover.py";
constexpr char PYTHON_MODIFIED[] = "/usr/bin/python3 tool/modified.py";
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

    static FILE* modifiedpipe;

public:
    static auto popEvents(std::string eventType, std::string collection)->std::vector<FileEvent>;
    static void discoverFiles();
    static auto openModified() -> char;
    static auto pollModified() -> char;
    static auto closeModified() -> char;
};


std::vector<FileEvent> Watcher::events;
FILE* Watcher::modifiedpipe;

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

void Watcher::discoverFiles()
{
    using namespace std::chrono_literals;



    FILE* pipe = popen(PYTHON_DISCOVER, "r");
    if (!pipe) {
        printf("FAILED OPENING the pipe\n");
        return;
    }


    const int DATA_SIZE = 128;
    char  data[DATA_SIZE] = "\n";
    char* msg;
    while ((msg = fgets(data, DATA_SIZE, pipe))) {
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



auto Watcher::openModified() -> char
{
    Watcher::modifiedpipe = popen(PYTHON_MODIFIED, "r");
    if (!Watcher::modifiedpipe) {
        printf("FAILED OPENING the pipe\n");
        return 1;
    }
    printf("OPENING the modified pipe\n");
    return 0;
}

auto Watcher::pollModified() -> char
{
    const int DATA_SIZE = 128;
    char  data[DATA_SIZE] = "\n";

    char* lastmsg; 
    char* msg;

    while ((msg = fgets(data, DATA_SIZE, Watcher::modifiedpipe))) {
        strcpy(lastmsg, msg);
/*      
        std::stringstream ss;
        FileEvent fevent;
        ss << msg;
        ss >> fevent.event_t >> fevent.collection >> fevent.tag >> fevent.extension;
        events.push_back(fevent);*/
    } 

    printf("msg %s\n", msg);
}

auto Watcher::closeModified() -> char
{
    if (int notclosed = pclose(Watcher::modifiedpipe); notclosed) {
        printf("FAILED CLOSING the pipe\n");
        return 1;
    }
    printf("CLOSING the modified pipe\n");
    return 0;
}


int main()
{
//    Watcher::discoverFiles();

    if (auto err = Watcher::openModified(); err) {
        return 1;
    }

    for(;;) 
    {
        printf("\n\nPOLLING\n");
        
        if(auto err = Watcher::pollModified(); err) {
            return 1;
        }
        printf("...polling after\n");
        using namespace std::chrono_literals;
        //std::this_thread::sleep_for(1s);
        std::cin.get();
    }

    Watcher::closeModified();

    return 0;
}
