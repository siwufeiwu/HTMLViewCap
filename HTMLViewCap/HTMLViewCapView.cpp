
// HTMLViewCapView.cpp : CHTMLViewCapView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "HTMLViewCap.h"
#endif

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
	// TODO: �ڴ˴���ӹ������

}

CHTMLViewCapView::~CHTMLViewCapView()
{
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

	EndModalLoop(S_OK);
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
	CList<CString> lstUrl;
	lstUrl.AddTail(CString(_T("http://178.com")));
	lstUrl.AddTail(CString(_T("http://weibo.com")));
	lstUrl.AddTail(CString(_T("http://qq.com")));

	SaveImages(lstUrl);
}

void CHTMLViewCapView::SaveImages(CList<CString> &lstUrl)
{
	Sleep(500);
	for (POSITION pos = lstUrl.GetHeadPosition();
		pos != NULL;
		lstUrl.GetNext(pos))
	{
		// Initialize GDI+.
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		IDispatch *pDoc = NULL;
		IViewObject *pViewObject = NULL;
		VARIANT vUrl;

		// ��������ļ�·��
		CTime t = CTime::GetCurrentTime();
		CString csTime = t.Format("\\%Hʱ%M��%S��-");
		CString csPath = ::theApp.m_csImageDir;
		csPath.Append(csTime);

		vUrl.vt = ::VT_BSTR;
		vUrl.bstrVal = (BSTR)(lstUrl.GetAt(pos).GetString());  

		if (m_pBrowserApp->Navigate2(&vUrl, NULL, NULL, NULL, NULL) == S_OK)
				RunModalLoop();
		else {
				TRACE(_T("%d Document Navigate Failed!\n"));
				return ;
		}
				
		// wait for document to load
		//....
		TRACE(_T("%s Document Complete!\n"), vUrl.bstrVal);
		m_pBrowserApp->Refresh();
		// render to enhanced metafile HDC.
		HRESULT hr = m_pBrowserApp->get_Document(&pDoc);

		if (hr != S_OK) {
			TRACE(_T("%s get_Document failed!\n"), vUrl.bstrVal);
			return ;
		}

		pDoc->QueryInterface(IID_IViewObject, (void**)&pViewObject); // result is first div of document

		if (pDoc == NULL) {
			TRACE(_T("%d query IID_IViewObject failed!\n"), vUrl.bstrVal);
			return ;
		}

		m_pBrowserApp->put_Width(1024);
		m_pBrowserApp->put_Height(1024);

		CBitmapDC destDC(1024, 1024);
		HRESULT drawResult = OleDraw(pViewObject, DVASPECT_CONTENT, destDC, NULL);

		CBitmap *pBM = destDC.Close();
		Bitmap *gdiBMP = Bitmap::FromHBITMAP(HBITMAP(pBM->GetSafeHandle()), NULL);
		CLSID jpegClsid;
		::GetEncoderClsid(_T("image/jpeg"), &jpegClsid);
		CString csFileName;
		csFileName.Format(_T("%u.jpg"), pos);
		csPath.Append(csFileName);
		gdiBMP->Save(csPath.GetString(), &jpegClsid, NULL);

		pDoc->Release();
		pViewObject->Release();
		GdiplusShutdown(gdiplusToken);
	}
}