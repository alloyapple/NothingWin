#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H

#include <windows.h>

class KeyboardHook
{
public:
    KeyboardHook();
    ~KeyboardHook();

    void install();
    void uninstall();

    static void setCallback(void (*callback)());

private:
    static LRESULT CALLBACK keyboardHookProc(int code, WPARAM wParam, LPARAM lParam);
    static void (*s_callback)();
    HHOOK m_hook;
};

#endif
