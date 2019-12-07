#include "stdafx.h"

// General
#include "ShadersUtils.h"

std::string RecursionInclude(IBaseManager* BaseManager, std::shared_ptr<IFile> f)
{
    if (f == nullptr)
    {
        Log::Error("Error open shader.");
        return "";
    }

    std::string data = "";

    while (!f->isEof())
    {
        std::string line;
        if (false == f->readLine(&line))
        {
            break;
        }

        // Skip empty lines
        if (line.length() == 0)
        {
            continue;
        }

        // Find directive
        if (line[0] == '#' && line[1] == 'i' && line[2] == 'n' && line[3] == 'c' && line[4] == 'l')
        {
            size_t firstBracketPosition = line.find('"');
            _ASSERT(firstBracketPosition != std::string::npos);

            size_t lastBracketPosition = line.find_last_of('"');
            _ASSERT(firstBracketPosition != lastBracketPosition);

            std::string inludeFileName = line.substr(firstBracketPosition + 1, lastBracketPosition - firstBracketPosition - 1);
            CFile::FixFilePath(inludeFileName);

            std::shared_ptr<IFile> includeFile = GetManager<IFilesManager>(BaseManager)->Open(inludeFileName);

            data += RecursionInclude(BaseManager, includeFile) + '\n';

            continue;
        }

        data += line + '\n';
    }

    return data;
}