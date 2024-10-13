#ifndef MFC_CEF_DEMO_UTILS_H
#define MFC_CEF_DEMO_UTILS_H

#include <string>
#include <Windows.h>

namespace utils {
bool SetFullScreen(HWND hWnd);
std::wstring GetWindowText(HWND hWnd);
void NotifyWindowToClose(HWND hWwnd);
}

#endif
