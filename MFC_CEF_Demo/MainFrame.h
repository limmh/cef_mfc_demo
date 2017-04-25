#ifndef MFC_CEF_DEMO_MAIN_FRAME_H
#define MFC_CEF_DEMO_MAIN_FRAME_H

#include <afxwin.h>
#include <afxcmn.h>
#include "handler.h"
#include <string>

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
	~CMainFrame();
	bool init();

DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT createStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT type, int x, int y);
	afx_msg void OnMove(int x, int y);

	virtual BOOL PreTranslateMessage(MSG *pMsg) override;

private:
	void OnBtnBackClicked();
	void OnBtnForwardClicked();
	void OnBtnReloadClicked();
	void OnBtnStopClicked();
	void OnBtnZoomInClicked();
	void OnBtnZoomOutClicked();
	void OnBtnZoomResetClicked();
	void OnBtnScreenModeClicked();

	void LoadPage();
	void Find();
	void CancelFind();
	void ShowDownloadCompleteMessage();

private:
	class CUrlEdit : public CEdit {
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg) override;
	};

	class CMyFindEdit : public CEdit {
	public:
		virtual BOOL PreTranslateMessage(MSG *pMsg) override;
	};

private:
	CefRefPtr<ClientHandler> m_browserHandler;
	CFont m_font;
	CButton m_btnBack, m_btnForward, m_btnReload, m_btnStop;
	CButton m_btnZoomIn, m_btnZoomOut, m_btnZoomReset, m_btnScreenMode;
	CUrlEdit m_editUrl;
	CMyFindEdit m_editFind;
	CToolTipCtrl m_tooltipCtrl;
	std::wstring m_url, m_keyword;
	LONG_PTR m_originalStyle;
	double m_zoomLevel;
	int m_left, m_top, m_width, m_height;
	bool m_screenModeToggled, m_isFullScreen, m_cefIsInitialized;
};

#endif
