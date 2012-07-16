#pragma once
class CHTMLViewCapUrl
{
public:
	CHTMLViewCapUrl(void);
	~CHTMLViewCapUrl(void);

	CHTMLViewCapUrl(CString csUrl, DWORD nWidth, DWORD nHeight);

public:
	CString m_csUrl;
	DWORD m_nWidth;
	DWORD m_nHeight;
};

