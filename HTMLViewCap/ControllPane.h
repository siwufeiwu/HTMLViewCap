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
	enum {
		TIMER_1,
		TIMER_2,
		TIMER_3
	};

	// ״̬�����ڼ�¼IDC_START��ť�Ƿ񱻰��¡�
	BOOL m_bIsStart;

	// ��¼���
	int m_nIntervel;

	// ��¼�������
	int m_nIntervelCount;

	//
	int m_nHour;
	int m_nMinute;

	// �������ͼƬ�Ŀ��
	int m_nWidth;
	int m_nHeight;

	// ��¼��ǰ״̬���ַ���
	CString m_csState;
	CString m_csUrl;

	CComboBox m_cbHours;
	CComboBox m_cbMins;
	CListBox m_lstUrl;

	CTime m_tCurrent;
	CTime m_tDest;
	CTimeSpan m_tsDiff;
	CTimeSpan m_tsTmSpanDiff;

	// ��������URL������
	CList<CHTMLViewCapUrl> m_lstHTMLUrl;

	// ���ڹرսű�����߳�
	CWinThread *m_pThrdClosed;

	// �ٽ��� ���ڷ�ֹ ��ͼTimer�ظ�����
	CRITICAL_SECTION m_cs;

	// Methods
	void UpdateTime();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBtnAddurl();
	afx_msg void OnUpdateBtnAddurl(CCmdUI *pCmdUI);
	afx_msg void OnBtnImportExcel();
	afx_msg void OnUpdateBtnImportExcel(CCmdUI *pCmdUI);
};


