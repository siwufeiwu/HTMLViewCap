
// HTMLViewCapView.h : CHTMLViewCapView 类的接口
//

#pragma once


class CHTMLViewCapView : public CHtmlView
{
protected: // 仅从序列化创建
	CHTMLViewCapView();
	DECLARE_DYNCREATE(CHTMLViewCapView)

// 特性
public:
	CHTMLViewCapDoc* GetDocument() const;

// 操作
public:
	void SaveImages(CList<CString> &lstUrl);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

// 实现
public:
	virtual ~CHTMLViewCapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSaveImage();
};

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

#ifndef _DEBUG  // HTMLViewCapView.cpp 中的调试版本
inline CHTMLViewCapDoc* CHTMLViewCapView::GetDocument() const
   { return reinterpret_cast<CHTMLViewCapDoc*>(m_pDocument); }
#endif

