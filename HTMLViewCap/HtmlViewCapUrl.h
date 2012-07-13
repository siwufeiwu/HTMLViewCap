#pragma once
class CHtmlViewCapUrl
{
public:
	CHtmlViewCapUrl(void);
	~CHtmlViewCapUrl(void);

	CHtmlViewCapUrl(CString csUrl, DWORD nWidth, DWORD nHeight);
private:
	CString m_csUrl;
	DWORD m_nWidth;
	DWORD m_nHeight;
};

