#pragma once


// CGameDlg 대화 상자입니다.

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

	/* Picture Ctrl 용 상수값 */
	enum USER_ID { USER_PLAYER, USER_OTHER, USER_END };
	enum CARD_STATUS { OWN, THROWN, CARD_END };

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
	LPARAM OnReceive(UINT wParam, LPARAM lParam);

	void InitSocket(CSocCom* pSocCom);

public:
	/* 이미지 관련 함수 */
	void InitPicCtrl();

	void ChangeCardImage(const USER_ID& eID, const CARD_STATUS& eStatus = THROWN, const CARD& tCard = {FRUIT_BACK, 1});
	
//	afx_msg void OnClieckedImgPlayerOwn(); // 카드 이미지 교체 테스트

public:
	virtual void OnCancel();

public:
	/* 소켓 관련 변수 */
	CSocCom* m_pSocCom = nullptr;

public:
	CListBox m_list;

public:
	/* 이미지 관련 변수 */
	CStatic m_BellPicCtrl; // 종 이미지 컨트롤
	CStatic m_CardPicCtrl[USER_END][CARD_END]; 	// 카드 이미지 컨트롤

	CImageMgr* m_pImgMgr = nullptr; // 이미지 매니저 포인터 변수

public:
	CString m_strGain;
	CString m_strSend;
	CEdit m_strWholeCountNum;
	CEdit m_strCardCountNum;
	CStatic m_strCardCount;
	CStatic m_strWholeCount;
	CString m_strMe;
	afx_msg void OnBnClickedButtonSend();
	void SendChat(int iType, CString strTmp);
	afx_msg void OnClickedImgPlayerOwn();


};
