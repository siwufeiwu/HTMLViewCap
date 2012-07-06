
// HTMLViewCapView.cpp : CHTMLViewCapView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HTMLViewCap.h"
#endif

#include "HTMLViewCapDoc.h"
#include "HTMLViewCapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "BitmapDC.h"
#include "FileSpec.h"

// CHTMLViewCapView

IMPLEMENT_DYNCREATE(CHTMLViewCapView, CHtmlView)

BEGIN_MESSAGE_MAP(CHTMLViewCapView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SAVEIAMGE, &CHTMLViewCapView::OnSaveiamge)
END_MESSAGE_MAP()

// CHTMLViewCapView 构造/析构

CHTMLViewCapView::CHTMLViewCapView()
{
	// TODO: 在此处添加构造代码

}

CHTMLViewCapView::~CHTMLViewCapView()
{
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

	EndModalLoop(S_OK);
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

void CHTMLViewCapView::OnSaveiamge()
{
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	VARIANT vUrl;
	vUrl.vt = ::VT_BSTR;
	vUrl.bstrVal = _T("http://weibo.com");
	if (m_pBrowserApp->Navigate2(&vUrl, NULL, NULL, NULL, NULL) == S_OK)
		RunModalLoop();
	else
		TRACE("Nav2 FALSE!\n");

	// wait for document to load
    //....

    /// render to enhanced metafile HDC.
    IDispatch *pDoc = (IDispatch *) NULL;
    HRESULT hr = m_pBrowserApp->get_Document(&pDoc);
    IViewObject* pViewObject = NULL;
    pDoc->QueryInterface(IID_IViewObject, (void**)&pViewObject); // result is first div of document

	/*
	Bitmap myBmp(800, 600);
	Graphics g(&myBmp);
	HDC mydc = g.GetHDC();
	if (mydc != NULL)
	{
		HRESULT drawResult = OleDraw(pViewObject, DVASPECT_CONTENT, mydc, NULL);
		g.ReleaseHDC(mydc);
	}

	CLSID jpegClsid;
	GetEncoderClsid(_T("image/jpeg"), &jpegClsid);
	myBmp.Save(_T("c://1.jpg"), &jpegClsid, NULL);
	*/

	m_pBrowserApp->put_Width(1024);
	m_pBrowserApp->put_Height(1024);

	CFileSpec fsDest(_T("1.bmp"));
	CBitmapDC destDC(1024, 1024);
    HRESULT drawResult = OleDraw(pViewObject, DVASPECT_CONTENT, destDC, NULL);
	pViewObject->Release();

	CBitmap *pBM = destDC.Close();
	Bitmap *gdiBMP = Bitmap::FromHBITMAP(HBITMAP(pBM->GetSafeHandle()), NULL);
	CLSID jpegClsid;
	::GetEncoderClsid(_T("image/bmp"), &jpegClsid);
	gdiBMP->Save(_T("c://1.jpg"), &jpegClsid, NULL);

	//Bitmap *gdiBMP = Bitmap::FromHBITMAP(HBITMAP(pBM->GetSafeHandle()), NULL);
	//gdiBMP->Save(_T("C:\\1.bmp"), 2);
/*	Image  *gdiThumb = gdiBMP->GetThumbnailImage(100, 100)*/;

//// Create an Image object based on a PNG file.
//   Image  image(L"Mosaic.png");
//
//   Graphics graphics(hdc);
//   // Draw the image.
//   graphics.DrawImage(&image, 10, 10);
//
//   // Construct a Graphics object based on the image.
//   Graphics imageGraphics(&image);
//
//   // Alter the image.
//   SolidBrush brush(Color(255, 0, 0, 255));
//   imageGraphics.FillEllipse(&brush, 20, 30, 80, 50);
//
//   // Draw the altered image.
//   graphics.DrawImage(&image, 200, 10);
//
//   // Save the altered image.
//   CLSID pngClsid;
//   GetEncoderClsid(L"image/png", &pngClsid);
//   image.Save(L"Mosaic2.png", &pngClsid, NULL);

	//gdiThumb->Save(A2W(fsDest.GetFullSpec()), &m_encoderClsid);
	//delete gdiBMP;
	////delete gdiThumb;
	//delete pBM;

	GdiplusShutdown(gdiplusToken);
}
