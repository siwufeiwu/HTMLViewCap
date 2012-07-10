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
	CComboBox m_cbHours;
};


