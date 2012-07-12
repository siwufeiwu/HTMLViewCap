#pragma once


// CControllPane

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

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitDialog();
private:
	BOOL m_bIsStart;
	int m_nHour;
	int m_nMinute;
	int m_nIntervel;
	CComboBox m_cbHours;

	CTime m_tCurrent;
	CTime m_tDest;
	CTimeSpan m_tsDiff;
	CTimeSpan m_tsTmSpanDiff;

	// Methods
	void UpdateTime();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


