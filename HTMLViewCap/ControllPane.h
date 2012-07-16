#pragma once

#include "HTMLViewCapUrl.h"

class CControllPane : public CPaneDialog
{
	DECLARE_SERIAL(CControllPane)

public:
	CControllPane();
	virtual ~CControllPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnUpdateStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStop(CCmdUI *pCmdUI);
	afx_msg LRESULT HandleInitDialog(WPARAM wParam,LPARAM lParam);

	virtual void DoDataExchange(CDataExchange* pDX);
private:
	BOOL m_bIsStart;
	int m_nIntervel;
	int m_nHour;
	int m_nMinute;
	int m_nWidth;
	int m_nHeight;

	CString m_csState;
	CString m_csUrl;

	CComboBox m_cbHours;
	CComboBox m_cbMins;
	CListBox m_lstUrl;

	CTime m_tCurrent;
	CTime m_tDest;
	CTimeSpan m_tsDiff;
	CTimeSpan m_tsTmSpanDiff;

	CList<CHTMLViewCapUrl> m_lstHTMLUrl;

	// Methods
	void UpdateTime();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBtnAddurl();
	afx_msg void OnUpdateBtnAddurl(CCmdUI *pCmdUI);
	afx_msg void OnBtnImportExcel();
	afx_msg void OnUpdateBtnImportExcel(CCmdUI *pCmdUI);
};


