
// HTMLViewCapView.cpp : CHTMLViewCapView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HTMLViewCap.h"
#endif

#include "HtmlViewCapUrl.h"

#include "HTMLViewCapDoc.h"
#include "HTMLViewCapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "BitmapDC.h"

// CHTMLViewCapView

IMPLEMENT_DYNCREATE(CHTMLViewCapView, CHtmlView)

BEGIN_MESSAGE_MAP(CHTMLViewCapView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_SAVEIMAGE, &CHTMLViewCapView::OnSaveImage)
END_MESSAGE_MAP()

// CHTMLViewCapView 构造/析构

CHTMLViewCapView::CHTMLViewCapView()
{
	m_wating_time_max = 10;

	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	::GetEncoderClsid(_T("image/jpeg"), &m_jpegClsid);
	m_hDocCompleteEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

CHTMLViewCapView::~CHTMLViewCapView()
{
	::CloseHandle(m_hDocCompleteEvent);
	GdiplusShutdown(m_gdiplusToken);
}

BOOL CHTMLViewCapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CHtmlView::PreCreateWindow(cs);
}

void CHTMLViewCapView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate(); 
}

void CHTMLViewCapView::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	UNUSED_ALWAYS(pDisp);
	UNUSED_ALWAYS(URL);

	HRESULT hr;
	IUnknown* pUnkBrowser = NULL;
    IUnknown* pUnkDisp = NULL;

	// 这个 DocumentComplete 事件是否是顶层框架窗口的?
    // 检查 COM 标识: 比较IUnknown 接口指针.
	hr = m_pBrowserApp->QueryInterface(IID_IUnknown, (void**)&pUnkBrowser);

	if ( SUCCEEDED(hr) ) {
		hr = pDisp->QueryInterface(IID_IUnknown,  (void**)&pUnkDisp);

		if ( SUCCEEDED(hr) ) {
			if ( pUnkBrowser == pUnkDisp ) {
				TRACE("Top Document completely loaded!\n");
				EndModalLoop(S_OK);
				//::SetEvent(m_hDocCompleteEvent);
			}
		}

		pUnkDisp->Release();
	}
	pUnkBrowser->Release();
}


// CHTMLViewCapView 打印


void CHTMLViewCapView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


void CHTMLViewCapView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHTMLViewCapView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHTMLViewCapView 诊断

#ifdef _DEBUG
void CHTMLViewCapView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHTMLViewCapView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CHTMLViewCapDoc* CHTMLViewCapView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHTMLViewCapDoc)));
	return (CHTMLViewCapDoc*)m_pDocument;
}
#endif //_DEBUG


// CHTMLViewCapView 消息处理程序
void CHTMLViewCapView::OnSize(UINT nType, int cx, int cy)
{

}

// 
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;  // number of image encoders
   UINT  size = 0; // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

void CHTMLViewCapView::OnSaveImage()
{
	//CList<CString> lstUrl;
	//lstUrl.AddTail(CString(_T("http://178.com")));
	//lstUrl.AddTail(CString(_T("http://weibo.com")));
	//lstUrl.AddTail(CString(_T("http://qq.com")));
	//SaveImages(lstUrl);
}

void CHTMLViewCapView::SaveImages(CList<CString> &lstUrl)
{

}

/*
		计算等待时间的函数
*/
static UINT calTime(LPVOID /* para */) 
{
	return 0;
}


void CHTMLViewCapView::SaveImages(CList<CHTMLViewCapUrl> &lstUrl)
{
	// 
	POSITION pos = NULL;

	// 浏览器和图片的宽度和高度
	DWORD nWidth  = 1024;
	DWORD nHeight = 2048;

	IDispatch *pDoc = NULL;
	IViewObject *pViewObject = NULL;
	VARIANT vUrl;
	VARIANT vUrlName;

	//
	CTime t;
	CString csTime, csDate, csMark1;
	CString csPath;
	CString csFileName;
	HRESULT hr;

	//
	CBitmap *pBM;
	Bitmap *gdiBMP;
	// 用于生成图片的序数
	static DWORD nNum = 0;

	/*
			创建字体
	*/
	HFONT hfont;
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight   =   0;
	lf.lfWidth    =   0;
	lf.lfEscapement   =   0;
	lf.lfOrientation   =   0;
	lf.lfStrikeOut   =   FALSE;
	lf.lfCharSet   =   DEFAULT_CHARSET;
	lf.lfOutPrecision   =   OUT_DEFAULT_PRECIS;  
	lf.lfClipPrecision   =   CLIP_DEFAULT_PRECIS;  
	lf.lfQuality   =   ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily =   VARIABLE_PITCH;
	lstrcpy(lf.lfFaceName,   _T("微软雅黑"));
	hfont   =   CreateFontIndirect(&lf);

	// 循环遍历所有URL
	for (pos = lstUrl.GetHeadPosition();
		   pos != NULL;
		   lstUrl.GetNext(pos))
	{
		// 获取图片和浏览器分辨率
		nWidth  = lstUrl.GetAt(pos).m_nWidth;
		nHeight = lstUrl.GetAt(pos).m_nHeight;
		CString &csUrl = lstUrl.GetAt(pos).m_csUrl;
		CBitmapDC destDC(nWidth, nHeight);

		// 创建 媒体名称目录
		csPath = ::theApp.m_csImageDir;
		csPath.Append(_T("\\"));
		if (lstUrl.GetAt(pos).m_bHasMediaName) {
			csPath.Append(lstUrl.GetAt(pos).m_csMediaName);
		}  else {
			csPath.Append(lstUrl.GetAt(pos).getMediaNameInUrl());
		}

		::CreateDirectory(csPath, NULL);

		// 创建输出文件路径
		t = CTime::GetCurrentTime();
		csTime = t.Format("\\%H时%M分%S秒-");
		csMark1 = t.Format("%H:%M");
		csDate    = t.Format(" %Y/%m/%d");
		csPath.Append(csTime);

		vUrl.vt = ::VT_BSTR;
		vUrl.bstrVal = (BSTR)csUrl.GetString();  

		m_pBrowserApp->put_Width(nWidth);
		m_pBrowserApp->put_Height(nHeight);

		if (m_pBrowserApp->Navigate2(&vUrl, NULL, NULL, NULL, NULL) == S_OK)
		{
			m_tBeforeEnterLoop = CTime::GetCurrentTime();
			RunModalLoop();
		} else {
			TRACE(_T("%d Document Navigate Failed!\n"), vUrl);
			MessageBox(_T("Navi Error"), _T("Error"), MB_OK);
			return ;
		}
		TRACE("Begin Cap!\n");
		// wait for document to load
		m_pBrowserApp->Refresh();

		// render to enhanced metafile HDC.
		hr = m_pBrowserApp->get_Document(&pDoc);

		if (hr != S_OK) {
			TRACE(_T("%s get_Document failed!\n"), vUrl.bstrVal);
			return ;
		}

		pDoc->QueryInterface(IID_IViewObject, (void**)&pViewObject); // result is first div of document

		if (pDoc == NULL) {
			TRACE(_T("%d query IID_IViewObject failed!\n"), vUrl.bstrVal);
			return ;
		}

		hr = OleDraw(pViewObject, DVASPECT_CONTENT, destDC, NULL);
		if (hr != S_OK) {
			TRACE(_T("%s OleDraw failed!\n"), vUrl.bstrVal);
			return ;
		} 

		/*
				使用字体
		*/
		destDC.SelectObject(hfont);
		destDC.SetTextColor(RGB(0, 0, 0));
		destDC.SetBkColor(RGB(235, 231, 228));
		//destDC.SetBkMode(TRANSPARENT);

        // 输出水印
		TextOut(destDC, nWidth - 70, nHeight - 100, (LPCTSTR)csMark1, csMark1.GetLength()); 
		TextOut(destDC, nWidth - 100, nHeight - 70, (LPCTSTR)csDate, csDate.GetLength()); 
		pBM = destDC.Close();
		gdiBMP = Bitmap::FromHBITMAP(HBITMAP(pBM->GetSafeHandle()), NULL);

		csFileName.Format(_T("%u.jpg"), nNum++);
		csPath.Append(csFileName);
		
		// 保存图片
		gdiBMP->Save(csPath.GetString(), &m_jpegClsid, NULL);
	
		// 清理资源
		delete gdiBMP;
		pBM->DeleteObject();
		pViewObject->Release();
		pDoc->Release();
	}
}

/*
			等待5秒
*/
int CHTMLViewCapView::RunModalLoop(DWORD dwFlags)
{
			ASSERT(::IsWindow(m_hWnd)); // window must be created
			ASSERT(!(m_nFlags & WF_MODALLOOP)); // window must not already be in modal state

			// for tracking the idle time state
			BOOL bIdle = TRUE;
			LONG lIdleCount = 0;
			BOOL bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
			HWND hWndParent = ::GetParent(m_hWnd);
			m_nFlags |= (WF_MODALLOOP|WF_CONTINUEMODAL);
			MSG *pMsg = AfxGetCurrentMessage();

			// acquire and dispatch messages until the modal state is done
			for (;;)
			{
				ASSERT(ContinueModal());

				// phase1: check to see if we can do idle work
				while (bIdle &&
					!::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE))
				{
					ASSERT(ContinueModal());

					// show the dialog when the message queue goes idle
					if (bShowIdle)
					{
						ShowWindow(SW_SHOWNORMAL);
						UpdateWindow();
						bShowIdle = FALSE;
					}

					// call OnIdle while in bIdle state
					if (!(dwFlags & MLF_NOIDLEMSG) && hWndParent != NULL && lIdleCount == 0)
					{
						// send WM_ENTERIDLE to the parent
						::SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)m_hWnd);
					}
					if ((dwFlags & MLF_NOKICKIDLE) ||
						!SendMessage(WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++))
					{
						// stop idle processing next time
						bIdle = FALSE;
					}
				}

				// Get current time
				//CTime tBeginLoop;
				CTime tEndLoop;
				CTimeSpan span;
				BOOL bQuit = FALSE;

				// phase2: pump messages while available
				do
				{
					ASSERT(ContinueModal());
					TRACE("pump messages!\n");

					//
					//tBeginLoop = CTime::GetCurrentTime();

					// pump message, but quit on WM_QUIT
					if (!AfxPumpMessage())
					{
						AfxPostQuitMessage(0);
						return -1;
					}

					// show the window when certain special messages rec'd
					if (bShowIdle &&
						(pMsg->message == 0x118 || pMsg->message == WM_SYSKEYDOWN))
					{
						ShowWindow(SW_SHOWNORMAL);
						UpdateWindow();
						bShowIdle = FALSE;
					}

					if (!ContinueModal() || bQuit)
						goto ExitModal;

					// reset "no idle" state after pumping "normal" message
					if (AfxIsIdleMessage(pMsg))
					{
						bIdle = TRUE;
						lIdleCount = 0;
					}

					tEndLoop = CTime::GetCurrentTime();
					span =  tEndLoop - m_tBeforeEnterLoop;
					TRACE("Span : %d .\n", span.GetTotalSeconds());
					if (span.GetTotalSeconds() >= m_wating_time_max)
					{
						bQuit = TRUE;
					}

				} while (::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE));
			}

		ExitModal:
			m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);
			return m_nModalResult;
}
