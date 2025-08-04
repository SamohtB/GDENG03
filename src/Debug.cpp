#include "pch.h"
#include "Debug.h"
#include "DebugWindow.h"

std::unique_ptr<Debug> Debug::sharedInstance = nullptr;

Debug* Debug::GetInstance()
{
    return sharedInstance.get();
}

void Debug::Initialize()
{
    try
    {
        sharedInstance = std::make_unique<Debug>();
    }
    catch (...)
    {
        Debug::Log("Debug Logger Creation Failed!");
    }
}

void Debug::Destroy()
{
    sharedInstance.reset();
    sharedInstance = nullptr;
}

void Debug::AssignWindow(DebugWindow* window)
{
	this->m_window = window;
}

void Debug::ThrowIfFailed(HRESULT hr, const String& message)
{
    if (FAILED(hr))
    {
        if (!message.empty())
        {
            LogError(message);
        }

        LogError(std::to_string(hr));

        throw HrException(hr);

    }
}

void Debug::Log(const String& message)
{
    SetColor(7);
    std::cout << "[Log] " << message << std::endl;
    ResetColor();

    if (sharedInstance->m_window != nullptr)
    {
        sharedInstance->m_window->LogMessage("[Log] " + message);
    }
}

void Debug::LogWarning(const std::string& message)
{
    SetColor(14);
    std::cout << "[Warning] " << message << std::endl;
    ResetColor();

    if (sharedInstance->m_window != nullptr)
    {
        sharedInstance->m_window->LogMessage("[Warning] " + message);
    }
}

void Debug::LogError(const std::string& message)
{
    SetColor(12);
    std::cerr << "[Error] " << message << std::endl;
    ResetColor();

    if (sharedInstance->m_window != nullptr)
    {
        sharedInstance->m_window->LogMessage("[Error] " + message);
    }
}

void Debug::LogException(const std::exception& e)
{
    SetColor(12);
    std::cerr << "[Exception] " << e.what() << std::endl;
    ResetColor();

    if (sharedInstance->m_window)
    {
        sharedInstance->m_window->LogMessage("[Exception] " + std::string(e.what()));
    }
}

void Debug::Break()
{
    DEBUG_BREAK();
}

void Debug::Assert(bool condition, const std::string& message)
{
    if (!condition)
    {
        LogError(message);
        Break();
    }
}

void Debug::SetColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Debug::ResetColor()
{
    SetColor(7);
}

std::string Debug::HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    return std::string(s_str);
}
