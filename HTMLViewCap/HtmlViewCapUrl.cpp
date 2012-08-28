#include "StdAfx.h"

#include <string>

#include "HtmlViewCapUrl.h"

CHTMLViewCapUrl::CHTMLViewCapUrl(void)
{
	
}


CHTMLViewCapUrl::~CHTMLViewCapUrl(void)
{

}

CHTMLViewCapUrl::CHTMLViewCapUrl(CString csUrl, 
								 DWORD nWidth, 
								 DWORD nHeight,
								 CString csMediaName)  :
m_csUrl(csUrl), m_nWidth(nWidth), m_nHeight(nHeight),
m_csMediaName(csMediaName), m_bHasMediaName(TRUE)
{

}
CString CHTMLViewCapUrl::getMediaNameInUrl()
{
	CString csRet = _T("");
	std::wsmatch m;
	std::wregex regexHostname = 
				std::wregex(L"([[:w:]]+)\.(com|net|org|gov|cc|biz|info|cn)(\.(cn|hk))*");
	
	std::wstring strUrl = m_csUrl.GetString();
	bool found = std::regex_search(strUrl, m, regexHostname);
	if (found) {
		csRet = m.str().c_str();
	} 
	return csRet;
}
