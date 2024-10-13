#include "utils.h"
#include <stdlib.h>

namespace utils {

bool SetFullScreen(HWND hWnd)
{
	// Obtain the monitor information
	HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = {};
	mi.cbSize = sizeof(mi);
	if (!GetMonitorInfo(hMonitor, &mi))
		return false; 

	// Change the window to a pop up window (no title bar)
	SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	// Set the window position
	const BOOL status = 
				SetWindowPos(hWnd,
				HWND_TOP,
				mi.rcMonitor.left,
				mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_SHOWWINDOW);

	return (status != FALSE);
}

std::wstring GetWindowText(HWND hWnd)
{
	std::wstring text;
	int len = GetWindowTextLengthW(hWnd);
	if (len) {
		wchar_t *p = (wchar_t*) malloc((sizeof *p) * (len + 1));
		if (p) {
			GetWindowTextW(hWnd, p, len + 1);
			p[len] = L'\0';
			text = p;
			free(p);
		}
	}
	return text;
}

void NotifyWindowToClose(HWND hWnd)
{
	::PostMessage(hWnd, WM_CLOSE, 0, 0);
}

} // namespace
