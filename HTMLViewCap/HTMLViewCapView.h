
// HTMLViewCapView.h : CHTMLViewCapView ��Ľӿ�
//

#pragma once


class CHTMLViewCapView : public CHtmlView
{
protected: // �������л�����
	CHTMLViewCapView();
	DECLARE_DYNCREATE(CHTMLViewCapView)

// ����
public:
	CHTMLViewCapDoc* GetDocument() const;

// ����
public:
	void SaveImages(CList<CString> &lstUrl);

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

// ʵ��
public:
	virtual ~CHTMLViewCapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // HTMLViewCapView.cpp �еĵ��԰汾
inline CHTMLViewCapDoc* CHTMLViewCapView::GetDocument() const
   { return reinterpret_cast<CHTMLViewCapDoc*>(m_pDocument); }
#endif

