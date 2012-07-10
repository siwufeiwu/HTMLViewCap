#pragma once
#include "stdafx.h"

// CHTMLImage command target

class CHTMLImage : public CObject
{
protected:

public:
	CHTMLImage(CWnd *m_pParent);
	virtual ~CHTMLImage();

	virtual BOOL OnCreate();

protected:
	CWnd *m_pParent;
	CWnd m_wndBrowser;
	TCHAR lpszWindowName[40];
};


