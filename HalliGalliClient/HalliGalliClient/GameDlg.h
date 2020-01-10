#pragma once


// CGameDlg 대화 상자입니다.

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGameDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow(); 

public:
	/* 소켓 관련 함수 */
	void InitSocket(CSocCom* pSocCom);

public:
	afx_msg void OnStnClickedStaticGain();

public:
	/* 소켓 관련 변수 */
	CSocCom* m_pSocCom = nullptr;

public:
	CListBox m_list;

public:
	CString m_strCardCount;
	CString m_strGain;
	CStatic m_strMe;
	CString m_strSend;
};
