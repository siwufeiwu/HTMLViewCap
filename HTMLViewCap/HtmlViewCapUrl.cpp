#include "StdAfx.h"
#include "HtmlViewCapUrl.h"


CHTMLViewCapUrl::CHTMLViewCapUrl(void)
{
	
}


CHTMLViewCapUrl::~CHTMLViewCapUrl(void)
{

}

CHTMLViewCapUrl::CHTMLViewCapUrl(CString csUrl, 
								 DWORD nWidth, 
								 DWORD nHeight) :
m_csUrl(csUrl), m_nWidth(nWidth), m_nHeight(nHeight)
{

}
