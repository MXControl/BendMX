// ConfiguratorView.h : Schnittstelle der Klasse CConfiguratorView
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGURATORVIEW_H__16421B21_6844_4C77_9CCD_2365AFFFCD8B__INCLUDED_)
#define AFX_CONFIGURATORVIEW_H__16421B21_6844_4C77_9CCD_2365AFFFCD8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ini.h"
#include <queue>
#include <vector>
#include <Afxmt.h>
#include <Afxtempl.h>


typedef struct{

	CString strTrigger;
	CString strResponse;
} COMMAND, *PCOMMAND;


class CConfiguratorView : public CFormView
{
protected: // Nur aus Serialisierung erzeugen
	CConfiguratorView();
	DECLARE_DYNCREATE(CConfiguratorView)

// Attribute
public:
	CConfiguratorDoc* GetDocument();
// Dialogfelddaten
	//{{AFX_DATA(CConfiguratorView)
	enum { IDD = IDD_MAINVIEW };
	CListCtrl	m_lbEvents;
	//}}AFX_DATA

	CIni	m_Ini;
	CString m_strWd;
	CImageList m_cImageList;

	CStringArray m_aBadWords;
	// Sound events
	CArray<PCOMMAND, PCOMMAND> m_aSounds;

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CConfiguratorView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementierung
public:
	void ClearRandom(BOOL bAll = TRUE);
	void WriteConfig();
	CString m_strIni;
	BOOL m_bModified;
	void SaveSettings();
	void LoadSettings();
	void ParseConfig();
	virtual ~CConfiguratorView();
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;

#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CConfiguratorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopupDelete();
	afx_msg void OnRclickEvents(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupEditevent();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkEvents(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupAddsound();
	afx_msg void OnPopupPlaysound();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Testversion in ConfiguratorView.cpp
inline CConfiguratorDoc* CConfiguratorView::GetDocument()
   { return (CConfiguratorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_CONFIGURATORVIEW_H__16421B21_6844_4C77_9CCD_2365AFFFCD8B__INCLUDED_)
