// ControllPane.cpp : implementation file
//

#include "stdafx.h"
#include "HTMLViewCap.h"
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
END_MESSAGE_MAP()

// CControllPane message handlers
void CControllPane::OnStart()
{
	// ¿ªÊ¼ ½ØÍ¼
	TRACE("Start!\n");
	m_bIsStart = TRUE;

	CTime t = CTime::GetCurrentTime();

	//CHTMLImage html(this);
	//BOOL success = html.OnCreate();
	//TRACE("%d \n", success);
}

void CControllPane::OnStop()
{
	// Í£Ö¹ ½ØÍ¼
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
	CDockablePane::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOURS, m_cbHours);
}


