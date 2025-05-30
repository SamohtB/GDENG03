#pragma once

#include <stdexcept>
#include <string>
#include <wrl.h>
#include <comdef.h>
#include <sstream>
#include <iomanip>

// Utility: Convert HRESULT to readable message
inline std::string HrToMessage(HRESULT hr)
{
    _com_error err(hr);
    std::ostringstream oss;
    oss << "HRESULT 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8)
        << static_cast<unsigned int>(hr)
        << ": " << err.ErrorMessage();
    return oss.str();
}

// Exception class with context
class DxException : public std::runtime_error
{
public:
    DxException(HRESULT hr, const char* file, int line, const char* function, const char* message = "")
        : std::runtime_error(BuildMessage(hr, file, line, function, message)), m_hr(hr) {}

    HRESULT Error() const { return m_hr; }

private:
    const HRESULT m_hr;

    static std::string BuildMessage(HRESULT hr, const char* file, int line, const char* function, const char* message)
    {
        std::ostringstream oss;
        oss << "[DX12 ERROR] in " << function << " at " << file << ":" << line << "\n"
            << HrToMessage(hr);
        if (message && *message)
            oss << "\nAdditional Info: " << message;
        return oss.str();
    }
};

// Macro for easier usage
#define THROW_IF_FAILED(hr, ...)                                     \
    {                                                                \
        HRESULT _hr_ = (hr);                                         \
        if (FAILED(_hr_))                                            \
            throw DxException(_hr_, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
    }

// Optional release safety macro
#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = nullptr; }

