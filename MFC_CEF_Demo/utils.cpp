#include "utils.h"
#include <stdlib.h>

namespace utils {

BOOL SetFullScreen(HWND hWnd)
{
	// Obtain the monitor information
	HMONITOR hmon = MonitorFromWindow(NULL, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = {};
	mi.cbSize = sizeof(mi);
	if (!GetMonitorInfo(hmon, &mi))
		return FALSE; 

	// Change the window to a pop up window (no title bar)
	SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	// Set the window position
	SetWindowPos(hWnd,
				HWND_TOP,
				mi.rcMonitor.left,
				mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_SHOWWINDOW);

	return TRUE;
}

void GetWindowText(HWND hwnd, std::wstring& text) {
	text.clear();
	int len = GetWindowTextLengthW(hwnd);
	if (len) {
		wchar_t *p = (wchar_t*) malloc((sizeof *p) * (len + 1));
		if (p) {
			GetWindowTextW(hwnd, p, len + 1);
			p[len] = L'\0';
			text = p;
			free(p);
		}
	}
}

} // namespace
