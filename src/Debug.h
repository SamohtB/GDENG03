#pragma once
#define DEBUG_BREAK() __debugbreak()
#include <iostream>

class DebugWindow;

class Debug
{
public:
    static Debug* GetInstance();
    static void Initialize();
    static void Destroy();

    static void ThrowIfFailed(HRESULT hr, const String& message = "");
    static void Log(const String& message);
    static void LogWarning(const std::string& message);
    static void LogError(const std::string& message);
    static void LogException(const std::exception& e);
    static void Break();
    static void Assert(bool condition, const std::string& message = "Assertion failed");

    class HrException : public std::runtime_error
    {
    public:
        explicit HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}

        HRESULT Error() const { return m_hr; }

    private:
        HRESULT m_hr;
    };

	void AssignWindow(DebugWindow* window);

    Debug() = default;
    ~Debug() = default;
    Debug(Debug const&) = delete;
    Debug& operator=(Debug const&) = delete;

private:
    static void SetColor(int color);
    static void ResetColor();
    static std::string HrToString(HRESULT hr);

	static std::unique_ptr<Debug> sharedInstance;
	DebugWindow* m_window;
};

