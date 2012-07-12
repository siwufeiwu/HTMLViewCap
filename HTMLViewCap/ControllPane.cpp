// ControllPane.cpp : implementation file
//
#include "stdafx.h"

#include "HTMLViewCap.h"
#include "MainFrm.h"
#include "HTMLViewCapDoc.h"
#include "HTMLViewCapView.h"

#include "ControllPane.h"



// CControllPane
IMPLEMENT_SERIAL(CControllPane, CPaneDialog, VERSIONABLE_SCHEMA | 1)

CControllPane::CControllPane()	
{
	OnInitDialog();
}

CControllPane::~CControllPane()
{

}

void CControllPane::OnInitDialog()
{
	m_bIsStart = FALSE;
	
	m_nHour   = 0;
	m_nMinute = 0;
	m_nIntervel = 30;
	//m_cbHours.CreateDlg(MAKEINTRESOURCE(IDC_HOURS), this);
	//m_cbHours.AddString(_T("1"));
	//TCHAR tcBuf[2];
	//for (int i=0; i<24; ++i)0
	//{
	//	_stprintf_s(tcBuf, _T("%d"), i);
	//	m_cbHours.AddString(_T("1"));
	//}
}

BEGIN_MESSAGE_MAP(CControllPane, CPaneDialog)
	ON_COMMAND(IDC_START, &CControllPane::OnStart)
	ON_COMMAND(IDC_STOP, &CControllPane::OnStop)
	ON_UPDATE_COMMAND_UI(IDC_START, &CControllPane::OnUpdateStart)
	ON_UPDATE_COMMAND_UI(IDC_STOP, &CControllPane::OnUpdateStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CControllPane message handlers
void CControllPane::OnStart()
{
	UpdateData(TRUE);
#ifdef _DEBUG
	TRACE("Hour: %d, Mintue: %d Intervel: %d.\n", 
			m_nHour, m_nMinute, m_nIntervel);
#endif

	CTime currTime = CTime::GetCurrentTime();
	tm destTime_tm;
	currTime.GetLocalTm(&destTime_tm);
	destTime_tm.tm_hour = m_nHour;
	destTime_tm.tm_min  = m_nMinute;
	destTime_tm.tm_sec  = 0;
	m_tDest = mktime(&destTime_tm);

#ifdef _DEBUG
	CString csTime = m_tDest.Format("Start will at : %Y-%m-%d %H:%M:%S\n");;
	TRACE(csTime);
#endif

	//
	m_bIsStart = TRUE;
	// 每秒获取系统时间
	this->SetTimer(1, 1000, NULL);
	
	//CHTMLImage html(this);
	//BOOL success = html.OnCreate();
	//TRACE("%d \n", success);
}

void CControllPane::OnStop()
{
	// 停止 截图
	TRACE("Stop!\n");
	m_bIsStart = FALSE;
}

void CControllPane::OnUpdateStart(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bIsStart);
}

void CControllPane::OnUpdateStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bIsStart);
}

void CControllPane::DoDataExchange(CDataExchange* pDX)
{
	//DDX_Control(pDX, IDC_HOURS, m_cbHours);
	DDX_Text(pDX, IDC_HOURS, m_nHour);
	DDX_Text(pDX, IDC_MINS,  m_nMinute);
	DDX_Text(pDX, IDC_INTERVEL, m_nIntervel);

	CDockablePane::DoDataExchange(pDX);
}

void CControllPane::OnTimer(UINT_PTR nIDEvent)
{
	CMainFrame *pWnd;
	CHTMLViewCapView *pView;
#ifdef _DEBUG
	CString csTime;
#endif

	switch (nIDEvent)
	{
	case 1 :
		m_tCurrent = CTime::GetCurrentTime();
		m_tsDiff = m_tDest - m_tCurrent;

#ifdef _DEBUG
		csTime = m_tCurrent.Format("%Y-%m-%d %H:%M:%S\n");
		TRACE(csTime);
		TRACE("TimeDiff : %d\n", m_tsDiff.GetTotalSeconds());
#endif
		if (m_tsDiff == 0) {
			KillTimer(nIDEvent);
			// 30分钟触发一次
			SetTimer(2, m_nIntervel * 1000, NULL);
			//SetTimer(2, m_nIntervel * 60 * 1000, NULL);
		}
		break;
	case 2 :
		TRACE("Timer 2 OnTimer.\n");

		/*
				获取View, 保存图片。
		*/
		pWnd = (CMainFrame *)GetParentFrame();
		pView = (CHTMLViewCapView *)pWnd->GetActiveView();
		pView->OnSaveImage();
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	//CDockablePane::OnTimer(nIDEvent);
}

