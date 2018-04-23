#include <iostream>
#include <cstdio>
#include <cstdlib>  
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <set>
#include <iomanip>

struct FileDiscovery 
{
    std::string tag;
    std::string extension;
};

struct FileDiscoverySets
{
    std::set<FileDiscovery> textures;
    std::set<FileDiscovery> shaders;
    std::set<FileDiscovery> materials;
    std::set<FileDiscovery> models;
    std::set<FileDiscovery> scenes;
} fileTags;


#ifdef WIN32
    const auto& popen = _popen;
    const auto& pclose = _pclose;
    constexpr char PYTHON_COMMAND[] = "C:\\Python36\\python.exe discover.py";
#else
    constexpr char PYTHON_COMMAND[] = "/usr/bin/python3 discover.py";
#endif

int main()
{
    using namespace std::chrono_literals;
    FILE* pipe = popen(PYTHON_COMMAND, "r");
    const int DATA_SIZE =  128;
    char  data[DATA_SIZE] = "\n";
     
    if (!pipe) {
        printf("FAILED OPENING the pipe\n");
        return 1;
    }

    printf("PUTS\n");
    int status = fputs("textures/*", pipe);

    printf("GETS\n");
    char* msg = fgets(data, DATA_SIZE, pipe);
    
    if(!msg)
        return 1;

    printf("gets %s\n", msg);

    printf("SLEEP\n");
    std::this_thread::sleep_for(.01s);
 
    printf("PUTS EXIT\n");
    status = fputs("EXIT", pipe);

    printf("CLOSE\n");
    int status = pclose(pipe);
    
    printf("really closing\n");
    if (closestatus) {
        printf("FAILED CLOSING the pipe\n");
        return 1;
    }
    printf("CLOSING the pipe\n");

    return 0;
}