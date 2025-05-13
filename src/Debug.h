#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>

enum class LogLevel 
{
    Log,
    Warning,
    Error
};

class Debug
{
public:
    static void Log(const std::string& message);
    static void LogWarning(const std::string& message);
    static void LogError(const std::string& message);

private:
    static void Print(const std::string& message, LogLevel level);
};