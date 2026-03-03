#include "keyboardhook.h"

#include <QDebug>

void (*KeyboardHook::s_callback)() = nullptr;

KeyboardHook::KeyboardHook()
    : m_hook(nullptr)
{
}

KeyboardHook::~KeyboardHook()
{
    uninstall();
}

void KeyboardHook::install()
{
    if (m_hook) return;
    m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, GetModuleHandle(nullptr), 0);
    qDebug() << "Keyboard hook installed";
}

void KeyboardHook::uninstall()
{
    if (m_hook) {
        UnhookWindowsHookEx(m_hook);
        m_hook = nullptr;
        qDebug() << "Keyboard hook uninstalled";
    }
}

void KeyboardHook::setCallback(void (*callback)())
{
    s_callback = callback;
}

LRESULT CALLBACK KeyboardHook::keyboardHookProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION && s_callback) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            s_callback();
        }
    }
    return CallNextHookEx(nullptr, code, wParam, lParam);
}
