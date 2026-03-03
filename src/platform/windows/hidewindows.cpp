#include "hidewindows.h"

#include <windows.h>
#include <winuser.h>
#include <string.h>
#include <QDebug>

static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    if (!IsWindowVisible(hwnd)) return TRUE;

    HWND ownWnd = reinterpret_cast<HWND>(lParam);
    if (hwnd == ownWnd) return TRUE;

    WCHAR className[256];
    GetClassNameW(hwnd, className, 256);

    if (wcscmp(className, L"Shell_TrayWnd") == 0) return TRUE;
    if (wcscmp(className, L"Shell_SecondaryTrayWnd") == 0) return TRUE;
    if (wcscmp(className, L"WorkerW") == 0) return TRUE;
    if (wcscmp(className, L"Progman") == 0) return TRUE;
    if (wcscmp(className, L"DV2ControlHost") == 0) return TRUE;

    ShowWindow(hwnd, SW_MINIMIZE);

    return TRUE;
}

void hideAllWindows()
{
    HWND ownWnd = GetForegroundWindow();
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(ownWnd));
    qDebug() << "All windows minimized";
}

void showAllWindows()
{
    HWND ownWnd = GetForegroundWindow();
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(ownWnd));
}
