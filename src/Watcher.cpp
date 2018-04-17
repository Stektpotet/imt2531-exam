#pragma once
#include <overkill/Watcher.hpp>

#include <tinyheaders/tinyfiles.h>


namespace overkill
{

std::vector<ModifiedFile> Watcher::m_modifiedFiles;

void Watcher::watchFile(const std::string tag, const std::string path, ModifiedFile::Callback callback) 
{
    ModifiedFile mfile = ModifiedFile{
        tag,
        path,
        callback
    };
    Watcher::m_modifiedFiles.push_back(mfile);

    auto status = tfGetFileTime(mfile.path.c_str(), &(mfile.modTime));
}

void Watcher::scanFiles()
{
#ifdef WIN32

    LOG_DEBUG("Scanning files...");

    for (auto mfile : Watcher::m_modifiedFiles)
    {
        tfFILETIME newmodTime;

        auto status = tfGetFileTime(mfile.path.c_str(), &newmodTime);

        LOG_DEBUG("new %u vs old %u", newmodTime.time.dwLowDateTime, mfile.modTime.time.dwLowDateTime);

        if (newmodTime.time.dwLowDateTime != mfile.modTime.time.dwLowDateTime)
            mfile.callback(mfile);
        // @todo implement the same for Mac and Linux. Windows uses DWORD as time type. Unix systems uses time_t.
    }
#endif
}

}


