#pragma once
class CHTMLViewCapUrl
{
public:
	CHTMLViewCapUrl(void);
	~CHTMLViewCapUrl(void);

	CHTMLViewCapUrl(CString csUrl, DWORD nWidth, DWORD nHeight, CString csMediaName);

public:
	CString m_csUrl;
	CString m_csMediaName;
	DWORD m_nWidth;
	DWORD m_nHeight;
	BOOL m_bHasMediaName;


public:
	CString getMediaNameInUrl();
};

