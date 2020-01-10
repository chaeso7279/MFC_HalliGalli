#pragma once


// CIntroDlg 대화 상자입니다.

class CIntroDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIntroDlg)

public:
	CIntroDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIntroDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INTRO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
