#pragma once

#include <vector>
#include <msxml2.h>
#include <regex>

using std::wregex;
using std::regex_iterator;
const CString NET_FAILED_TO_CREATE_INSTANCE_TEXT = _T("failed to create instance");
const CString NET_STOP_TEXT = _T("stopped");
const CString NET_TIMEOUT_TEXT = _T("timeout");
enum HTTPRequestResult { NET_SUCCESS, NET_FAILED_TO_CREATE_INSTANCE, NET_STOP, NET_TIMEOUT, };

class HttpClient
{
public:
	HttpClient();
	virtual ~HttpClient();


	CString g_cookie;

	CString EncodeURI(CString url);
	CString HTMLEscape(const CString& src);
	CString HTMLUnescape(const CString& src);

	CString HTTPGet(LPCTSTR URL, BOOL useCookie , volatile BOOL* stopFlag , CString* cookie );
	CString HTTPPost(LPCTSTR URL, LPCTSTR data, BOOL useCookie , volatile BOOL* stopFlag , CString* cookie );
	void ReceiveCookie(LPCTSTR headers, CString& cookie);

	HTTPRequestResult HTTPGetRaw(LPCTSTR URL, BYTE** buffer, ULONG* size, BOOL useCookie, volatile BOOL* stopFlag, CString* cookie);
	HTTPRequestResult HTTPRequestBase(BOOL postMethod, CComPtr<IServerXMLHTTPRequest>& xml, LPCTSTR URL, LPCTSTR data, BOOL useCookie, volatile BOOL* stopFlag, CString* cookie);
	
	
	void Delay(DWORD time);
	void DoEvents();

};

