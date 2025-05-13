#include "Debug.h"

#ifdef _WIN32
#include <windows.h>
#endif

void Debug::Print(const std::string& message, LogLevel level) 
{
    std::string levelTag;
    std::ostream* outStream = &std::cout;

    switch (level) 
    {
    case LogLevel::Log:
        levelTag = "[Log]   ";
        break;

    case LogLevel::Warning:
        levelTag = "[Warning]   ";
        break;

    case LogLevel::Error:
        levelTag = "[Error] ";
        outStream = &std::cerr;
        break;
    }

    *outStream << levelTag << message << std::endl;
}

void Debug::Log(const std::string& message) 
{
    Print(message, LogLevel::Log);
}

void Debug::LogWarning(const std::string& message) {
    Print(message, LogLevel::Warning);
}

void Debug::LogError(const std::string& message) {
    Print(message, LogLevel::Error);
}
