// HTMLImage.cpp : implementation file
//

#include "stdafx.h"
#include "HTMLViewCap.h"
#include "HTMLImage.h"


// CHTMLImage

CHTMLImage::CHTMLImage(CWnd *pParent)
{
	ASSERT(pParent);
	this->m_pParent = pParent;
	::_tccpy(lpszWindowName, _T("MyBrowser"));
}

CHTMLImage::~CHTMLImage()
{

}

BOOL CHTMLImage::OnCreate()
{
	RECT rectClient = {0, 0, 800, 600};
	// create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
	if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, lpszWindowName,
			WS_VISIBLE | WS_CHILD, rectClient, m_pParent, AFX_IDW_PANE_FIRST))
	{
		return FALSE;
	}

	return TRUE;
}


// CHTMLImage member functions
