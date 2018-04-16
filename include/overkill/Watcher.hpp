#pragma once
#include <string>
#include <vector>

#include <tinyheaders/tinyfiles.h>
#include <PMS/logger.h>

namespace overkill
{

struct ModifiedFile
{
    using Callback = void(*)(const ModifiedFile& mfile);

    std::string tag;
    std::string path;
    Callback    callback;
    tfFILETIME  modTime;
};

class Watcher
{
    static std::vector<ModifiedFile> m_modifiedFiles;
public:

    static void watchFile(const std::string tag, const std::string path, ModifiedFile::Callback callback);
    static void scanFiles();
};


}
