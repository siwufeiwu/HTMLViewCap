
// HTMLViewCap.h : HTMLViewCap Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CHTMLViewCapApp:
// �йش����ʵ�֣������ HTMLViewCap.cpp
//

class CHTMLViewCapApp : public CWinAppEx
{
public:
	CHTMLViewCapApp();

	CString m_csImageDir;

protected:
	CMultiDocTemplate* m_pDocTemplate;
public:

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
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
