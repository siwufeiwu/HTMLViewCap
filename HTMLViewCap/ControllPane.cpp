// ControllPane.cpp : implementation file
//
#include "stdafx.h"

#include "HTMLViewCap.h"
#include "MainFrm.h"
#include "HTMLViewCapDoc.h"

#include "HTMLViewCapUrl.h"

#include "HTMLViewCapView.h"

#include "ControllPane.h"


// CControllPane
IMPLEMENT_SERIAL(CControllPane, CPaneDialog, VERSIONABLE_SCHEMA | 1)

CControllPane::CControllPane() : m_bIsStart(FALSE)
{
	m_nIntervel = 180;
}

CControllPane::~CControllPane()
{

}

LRESULT CControllPane::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	CPaneDialog::HandleInitDialog(wParam, lParam);

	/*
			��ʼ�� URL�б��
	*/
	//CListBox *lstUrl = (CListBox *)GetDlgItem(IDC_LIST_URL);
	//lstUrl->AddString(_T("http://weibo.com"));
	//lstUrl->AddString(_T("http://qq.com"));

	/*
			Сʱ ComboBox
	*/
	TCHAR tcBuf[3];
	CComboBox *cbHours = (CComboBox *)GetDlgItem(IDC_HOURS);
	for (int i=0; i<=24; ++i)
	{
		_stprintf_s(tcBuf, _T("%d"), i);
		cbHours->AddString(tcBuf);
	}
	cbHours->SelectString(0, _T("0"));
	/*
			���� ComboBox
	*/
	CComboBox *cbMins = (CComboBox *)GetDlgItem(IDC_MINS);
	for (int i=0; i<=60; ++i)
	{
		_stprintf_s(tcBuf, _T("%d"), i);
		cbMins->AddString(tcBuf);
	}
	cbMins->SelectString(0, _T("0"));

	/*
			״̬ Static
	*/
	CStatic *txtState = (CStatic *)GetDlgItem(IDC_TEXT_STATE);
	CString csState;
	txtState->GetWindowText(csState);
	csState.Append(_T("״̬: ׼������"));
	txtState->SetWindowText(csState);

	/*
			�߶�
	*/
	CEdit *edtHeight = (CEdit *)GetDlgItem(IDC_EDIT_HEIGHT);
	edtHeight->SetWindowText(_T("2048"));

	/*
			���
	*/
	CEdit *edtWidth = (CEdit *)GetDlgItem(IDC_EDIT_WIDTH);
	edtWidth->SetWindowText(_T("1024"));

	return TRUE;
}

BEGIN_MESSAGE_MAP(CControllPane, CPaneDialog)
	ON_COMMAND(IDC_START, &CControllPane::OnStart)
	ON_COMMAND(IDC_STOP, &CControllPane::OnStop)
	ON_UPDATE_COMMAND_UI(IDC_START, &CControllPane::OnUpdateStart)
	ON_UPDATE_COMMAND_UI(IDC_STOP, &CControllPane::OnUpdateStop)
	ON_WM_TIMER()
	ON_COMMAND(IDC_BTN_ADDURL, &CControllPane::OnBtnAddurl)
	ON_UPDATE_COMMAND_UI(IDC_BTN_ADDURL, &CControllPane::OnUpdateBtnAddurl)
	ON_MESSAGE(WM_INITDIALOG, &CControllPane::HandleInitDialog)
	ON_COMMAND(IDC_BTN_IMPORT_EXCEL, &CControllPane::OnBtnImportExcel)
	ON_UPDATE_COMMAND_UI(IDC_BTN_IMPORT_EXCEL, &CControllPane::OnUpdateBtnImportExcel)
END_MESSAGE_MAP()

UINT CloseErrorWnd(LPVOID pParam)
{
	HWND hWnd = NULL;
	BOOL isClosed = FALSE;
	while (isClosed != TRUE) {
		hWnd = ::FindWindow(_T("Internet Explorer_TridentDlgFrame"), 
							_T("Internet Explorer �ű�����"));
		if (hWnd != NULL)
		{
			SendMessage(hWnd,   WM_SYSCOMMAND,   SC_CLOSE,   0);
			hWnd = NULL;
			TRACE("Closed windows!\n");
		}
		Sleep(1000);
	}
    return 0;   // thread completed successfully
}


// CControllPane message handlers
void CControllPane::OnStart()
{
	UpdateData(TRUE);

	AfxBeginThread(CloseErrorWnd, NULL);
	//m_nHour	 = m_cbHours.GetCurSel();
	//m_nMinute  = m_cbMins.GetCurSel();

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
	// ÿ���ȡϵͳʱ��
	this->SetTimer(1, 1000, NULL);
}

void CControllPane::OnStop()
{
	// ֹͣ ��ͼ
	TRACE("Stop!\n");
	m_bIsStart = FALSE;

	CStatic *txtState = (CStatic *)GetDlgItem(IDC_TEXT_STATE);
	CString csState;
	txtState->GetWindowText(csState);
	csState.Format(_T("״̬: ׼������"));
	txtState->SetWindowText(csState);

	KillTimer(1);
	KillTimer(2);
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
	DDX_Control(pDX, IDC_LIST_URL, m_lstUrl);
	DDX_Control(pDX, IDC_HOURS, m_cbHours);
	DDX_Control(pDX, IDC_MINS, m_cbMins);
	DDX_Text(pDX, IDC_HOURS, m_nHour);
	DDX_Text(pDX, IDC_MINS,  m_nMinute);
	DDX_Text(pDX, IDC_INTERVEL, m_nIntervel);
	DDX_Text(pDX, IDC_EDIT_URL, m_csUrl);
	DDX_Text(pDX, IDC_TEXT_STATE, m_csState);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
}

void CControllPane::OnTimer(UINT_PTR nIDEvent)
{
	CMainFrame *pWnd;
	CHTMLViewCapView *pView;
	CList<CString> lstUrl;
	CStatic *txtState;
	CString csState;

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
		txtState = (CStatic *)GetDlgItem(IDC_TEXT_STATE);
		txtState->GetWindowText(csState);
		csState.Format(_T("״̬: %u ���ʼ��ȡ��"), m_tsDiff.GetTotalSeconds());
		txtState->SetWindowText(csState);

		if (m_tsDiff == 0) {
			KillTimer(nIDEvent);
			// 30���Ӵ���һ��
			SetTimer(2, m_nIntervel * 1000, NULL);
			OnTimer(2);
			//SetTimer(2, m_nIntervel * 60 * 1000, NULL);
		}
		break;
	case 2 :
		TRACE("Timer 2 OnTimer.\n");

		/*
				��ȡView, ����ͼƬ��
		*/
		pWnd = (CMainFrame *)GetParentFrame();
		pView = (CHTMLViewCapView *)pWnd->GetActiveView();
		UpdateData(TRUE);
		for (int i=0; i<m_lstUrl.GetCount(); ++i)
		{
			CString csUrl;
			m_lstUrl.GetText(i, csUrl);
			lstUrl.AddTail(csUrl);
		}
		pView->SaveImages(m_lstHTMLUrl);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CControllPane::OnBtnAddurl()
{
	UpdateData(TRUE);
	m_lstHTMLUrl.AddTail(CHTMLViewCapUrl(m_csUrl, m_nWidth, m_nHeight));
	CString url = m_csUrl;
	url.AppendFormat(_T(" ��:%d ��:%d "), m_nWidth, m_nHeight);
	m_lstUrl.AddString(url);
	UpdateData(TRUE);
}

void CControllPane::OnUpdateBtnAddurl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bIsStart);
}

void CControllPane::OnBtnImportExcel()
{
	BasicExcel e;
	CFileDialog dlgFile(TRUE);
	CStringA fileName;
	if(dlgFile.DoModal() != IDOK)
	{
		TRACE(_T("CFileDialog Cancel.\n"));
		return;
	}
	fileName = dlgFile.GetPathName();
	e.Load(fileName.GetString());

	BasicExcelWorksheet* URLSheet = e.GetWorksheet("URL");

	if (URLSheet == NULL) 
	{
		MessageBox(_T("��.xls�ļ�ʧ�ܣ���ȷ���ļ���ʽ��ȷ�͹ر����ڴ򿪵��ļ�֮�������³���"),
				   _T("����"));
		TRACE(_T("Open .xls file failed.\n"));
		return ;
	}

	m_lstHTMLUrl.RemoveAll();
	if (URLSheet) {
		const DWORD nColLen = 3;
		const DWORD nRowLen = URLSheet->GetTotalRows();
		TCHAR tcUrl[200];
		for (DWORD i=1; i<nRowLen; ++i)
		{
			BasicExcelCell *cell = URLSheet->Cell(i, 0);
			const char *cUrl = cell->GetString();
			MultiByteToWideChar(CP_ACP, 0, cUrl, strlen(cUrl)+1, tcUrl, 200);  
			CString csUrl(tcUrl);
	
			cell = URLSheet->Cell(i, 1);
			DWORD nWidth = cell->GetInteger();
			if (nWidth == 0) 
				nWidth = 1024;
			cell = URLSheet->Cell(i, 2);
			DWORD nHeight = cell->GetInteger();
			if (nHeight == 0)
				nHeight = 2048;
			m_lstHTMLUrl.AddTail(CHTMLViewCapUrl(csUrl, nWidth, nHeight));
		}

		// ����GUI 
		UpdateData(TRUE);
		m_lstUrl.ResetContent();
		for (POSITION pos=m_lstHTMLUrl.GetHeadPosition(); 
			 pos!=m_lstHTMLUrl.GetTailPosition(); 
			 m_lstHTMLUrl.GetNext(pos))
		{
			CHTMLViewCapUrl url = m_lstHTMLUrl.GetAt(pos);
			CString csUrl = url.m_csUrl;
			csUrl.AppendFormat(_T(" ��:%d ��:%d "), url.m_nWidth, url.m_nHeight);
			m_lstUrl.AddString(csUrl);
		}
		UpdateData(TRUE);
		//
		MessageBox(_T("��ȡ�ļ��ɹ���"),
				   _T("��Ϣ"));
	} 
}


void CControllPane::OnUpdateBtnImportExcel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bIsStart);
}
