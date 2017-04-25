#include "MyApp.h"
#include "MainFrame.h"

BOOL CMyApp::InitInstance()
{
	CMainFrame *mf = new CMainFrame;
	if (false == mf->init())
		return FALSE;
	m_pMainWnd = dynamic_cast<CWnd*>(mf);
	CWinApp::InitInstance();
	return TRUE;
}
