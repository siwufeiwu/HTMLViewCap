
// HTMLViewCapView.cpp : CHTMLViewCapView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_SAVEIMAGE, &CHTMLViewCapView::OnSaveImage)
END_MESSAGE_MAP()

// CHTMLViewCapView ����/����

CHTMLViewCapView::CHTMLViewCapView()
{
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	::GetEncoderClsid(_T("image/jpeg"), &m_jpegClsid);

}

CHTMLViewCapView::~CHTMLViewCapView()
{
	GdiplusShutdown(m_gdiplusToken);
}

BOOL CHTMLViewCapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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
	IStream* pStream = NULL;
	// ��� DocumentComplete �¼��Ƿ��Ƕ����ܴ��ڵ�?
    // ��� COM ��ʶ: �Ƚ�IUnknown �ӿ�ָ��.
	hr = m_pBrowserApp->QueryInterface( IID_IUnknown, (void**)&pUnkBrowser );

	if ( SUCCEEDED(hr) ) {
		hr = pDisp->QueryInterface( IID_IUnknown,  (void**)&pUnkDisp );

		if ( SUCCEEDED(hr) ) {
			if ( pUnkBrowser == pUnkDisp ) {
				TRACE("Document completely loaded!\n");
				EndModalLoop(S_OK);
			}
		}

		pUnkDisp->Release();
	}
	pUnkBrowser->Release();
}


// CHTMLViewCapView ��ӡ


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


// CHTMLViewCapView ���

#ifdef _DEBUG
void CHTMLViewCapView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHTMLViewCapView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CHTMLViewCapDoc* CHTMLViewCapView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHTMLViewCapDoc)));
	return (CHTMLViewCapDoc*)m_pDocument;
}
#endif //_DEBUG


// CHTMLViewCapView ��Ϣ�������
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

void CHTMLViewCapView::SaveImages(CList<CHTMLViewCapUrl> &lstUrl)
{


	//
	POSITION pos = NULL;
	int nWidth  = 1024;
	int nHeight = 2048;
	IDispatch *pDoc = NULL;
	IViewObject *pViewObject = NULL;
	VARIANT vUrl;
	CTime t;
	CString csTime;
	CString csPath;
	CString csFileName;
	HRESULT hr;
	CBitmap *pBM;
	Bitmap *gdiBMP;

	for (pos = lstUrl.GetHeadPosition();
		pos != NULL;
		lstUrl.GetNext(pos))
	{

		nWidth  = lstUrl.GetAt(pos).m_nWidth;
		nHeight = lstUrl.GetAt(pos).m_nHeight;
		CString csUrl = lstUrl.GetAt(pos).m_csUrl;
		CBitmapDC destDC(nWidth, nHeight);

		// ��������ļ�·��
		t = CTime::GetCurrentTime();
		csTime = t.Format("\\%Hʱ%M��%S��-");
		csPath = ::theApp.m_csImageDir;
		csPath.Append(csTime);

		vUrl.vt = ::VT_BSTR;
		vUrl.bstrVal = (BSTR)csUrl.GetString();  

		m_pBrowserApp->put_Width(nWidth);
		m_pBrowserApp->put_Height(nHeight);

		if (m_pBrowserApp->Navigate2(&vUrl, NULL, NULL, NULL, NULL) == S_OK)
				RunModalLoop();
		else {
				TRACE(_T("%d Document Navigate Failed!\n"), vUrl);
				MessageBox(_T("Navi Error"), _T("Error"), MB_OK);
				//continue ;
		}
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

		pBM = destDC.Close();
		gdiBMP = Bitmap::FromHBITMAP(HBITMAP(pBM->GetSafeHandle()), NULL);

		csFileName.Format(_T("%u.jpg"), pos);
		csPath.Append(csFileName);
		gdiBMP->Save(csPath.GetString(), &m_jpegClsid, NULL);
	
		delete gdiBMP;
		pBM->DeleteObject();
		pViewObject->Release();
		pDoc->Release();
	}
}
