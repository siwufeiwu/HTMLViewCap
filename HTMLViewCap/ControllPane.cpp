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

CControllPane::CControllPane() : 
		m_bIsStart(FALSE), m_nIntervel(180), m_pThrdClosed(NULL)
{
	::InitializeCriticalSection(&m_cs);
}

CControllPane::~CControllPane()
{
	::DeleteCriticalSection(&m_cs);
}

LRESULT CControllPane::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	CPaneDialog::HandleInitDialog(wParam, lParam);

	CTime currTime = CTime::GetCurrentTime();
	CString csSelect;

	/*
			初始化 URL列表框
	*/
	//CListBox *lstUrl = (CListBox *)GetDlgItem(IDC_LIST_URL);
	//lstUrl->AddString(_T("http://weibo.com"));
	//lstUrl->AddString(_T("http://qq.com"));

	/*
			小时 ComboBox
	*/
	TCHAR tcBuf[3];
	CComboBox *cbHours = (CComboBox *)GetDlgItem(IDC_HOURS);
	for (int i=0; i<=24; ++i)
	{
		_stprintf_s(tcBuf, _T("%d"), i);
		cbHours->AddString(tcBuf);
	}
	int hour = currTime.GetHour();
	csSelect.Format(_T("%d"), hour);
	cbHours->SelectString(0,  csSelect);

	/*
			分钟 ComboBox
	*/
	CComboBox *cbMins = (CComboBox *)GetDlgItem(IDC_MINS);
	for (int i=0; i<=60; ++i)
	{
		_stprintf_s(tcBuf, _T("%d"), i);
		cbMins->AddString(tcBuf);
	}
	int minute = currTime.GetMinute();
	csSelect.Format(_T("%d"), minute);
	cbMins->SelectString(0, csSelect);

	/*
			状态 Static
	*/
	CStatic *txtState = (CStatic *)GetDlgItem(IDC_TEXT_STATE);
	CString csState;
	txtState->GetWindowText(csState);
	csState.Append(_T("状态: 准备就绪"));
	txtState->SetWindowText(csState);

	/*
			高度
	*/
	CEdit *edtHeight = (CEdit *)GetDlgItem(IDC_EDIT_HEIGHT);
	edtHeight->SetWindowText(_T("2048"));

	/*
			宽度
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
	while (!isClosed) {
		hWnd = ::FindWindow(_T("Internet Explorer_TridentDlgFrame"), 
							_T("Internet Explorer 脚本错误"));
		if (hWnd == NULL) {
			hWnd = ::FindWindow(_T("Internet Explorer_TridentDlgFrame"), 
					_T("脚本错误"));
		}

		if (hWnd != NULL)
		{
			SendMessage(hWnd,   WM_SYSCOMMAND,   SC_CLOSE,   0);
			hWnd = NULL;
			TRACE("Closed window!\n");
		}
		TRACE("Finding window!\n");
		Sleep(1000);
	}
    return 0;   // thread completed successfully
}


// CControllPane message handlers
void CControllPane::OnStart()
{
	UpdateData(TRUE);

	if (!m_pThrdClosed)
		m_pThrdClosed = AfxBeginThread(CloseErrorWnd, NULL);
	else
		m_pThrdClosed->ResumeThread();

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
}

void CControllPane::OnStop()
{
	// 停止 截图
	TRACE("Stop!\n");
	m_bIsStart = FALSE;

	if (m_pThrdClosed) {
		m_pThrdClosed->SuspendThread();
	}

	CStatic *txtState = (CStatic *)GetDlgItem(IDC_TEXT_STATE);
	CString csState;
	txtState->GetWindowText(csState);
	csState.Format(_T("状态: 准备就绪"));
	txtState->SetWindowText(csState);

	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
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
	CTime currTime;

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
		csState.Format(_T("状态: %u 秒后开始截取。"), m_tsDiff.GetTotalSeconds());
		txtState->SetWindowText(csState);

		if (m_tsDiff == 0) {
			KillTimer(1);

			// 设置触发频率
			m_nIntervelCount = m_nIntervel;
			KillTimer(3);
			SetTimer(3, 1000, NULL);
			SetTimer(2, m_nIntervel * 1000, NULL);
			OnTimer(2);
		}
		break;
	case 2 :
		TRACE("Timer 2 OnTimer.\n");

		/*
				获取View, 保存图片。
		*/
		::EnterCriticalSection(&m_cs);
		pWnd = (CMainFrame *)GetParentFrame();
		pView = (CHTMLViewCapView *)pWnd->GetActiveView();
		pView->SaveImages(m_lstHTMLUrl);
		::LeaveCriticalSection(&m_cs);
		break;

	case 3 :
		if (m_nIntervelCount == 0)
			m_nIntervelCount = m_nIntervel;

		m_nIntervelCount--;
		txtState = (CStatic *)GetDlgItem(IDC_TEXT_STATE);
		txtState->GetWindowText(csState);
		csState.Format(_T("状态: 离下次截图还有 %d 秒。"), m_nIntervelCount);
		txtState->SetWindowText(csState);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CControllPane::OnBtnAddurl()
{
	UpdateData(TRUE);
	std::wsmatch m;
	std::wregex regexHostname = 
				std::wregex(L"([[:w:]]+)\.(com|net|org|gov|cc|biz|info|cn)(\.(cn|hk))*");
	std::wstring strUrl = m_csUrl.GetString();
	bool found = std::regex_search(strUrl, m, regexHostname);

	if (found) {
		CHTMLViewCapUrl objUrl = CHTMLViewCapUrl(m_csUrl, m_nWidth, m_nHeight, _T(""));
		objUrl.m_bHasMediaName = FALSE;
		m_lstHTMLUrl.AddTail(objUrl);
		CString csUrl = m_csUrl;
		csUrl.AppendFormat(_T(" 宽:%d 高:%d "), m_nWidth, m_nHeight);
		m_lstUrl.AddString(csUrl);
	} else {
		::AfxMessageBox(_T("Url 格式不正确!"));
	}
	UpdateData(TRUE);
}

void CControllPane::OnUpdateBtnAddurl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bIsStart);
}

/*
		导入Excel文件
*/
void CControllPane::OnBtnImportExcel()
{
	BasicExcel e;
	CFileDialog dlgFile(TRUE);
	CStringA csFileName;
	if(dlgFile.DoModal() != IDOK)
	{
		TRACE(_T("CFileDialog Cancel.\n"));
		return;
	}
	csFileName = dlgFile.GetPathName();
	e.Load(csFileName.GetString());

	BasicExcelWorksheet* URLSheet = e.GetWorksheet("URL");

	if (URLSheet == NULL) 
	{
		MessageBox(_T("打开.xls文件失败，请确认文件格式正确和关闭正在打开的文件之后再重新尝试"),
				   _T("警告"));
		TRACE(_T("Open .xls file failed.\n"));
		return ;
	}

	if (URLSheet) {
		//const DWORD nColLen = 3;
		const DWORD nRowLen = URLSheet->GetTotalRows();
		TCHAR tcUrl[200];

		/*
				先清楚原来的URL数据，然后导入新的URL数据。
		*/
		m_lstHTMLUrl.RemoveAll();
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

			cell = URLSheet->Cell(i, 3);
			CString csMediaName  = cell->GetWString();
			m_lstHTMLUrl.AddTail(CHTMLViewCapUrl(csUrl, nWidth, nHeight, csMediaName));
		}

		// 更新GUI 
		UpdateData(TRUE);
		m_lstUrl.ResetContent();
		for (POSITION pos=m_lstHTMLUrl.GetHeadPosition(); 
			 pos != NULL; 
			 m_lstHTMLUrl.GetNext(pos))
		{
			CHTMLViewCapUrl url = m_lstHTMLUrl.GetAt(pos);
			CString csUrl = url.m_csUrl;
			csUrl.AppendFormat(_T(" 宽:%d 高:%d "), url.m_nWidth, url.m_nHeight);
			m_lstUrl.AddString(csUrl);
		}
		UpdateData(TRUE);
		//
		MessageBox(_T("读取文件成功。"),
				   _T("消息"));
	} 
}


void CControllPane::OnUpdateBtnImportExcel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bIsStart);
}
