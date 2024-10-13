#ifndef MFC_CEF_DEMO_HANDLER_H
#define MFC_CEF_DEMO_HANDLER_H

#include <string>
#include <list>
#include "include/cef_client.h"
#include "include/base/cef_lock.h"

// ClientHandler derives from CefClient and other classes.
// The CefClient interface provides access to browser-instance-specific callbacks.
// A single CefClient instance can be shared among any number of browsers.
class ClientHandler :	
	public CefClient,
	public CefDisplayHandler,
	public CefDownloadHandler,
	public CefLifeSpanHandler
{
public:
	ClientHandler();
	virtual ~ClientHandler();

	// CefClient methods:
	// important to return |this| for the handler callbacks.

	virtual	CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
		return this;
	}

	virtual	CefRefPtr<CefDownloadHandler> GetDownloadHandler() override {
		return this;
	}
	
	virtual	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
		return this;
	}

	// CefDisplayHandler methods
	virtual	void OnAddressChange(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame,
		const CefString& url) override;
	virtual	void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;

	// CefDownloadHandler methods
	virtual	bool OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
		const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) override;
	virtual	void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
		CefRefPtr<CefDownloadItemCallback> callback) override;

	// CefLifeSpanHandler methods
	virtual	void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	virtual	bool DoClose(CefRefPtr<CefBrowser> browser) override;
	virtual	void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	// Self-defined methods
	CefRefPtr<CefBrowser> GetBrowser() {
		return m_Browser;
	}
	
	bool IsClosing() {
		base::cef_internal::AutoLock lock(m_lock);
		return m_bIsClosing;
	}
	
	CefWindowHandle	GetMainHwnd() {
		return m_MainHwnd;
	}

	void SetMainHwnd(CefWindowHandle hwnd);

private:
	base::cef_internal::Lock m_lock;
	void CloseAllPopups(bool force_close);

private:
	// The child browser window
	CefRefPtr<CefBrowser> m_Browser;
	
	// List of popup browser windows. Only accessed on the CEF UI thread.
	std::list<CefRefPtr<CefBrowser> > m_PopupList;

	// The child browser id
	int m_BrowserId;
	
	// The main frame window handle
	CefWindowHandle m_MainHwnd;

	// True if the main browser window is currently closing.
	bool m_bIsClosing;

IMPLEMENT_REFCOUNTING(ClientHandler);
};

#endif
