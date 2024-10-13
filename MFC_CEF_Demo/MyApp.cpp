#include "MyApp.h"
#include "MainFrame.h"

BOOL CMyApp::InitInstance()
{
	CMainFrame *frame = new CMainFrame;
	if (false == frame->init())
		return FALSE;
	m_pMainWnd = static_cast<CWnd*>(frame);
	CWinApp::InitInstance();
	return TRUE;
}
