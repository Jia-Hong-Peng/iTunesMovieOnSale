#include "stdafx.h"
#include "HttpClient.h"




HttpClient::HttpClient()
{
}


HttpClient::~HttpClient()
{
}





// URL編碼
CString HttpClient::EncodeURI(CString url)
{
	url.Replace(_T("%"), _T("%25"));
	url.Replace(_T("!"), _T("%21"));
	url.Replace(_T("*"), _T("%2A"));
	url.Replace(_T("'"), _T("%27"));
	url.Replace(_T("("), _T("%28"));
	url.Replace(_T(")"), _T("%29"));
	url.Replace(_T(";"), _T("%3B"));
	url.Replace(_T(":"), _T("%3A"));
	url.Replace(_T("@"), _T("%40"));
	url.Replace(_T("&"), _T("%26"));
	url.Replace(_T("="), _T("%3D"));
	url.Replace(_T("+"), _T("%2B"));
	url.Replace(_T("$"), _T("%24"));
	url.Replace(_T(","), _T("%2C"));
	url.Replace(_T("/"), _T("%2F"));
	url.Replace(_T("?"), _T("%3F"));
	url.Replace(_T("#"), _T("%23"));
	url.Replace(_T("["), _T("%5B"));
	url.Replace(_T("]"), _T("%5D"));
	url.Replace(_T(" "), _T("%20"));
	url.Replace(_T("\r\n"), _T("%2C"));
	url.Replace(_T("\n"), _T("%2C"));
	url.Replace(_T("\""), _T("%22"));

	return url;
}



// HTML轉義
CString HttpClient::HTMLEscape(const CString& src)
{
	CString result = src;
	result.Replace(_T("&"), _T("&amp;"));
	result.Replace(_T(" "), _T("&nbsp;"));
	result.Replace(_T("<"), _T("&lt;"));
	result.Replace(_T(">"), _T("&gt;"));
	return result;
}

// HTML反轉義
CString HttpClient::HTMLUnescape(const CString& src)
{
	CString result = src;
	//result.Replace(_T("<br>"), _T("\r\n")); // 不轉換行符
	result.Replace(_T("&nbsp;"), _T(" "));
	result.Replace(_T("&quot;"), _T("\""));
	result.Replace(_T("&&#039;"), _T("'"));
	result.Replace(_T("&lt;"), _T("<"));
	result.Replace(_T("&gt;"), _T(">"));
	result.Replace(_T("&amp;"), _T("&"));
	return result;
}



// 從HTTP頭提取Cookie並修改cookie
void HttpClient::ReceiveCookie(LPCTSTR headers, CString& cookie)
{
	static const wregex cookieExp(_T("Set-Cookie: (.*?)=(.*?);"));
	for (regex_iterator<LPCTSTR> it(headers, headers + _tcslen(headers), cookieExp), end; it != end; it++)
	{
		CString name = (*it)[1].str().c_str();
		CString value = (*it)[2].str().c_str();
		int start = cookie.Find(name + _T("="));
		if (start == -1)
			cookie += name + _T("=") + value + _T(";");
		else
		{
			start += name.GetLength() + 1;
			int end = cookie.Find(_T(';'), start);
			cookie = cookie.Left(start) + value + cookie.Right(cookie.GetLength() - end);
		}
	}
}

// HTTP請求
HTTPRequestResult HttpClient::HTTPRequestBase(BOOL postMethod, CComPtr<IServerXMLHTTPRequest>& xml,
	LPCTSTR URL, LPCTSTR data, BOOL useCookie, volatile BOOL* stopFlag, CString* cookie)
{
	if (FAILED(xml.CoCreateInstance(__uuidof(ServerXMLHTTP))))
		return NET_FAILED_TO_CREATE_INSTANCE;
	if (cookie == NULL)
		cookie = &g_cookie;

	if (postMethod)
	{
		xml->open(_bstr_t(_T("POST")), _bstr_t(URL), _variant_t(true), _variant_t(), _variant_t());
		xml->setRequestHeader(_bstr_t(_T("Content-Type")), _bstr_t(_T("application/x-www-form-urlencoded")));
	}
	else
		xml->open(_bstr_t(_T("GET")), _bstr_t(URL), _variant_t(true), _variant_t(), _variant_t());
	if (useCookie)
		xml->setRequestHeader(_bstr_t(_T("Cookie")), _bstr_t(*cookie));
	xml->send(_variant_t(data));

	// 等待
	DWORD startTime = GetTickCount();
	for (long state = 0; state != 4; xml->get_readyState(&state))
	{
		Delay(1);
		if (stopFlag != NULL && *stopFlag)
		{
			xml->abort();
			return NET_STOP;
		}
		if (GetTickCount() - startTime > 10000)
		{
			xml->abort();
			return NET_TIMEOUT;
		}
	}

	// 接收Cookie
	if (useCookie)
	{
		_bstr_t headers;
		xml->getAllResponseHeaders(headers.GetAddress());
		ReceiveCookie((LPCTSTR)headers, *cookie);
	}

	// 重定向
	long status;
	xml->get_status(&status);
	if (status == 302)
	{
		_bstr_t location;
		xml->getResponseHeader(_bstr_t(_T("Location")), location.GetAddress());
		return HTTPRequestBase(postMethod, xml, URL, data, useCookie, stopFlag, cookie);
	}

	return NET_SUCCESS;
}

// HTTP GET請求
CString HttpClient::HTTPGet(LPCTSTR URL, BOOL useCookie, volatile BOOL* stopFlag, CString* cookie)
{
	CComPtr<IServerXMLHTTPRequest> xml;
	HTTPRequestResult ret = HTTPRequestBase(FALSE, xml, URL, NULL, useCookie, stopFlag, cookie);
	if (ret != NET_SUCCESS)
		switch (ret)
	{
		case NET_FAILED_TO_CREATE_INSTANCE:
			return NET_FAILED_TO_CREATE_INSTANCE_TEXT;
		case NET_STOP:
			return NET_STOP_TEXT;
		case NET_TIMEOUT:
			return NET_TIMEOUT_TEXT;
	}

	_bstr_t result;
	xml->get_responseText(result.GetAddress());
	return (LPCTSTR)result;
}

// HTTP POST請求
CString HttpClient::HTTPPost(LPCTSTR URL, LPCTSTR data, BOOL useCookie, volatile BOOL* stopFlag, CString* cookie)
{
	CComPtr<IServerXMLHTTPRequest> xml;
	HTTPRequestResult ret = HTTPRequestBase(TRUE, xml, URL, data, useCookie, stopFlag, cookie);
	if (ret != NET_SUCCESS)
		switch (ret)
	{
		case NET_FAILED_TO_CREATE_INSTANCE:
			return NET_FAILED_TO_CREATE_INSTANCE_TEXT;
		case NET_STOP:
			return NET_STOP_TEXT;
		case NET_TIMEOUT:
			return NET_TIMEOUT_TEXT;
	}

	_bstr_t result;
	xml->get_responseText(result.GetAddress());
	return (LPCTSTR)result;
}

// HTTP GET請求，取得原始數據，注意自行delete buffer!!!
HTTPRequestResult HttpClient::HTTPGetRaw(LPCTSTR URL, BYTE** buffer, ULONG* size, BOOL useCookie, volatile BOOL* stopFlag, CString* cookie)
{
	if (buffer != NULL)
		*buffer = NULL;
	if (size != NULL)
		*size = 0;

	CComPtr<IServerXMLHTTPRequest> xml;
	HTTPRequestResult ret = HTTPRequestBase(FALSE, xml, URL, NULL, useCookie, stopFlag, cookie);
	if (ret != NET_SUCCESS)
		return ret;

	// 返回
	if (buffer != NULL && size != NULL)
	{
		_variant_t body;
		xml->get_responseBody(body.GetAddress());
		BYTE* p;
		if (SUCCEEDED(SafeArrayAccessData(body.parray, (void**)&p)))
		{
			*size = body.parray->rgsabound[0].cElements;
			*buffer = new BYTE[*size];
			memcpy(*buffer, p, *size);
			SafeArrayUnaccessData(body.parray);
		}
	}
	return NET_SUCCESS;
}


// 不阻塞消息的延遲
void HttpClient::Delay(DWORD time)
{
	DWORD startTime = GetTickCount();
	while (GetTickCount() - startTime < time)
	{
		DoEvents();
		Sleep(1); // 防止佔用CPU
	}
}

// 處理消息
void HttpClient::DoEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}
}
