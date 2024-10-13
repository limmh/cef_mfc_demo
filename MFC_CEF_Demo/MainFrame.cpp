#include "MainFrame.h"
#include "resource.h"
#include "utils.h"
#include "include/cef_app.h"
#include <string.h>

const int s_offset = 20;
const int s_height = 20;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_COMMAND(IDC_BUTTON_BACK, CMainFrame::OnBtnBackClicked)
	ON_COMMAND(IDC_BUTTON_FORWARD, CMainFrame::OnBtnForwardClicked)
	ON_COMMAND(IDC_BUTTON_RELOAD, CMainFrame::OnBtnReloadClicked)
	ON_COMMAND(IDC_BUTTON_STOP, CMainFrame::OnBtnStopClicked)
	ON_COMMAND(ID_URL_LOAD_PAGE, CMainFrame::LoadPage)
	ON_COMMAND(IDC_BUTTON_ZOOM_IN, CMainFrame::OnBtnZoomInClicked)
	ON_COMMAND(IDC_BUTTON_ZOOM_OUT, CMainFrame::OnBtnZoomOutClicked)
	ON_COMMAND(IDC_BUTTON_ZOOM_RESET, CMainFrame::OnBtnZoomResetClicked)
	ON_COMMAND(IDC_BUTTON_SCREEN_MODE, CMainFrame::OnBtnScreenModeClicked)
	ON_COMMAND(ID_FIND, CMainFrame::Find)
	ON_COMMAND(ID_CANCEL_FIND, CMainFrame::CancelFind)
	ON_COMMAND(ID_DOWNLOAD_COMPLETE, CMainFrame::ShowDownloadCompleteMessage)
END_MESSAGE_MAP()

CMainFrame::CMainFrame() :
	m_originalStyle(0),
	m_zoomLevel(0.0),
	m_left(0),
	m_top(0),
	m_width(500),
	m_height(400),
	m_screenModeToggled(false),
	m_isFullScreen(false)
{
}

CMainFrame::~CMainFrame()
{
}

bool CMainFrame::init()
{
	m_browserHandler = new ClientHandler;
	CefMainArgs mainArgs(::GetModuleHandle(NULL));
	int rc = CefExecuteProcess(mainArgs, nullptr, NULL);
	if (rc >= 0)
		return false;

	CefSettings settings;
	settings.multi_threaded_message_loop = true;
	settings.no_sandbox = true;

	if (!CefInitialize(mainArgs, settings, nullptr, NULL)) {
		//AfxMessageBox(_T("Failed to initialize CEF."), MB_ICONERROR);
		return false;
	}

	CefRefPtr<CefCommandLine> commandLine = CefCommandLine::CreateCommandLine();
	commandLine->InitFromString(::GetCommandLineW());
	if (commandLine->HasSwitch(L"url"))
		m_url = commandLine->GetSwitchValue(L"url");

	this->Create(NULL, _T(""));
	this->ShowWindow(SW_SHOW);
	this->UpdateWindow();

	return true;
}

int CMainFrame::OnCreate(LPCREATESTRUCT createStruct)
{
	CFrameWnd::OnCreate(createStruct);

	RECT rect = {};
	CWnd *parent = static_cast<CWnd*>(this);
	parent->GetClientRect(&rect);

	m_font.CreateFont(17, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_btnBack.Create(_T("<"), WS_VISIBLE, CRect(0, 0, s_offset, s_height), parent, IDC_BUTTON_BACK);
	m_btnForward.Create(_T(">"), WS_VISIBLE, CRect(s_offset, 0, 2*s_offset, s_height), parent, IDC_BUTTON_FORWARD);
	m_btnReload.Create(_T("R"), WS_VISIBLE, CRect(2*s_offset, 0, 3*s_offset, s_height), parent, IDC_BUTTON_RELOAD);
	m_btnStop.Create(_T("X"), WS_VISIBLE, CRect(3*s_offset, 0, 4*s_offset, s_height), parent, IDC_BUTTON_STOP);

	m_editFind.Create(ES_AUTOHSCROLL | WS_VISIBLE, CRect(rect.right - 8*s_offset, 0, rect.right, s_height), parent, IDC_EDIT_FIND);
	m_btnScreenMode.Create(_T("Mode"), WS_VISIBLE, CRect(rect.right - 10*s_offset, 0, rect.right - 8*s_offset, s_height), parent, IDC_BUTTON_SCREEN_MODE);
	m_btnZoomReset.Create(_T("100%"), WS_VISIBLE, CRect(rect.right - 13*s_offset, 0, rect.right - 10*s_offset, s_height), parent, IDC_BUTTON_ZOOM_RESET);
	m_btnZoomOut.Create(_T("-"), WS_VISIBLE, CRect(rect.right - 14*s_offset, 0, rect.right - 13*s_offset, s_height), parent, IDC_BUTTON_ZOOM_OUT);
	m_btnZoomIn.Create(_T("+"), WS_VISIBLE, CRect(rect.right - 15*s_offset, 0, rect.right - 14*s_offset, s_height), parent, IDC_BUTTON_ZOOM_IN);
	m_editUrl.Create(ES_AUTOHSCROLL | WS_VISIBLE, CRect(4*s_offset, 0, rect.right - 15*s_offset, s_height), parent, IDC_EDIT_URL);

	m_btnBack.SetFont(&m_font);
	m_btnForward.SetFont(&m_font);
	m_btnReload.SetFont(&m_font);
	m_btnStop.SetFont(&m_font);
	m_editFind.SetFont(&m_font);
	m_btnScreenMode.SetFont(&m_font);
	m_btnZoomReset.SetFont(&m_font);
	m_btnZoomOut.SetFont(&m_font);
	m_btnZoomIn.SetFont(&m_font);
	m_editUrl.SetFont(&m_font);

	if (m_tooltipCtrl.Create(static_cast<CWnd*>(this))) {
		m_tooltipCtrl.AddTool(&m_btnBack, _T("Go back"));
		m_tooltipCtrl.AddTool(&m_btnForward, _T("Go forward"));
		m_tooltipCtrl.AddTool(&m_btnReload, _T("Reload"));
		m_tooltipCtrl.AddTool(&m_btnStop, _T("Stop"));
		m_tooltipCtrl.AddTool(&m_editUrl, _T("Enter a URL and press Enter to load a page."));
		m_tooltipCtrl.AddTool(&m_btnZoomIn, _T("Zoom in"));
		m_tooltipCtrl.AddTool(&m_btnZoomOut, _T("Zoom out"));
		m_tooltipCtrl.AddTool(&m_btnZoomReset, _T("Reset to default zoom level."));
		m_tooltipCtrl.AddTool(&m_btnScreenMode, _T("Change screen mode"));
		m_tooltipCtrl.AddTool(&m_editFind, _T("Find a keyword or a phrase on this page. Press Enter to find and press Esc to cancel."));
		m_tooltipCtrl.Activate(TRUE);
	}

	HWND parentWnd = parent->GetSafeHwnd();
	m_originalStyle = ::GetWindowLongPtr(parentWnd, GWL_STYLE);

	CefWindowInfo windowInfo;
	CefBrowserSettings settings;
	CefRect cefRect(rect.left, rect.top + s_height, (rect.right - rect.left), (rect.bottom - rect.top - s_height));
	windowInfo.SetAsChild(parentWnd, cefRect);
	m_browserHandler->SetMainHwnd(parentWnd);

	if (m_url.empty())
		m_url = L"www.google.com";

	CefBrowserHost::CreateBrowser(windowInfo, m_browserHandler.get(), m_url, settings, nullptr, nullptr);
	return 0;
}

void CMainFrame::OnClose()
{
	if (m_browserHandler.get() && !m_browserHandler->IsClosing()) {
		CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
		if (browser.get()) {
			// Notify the browser window that we would like to close it. This will result in a call to ClientHandler::DoClose() if the JavaScript 'onbeforeunload' event handler allows it.
			browser->GetHost()->CloseBrowser(false);
			return; // Cancel the close.
		}
	}

	m_browserHandler = nullptr;
	CFrameWnd::OnClose();
	CefShutdown();
}

void CMainFrame::OnSize(UINT type, int width, int height)
{
	int posX = 0;
	int newEditUrlWidth = width - 4*s_offset - 15*s_offset;
	if (newEditUrlWidth > 0) 
		m_editUrl.SetWindowPos(NULL, 4*s_offset, 0, newEditUrlWidth, s_height, 0);

	posX = width - 8*s_offset;
	if (posX > 0)
		m_editFind.SetWindowPos(NULL, posX, 0, 8*s_offset, s_height, 0);
	posX = width - 10*s_offset;
	if (posX > 0)
		m_btnScreenMode.SetWindowPos(NULL, posX, 0, 2*s_offset, s_height, 0);
	posX = width - 13*s_offset;
	if (posX > 0)
		m_btnZoomReset.SetWindowPos(NULL, posX, 0, 3*s_offset, s_height, 0);
	posX = width - 14*s_offset;
	if (posX > 0)
		m_btnZoomOut.SetWindowPos(NULL, posX, 0, s_offset, s_height, 0);
	posX = width - 15*s_offset;
	if (posX > 0)
		m_btnZoomIn.SetWindowPos(NULL, posX, 0, s_offset, s_height, 0);

	if (!m_screenModeToggled) {
		m_width = width;
		m_height = height;
	}

	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get()) {
		int pageHeight = height - s_height;
		if (pageHeight > 0) {
			HWND browserWnd = browser->GetHost()->GetWindowHandle();
			::SetWindowPos(browserWnd, NULL, 0, s_height, width, pageHeight, SWP_NOZORDER);
		}
	}

	CFrameWnd::OnSize(type, width, height);
}

void CMainFrame::OnMove(int x, int y)
{
	if (!m_screenModeToggled) {
		m_left = x;
		m_top = y;
	}

	CFrameWnd::OnMove(x, y);
}

void CMainFrame::OnBtnBackClicked()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get())
		browser->GoBack();
}

void CMainFrame::OnBtnForwardClicked()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get())
		browser->GoForward();
}

void CMainFrame::OnBtnReloadClicked()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get())
		browser->Reload();
}

void CMainFrame::OnBtnStopClicked()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get())
		browser->StopLoad();
}

void CMainFrame::LoadPage()
{
	std::wstring url = utils::GetWindowText(m_editUrl.GetSafeHwnd());
	if (!url.empty()) {
		CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
		browser->GetMainFrame()->LoadURL(url);
	}
}

void CMainFrame::OnBtnZoomInClicked()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get()) {
		CefRefPtr<CefBrowserHost> browserHost = browser->GetHost();
		if (browserHost.get()) {
			m_zoomLevel += 1.0;
			browserHost->SetZoomLevel(m_zoomLevel);
		}
	}
}

void CMainFrame::OnBtnZoomOutClicked()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get()) {
		CefRefPtr<CefBrowserHost> browserHost = browser->GetHost();
		if (browserHost.get()) {
			m_zoomLevel -= 1.0;
			browserHost->SetZoomLevel(m_zoomLevel);
		}
	}
}

void CMainFrame::OnBtnZoomResetClicked()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get()) {
		CefRefPtr<CefBrowserHost> browserHost = browser->GetHost();
		if (browserHost.get()) {
			m_zoomLevel = 0.0;
			browserHost->SetZoomLevel(m_zoomLevel);
		}
	}
}

void CMainFrame::Find()
{
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get()) {
		CefRefPtr<CefBrowserHost> browserHost = browser->GetHost();
		if (browserHost.get()) {
			std::wstring text = utils::GetWindowText(m_editFind.GetSafeHwnd());
			if (!text.empty()) {
				const bool findNext = (0 == wcsicmp(text.c_str(), m_keyword.c_str()));
				browserHost->Find(text, true, false, findNext);
				if (!findNext)
					m_keyword = text;
			}
		}
	}
}

void CMainFrame::CancelFind()
{
	m_editFind.SetWindowText(_T(""));
	CefRefPtr<CefBrowser> browser = m_browserHandler->GetBrowser();
	if (browser.get()) {
		CefRefPtr<CefBrowserHost> browserHost = browser->GetHost();
		if (browserHost.get())
			browserHost->StopFinding(true);
	}
}

void CMainFrame::ShowDownloadCompleteMessage()
{
	AfxMessageBox(_T("The download is complete."), MB_ICONINFORMATION);
}

BOOL CMainFrame::PreTranslateMessage(MSG *pMsg)
{
	m_tooltipCtrl.RelayEvent(pMsg);
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnBtnScreenModeClicked()
{
	HWND wnd = GetSafeHwnd();
	m_screenModeToggled = true;

	if (m_isFullScreen) {
		m_isFullScreen = false;
		::SetWindowLongPtr(wnd, GWL_STYLE, m_originalStyle);
		SetWindowPos(NULL, m_left, m_top, m_width, m_height, 0);
		ShowWindow(SW_SHOW);
	} else {
		if (utils::SetFullScreen(wnd))
			m_isFullScreen = true;
	}

	m_screenModeToggled = false;
}

// CUrlEdit
BOOL CMainFrame::CUrlEdit::PreTranslateMessage(MSG *pMsg)
{
	switch (pMsg->message) {
	case WM_KEYDOWN:
		if (VK_RETURN == pMsg->wParam) {
			CWnd *main = AfxGetMainWnd();
			if (main != nullptr)
				::PostMessage(main->GetSafeHwnd(), WM_COMMAND, ID_URL_LOAD_PAGE, 0);
			return TRUE;
		}
		break;
	default:
		break;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

// CMyFindEdit
BOOL CMainFrame::CMyFindEdit::PreTranslateMessage(MSG *pMsg)
{
	switch (pMsg->message) {
	case WM_KEYDOWN:
		if (VK_RETURN == pMsg->wParam) {
			CWnd *main = AfxGetMainWnd();
			if (main != nullptr)
				::PostMessage(main->GetSafeHwnd(), WM_COMMAND, ID_FIND, 0);
			return TRUE;
		} else if (VK_ESCAPE == pMsg->wParam) {
			CWnd *main = AfxGetMainWnd();
			if (main != nullptr)
				::PostMessage(main->GetSafeHwnd(), WM_COMMAND, ID_CANCEL_FIND, 0);
			return TRUE;
		}
		break;
	default:
		break;
	}

	return CEdit::PreTranslateMessage(pMsg);
}
