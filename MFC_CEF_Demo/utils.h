#ifndef MFC_CEF_DEMO_UTILS_H
#define MFC_CEF_DEMO_UTILS_H

#include <string>
#include <Windows.h>

namespace utils {
BOOL SetFullScreen(HWND hWnd);
void GetWindowText(HWND hwnd, std::wstring& text);
}

#endif
