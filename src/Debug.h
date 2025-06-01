#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cassert>
#include <windows.h>
#include <comdef.h> // For _com_error
#define DEBUG_BREAK() __debugbreak()

class Debug
{
public:
    class HrException : public std::runtime_error
    {
    public:
        explicit HrException(HRESULT hr)
            : std::runtime_error(HrToString(hr)), m_hr(hr) {}

        HRESULT Error() const { return m_hr; }

    private:
        HRESULT m_hr;
    };

    static void ThrowIfFailed(HRESULT hr, const std::string& message = "")
    {
        if (FAILED(hr))
        {
            if (!message.empty()) 
            {
                LogError(message);
            }
            LogError(HrToString(hr));

            throw HrException(hr);
            Break();
        }
    }

    static void Log(const std::string& message) 
    {
        SetColor(7);
        std::cout << "[Log] " << message << std::endl;
        ResetColor();
    }

    static void LogWarning(const std::string& message) 
    {
        SetColor(14);
        std::cout << "[Warning] " << message << std::endl;
        ResetColor();
    }

    static void LogError(const std::string& message) 
    {
        SetColor(12);
        std::cerr << "[Error] " << message << std::endl;
        ResetColor();
        Break();
    }

    static void LogException(const std::exception& e) 
    {
        SetColor(12);
        std::cerr << "[Exception] " << e.what() << std::endl;
        ResetColor();
    }

    static void Break() 
    {
        DEBUG_BREAK();
    }

    static void Assert(bool condition, const std::string& message = "Assertion failed") 
    {
        if (!condition) 
        {
            LogError(message);
            Break();
        }
    }

private:
    static void SetColor(int color) 
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    static void ResetColor() 
    {
        SetColor(7);
    }

    static std::string HrToString(HRESULT hr)
    {
        char s_str[64] = {};
        sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
        return std::string(s_str);
    }
};

