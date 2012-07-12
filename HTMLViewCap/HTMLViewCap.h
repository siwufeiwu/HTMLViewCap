
// HTMLViewCap.h : HTMLViewCap 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CHTMLViewCapApp:
// 有关此类的实现，请参阅 HTMLViewCap.cpp
//

class CHTMLViewCapApp : public CWinAppEx
{
public:
	CHTMLViewCapApp();

	CString m_csImageDir;

protected:
	CMultiDocTemplate* m_pDocTemplate;
public:

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNewFrame();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern CHTMLViewCapApp theApp;
