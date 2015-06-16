#if !defined(AFX_WELCOMEMSG_H__41262A23_6D62_4C4A_A0FC_EF67C6B59A1D__INCLUDED_)
#define AFX_WELCOMEMSG_H__41262A23_6D62_4C4A_A0FC_EF67C6B59A1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WelcomeMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWelcomeMsg dialog
#ifdef __BENDMX__

class CWelcomeMsg : public CDialog
{
// Construction
public:
	void Apply();
	CWelcomeMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWelcomeMsg)
	enum { IDD = IDD_WELCOMEMSGS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWelcomeMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWelcomeMsg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //#ifdef __BENDMX__

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOMEMSG_H__41262A23_6D62_4C4A_A0FC_EF67C6B59A1D__INCLUDED_)
