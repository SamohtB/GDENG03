#include "pch.h"
#include "WindowsInputAdapter.h"
#include "HotkeySystem.hpp"
#include "InputEvent.h"
#include "KeyCodes.hpp"

WindowsInputAdapter::WindowsInputAdapter(HWND hwnd) : m_hwnd(hwnd)
{
    RAWINPUTDEVICE rid[2];

    // Keyboard
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x06;
    rid[0].dwFlags = RIDEV_INPUTSINK;
    rid[0].hwndTarget = hwnd;

    // Mouse
    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x02;
    rid[1].dwFlags = RIDEV_INPUTSINK;
    rid[1].hwndTarget = hwnd;

    RegisterRawInputDevices(rid, 2, sizeof(rid[0]));
}

void WindowsInputAdapter::processMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INPUT)
        handleRawInput((HRAWINPUT)lParam);
}

void WindowsInputAdapter::handleRawInput(HRAWINPUT hRawInput)
{
    UINT dwSize = 0;
    GetRawInputData(hRawInput, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

    std::vector<BYTE> lpb(dwSize);
    if (GetRawInputData(hRawInput, RID_INPUT, lpb.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
        return;

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb.data());

    InputEvent e{};
    e.modifiers = Modifiers::None;

    // ----- Set Modifiers -----
    if (GetKeyState(VK_SHIFT) & 0x8000) e.modifiers |= Modifiers::Shift;
    if (GetKeyState(VK_CONTROL) & 0x8000) e.modifiers |= Modifiers::Ctrl;
    if (GetKeyState(VK_MENU) & 0x8000) e.modifiers |= Modifiers::Alt;
    if (GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000) e.modifiers |= Modifiers::Super;

    // ----- Keyboard -----
    if (raw->header.dwType == RIM_TYPEKEYBOARD)
    {
        RAWKEYBOARD& kb = raw->data.keyboard;
        e.device = InputDeviceType::Keyboard;

        e.key = mapVirtualKeyToKeyCode(kb.VKey);

        e.action = (kb.Flags & RI_KEY_BREAK) ? InputAction::Release : InputAction::Press;
    }

    // ----- Mouse -----
    else if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        RAWMOUSE& mouse = raw->data.mouse;
        e.device = InputDeviceType::Mouse;

        if (mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) { e.key = KeyCode::LeftMouse; e.action = InputAction::Press; }
        else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP) { e.key = KeyCode::LeftMouse; e.action = InputAction::Release; }
        else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) { e.key = KeyCode::RightMouse; e.action = InputAction::Press; }
        else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP) { e.key = KeyCode::RightMouse; e.action = InputAction::Release; }
        else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN) { e.key = KeyCode::MiddleMouse; e.action = InputAction::Press; }
        else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP) { e.key = KeyCode::MiddleMouse; e.action = InputAction::Release; }
        else { e.key = KeyCode::None; }
    }
    else
        return;

    HotkeySystem::getInstance()->processInputEvent(e);
}

KeyCode WindowsInputAdapter::mapVirtualKeyToKeyCode(WPARAM vk)
{
    switch (vk)
    {
    // Letters
    case 'A': return KeyCode::A;
    case 'B': return KeyCode::B;
    case 'C': return KeyCode::C;
    case 'D': return KeyCode::D;
    case 'E': return KeyCode::E;
    case 'F': return KeyCode::F;
    case 'G': return KeyCode::G;
    case 'H': return KeyCode::H;
    case 'I': return KeyCode::I;
    case 'J': return KeyCode::J;
    case 'K': return KeyCode::K;
    case 'L': return KeyCode::L;
    case 'M': return KeyCode::M;
    case 'N': return KeyCode::N;
    case 'O': return KeyCode::O;
    case 'P': return KeyCode::P;
    case 'Q': return KeyCode::Q;
    case 'R': return KeyCode::R;
    case 'S': return KeyCode::S;
    case 'T': return KeyCode::T;
    case 'U': return KeyCode::U;
    case 'V': return KeyCode::V;
    case 'W': return KeyCode::W;
    case 'X': return KeyCode::X;
    case 'Y': return KeyCode::Y;
    case 'Z': return KeyCode::Z;

    // Numbers (top row)
    case '0': return KeyCode::Num0;
    case '1': return KeyCode::Num1;
    case '2': return KeyCode::Num2;
    case '3': return KeyCode::Num3;
    case '4': return KeyCode::Num4;
    case '5': return KeyCode::Num5;
    case '6': return KeyCode::Num6;
    case '7': return KeyCode::Num7;
    case '8': return KeyCode::Num8;
    case '9': return KeyCode::Num9;

    // Arrow Keys
    case VK_UP: return KeyCode::Up;
    case VK_DOWN: return KeyCode::Down;
    case VK_LEFT: return KeyCode::Left;
    case VK_RIGHT: return KeyCode::Right;

    // Function Keys
    case VK_F1: return KeyCode::F1;
    case VK_F2: return KeyCode::F2;
    case VK_F3: return KeyCode::F3;
    case VK_F4: return KeyCode::F4;
    case VK_F5: return KeyCode::F5;
    case VK_F6: return KeyCode::F6;
    case VK_F7: return KeyCode::F7;
    case VK_F8: return KeyCode::F8;
    case VK_F9: return KeyCode::F9;
    case VK_F10: return KeyCode::F10;
    case VK_F11: return KeyCode::F11;
    case VK_F12: return KeyCode::F12;

    // Control Keys
    case VK_ESCAPE: return KeyCode::Escape;
    case VK_TAB: return KeyCode::Tab;
    case VK_CAPITAL: return KeyCode::CapsLock;
    case VK_SHIFT: return KeyCode::Shift;
    case VK_CONTROL: return KeyCode::Control;
    case VK_MENU: return KeyCode::Alt;
    case VK_SPACE: return KeyCode::Space;
    case VK_RETURN: return KeyCode::Enter;
    case VK_BACK: return KeyCode::Backspace;
    case VK_INSERT: return KeyCode::Insert;
    case VK_DELETE: return KeyCode::Delete;
    case VK_HOME: return KeyCode::Home;
    case VK_END: return KeyCode::End;
    case VK_PRIOR: return KeyCode::PageUp;    // Page Up
    case VK_NEXT: return KeyCode::PageDown;   // Page Down

    // Punctuation / Symbols
    case VK_OEM_MINUS: return KeyCode::Minus;            // - key
    case VK_OEM_PLUS: return KeyCode::Equal;             // = key
    case VK_OEM_4: return KeyCode::LeftBracket;          // [ key
    case VK_OEM_6: return KeyCode::RightBracket;         // ] key
    case VK_OEM_5: return KeyCode::Backslash;           // \ key
    case VK_OEM_1: return KeyCode::Semicolon;           // ; key
    case VK_OEM_7: return KeyCode::Apostrophe;          // ' key
    case VK_OEM_COMMA: return KeyCode::Comma;           // , key
    case VK_OEM_PERIOD: return KeyCode::Period;         // . key
    case VK_OEM_2: return KeyCode::Slash;               // / key
    case VK_OEM_3: return KeyCode::Grave;               // ` key

    // Numpad Keys
    case VK_NUMPAD0: return KeyCode::Numpad0;
    case VK_NUMPAD1: return KeyCode::Numpad1;
    case VK_NUMPAD2: return KeyCode::Numpad2;
    case VK_NUMPAD3: return KeyCode::Numpad3;
    case VK_NUMPAD4: return KeyCode::Numpad4;
    case VK_NUMPAD5: return KeyCode::Numpad5;
    case VK_NUMPAD6: return KeyCode::Numpad6;
    case VK_NUMPAD7: return KeyCode::Numpad7;
    case VK_NUMPAD8: return KeyCode::Numpad8;
    case VK_NUMPAD9: return KeyCode::Numpad9;
    case VK_DECIMAL: return KeyCode::NumpadDecimal;
    case VK_DIVIDE: return KeyCode::NumpadDivide;
    case VK_MULTIPLY: return KeyCode::NumpadMultiply;
    case VK_SUBTRACT: return KeyCode::NumpadSubtract;
    case VK_ADD: return KeyCode::NumpadAdd;

    // Special / Misc
    case VK_SNAPSHOT: return KeyCode::PrintScreen;
    case VK_SCROLL: return KeyCode::ScrollLock;
    case VK_PAUSE: return KeyCode::Pause;
    case VK_APPS: return KeyCode::Menu;

    default: return KeyCode::None;
    }
}

