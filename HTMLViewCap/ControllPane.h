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

	// 状态：用于记录IDC_START按钮是否被按下。
	BOOL m_bIsStart;

	// 记录间隔
	int m_nIntervel;

	// 记录间隔计数
	int m_nIntervelCount;

	//
	int m_nHour;
	int m_nMinute;

	// 浏览器和图片的宽高
	int m_nWidth;
	int m_nHeight;

	// 记录当前状态的字符串
	CString m_csState;
	CString m_csUrl;

	CComboBox m_cbHours;
	CComboBox m_cbMins;
	CListBox m_lstUrl;

	CTime m_tCurrent;
	CTime m_tDest;
	CTimeSpan m_tsDiff;
	CTimeSpan m_tsTmSpanDiff;

	// 保存所有URL的链表
	CList<CHTMLViewCapUrl> m_lstHTMLUrl;

	// 用于关闭脚本错的线程
	CWinThread *m_pThrdClosed;

	// 临界区 用于防止 截图Timer重复运行
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


