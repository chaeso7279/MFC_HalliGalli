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
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow(); 

public:
	/* 소켓 관련 함수 */
	
	void InitSocket(CSocCom* pSocCom);
	void SendGame(int iType, CString strTemp = "");

public:
	/* 이미지 관련 함수 */
	void InitPicCtrl();

	void ChangeCardImage(const USER_ID& eID, const CARD_STATUS& eStatus = THROWN, const CARD& tCard = {FRUIT_BACK, 1});
	
	afx_msg void OnClieckedImgPlayerOwn(); // 카드 이미지 교체 테스트
	

	afx_msg void OnBnClickedButtonSub();

public:
	/* 게임 진행 관련 함수 */
	void InitGame();
	BOOL ReceiveCard(const char* pCardInfo);
	void addMyThrownCard(const CARD sCard);
	void deleteAllMyThrownCard();
	void addOtherThrownCard(const CARD sCard);
	void deleteAllOtherThrownCard();
	void CheckFive();
	void Win();

public:
	virtual void OnCancel();

	afx_msg void OnStnClickedStaticGain();

public:
	/* 소켓 관련 변수 */
	CSocCom* m_pSocCom = nullptr;

	BOOL m_bConnect = FALSE;

public:
	CListBox m_list;

public:
	/* 이미지 관련 변수 */
	CStatic m_BellPicCtrl; // 종 이미지 컨트롤
	CStatic m_CardPicCtrl[USER_END][CARD_END]; 	// 카드 이미지 컨트롤

	CImageMgr* m_pImgMgr = nullptr; // 이미지 매니저 포인터 변수

public:
	CString m_strCardCount;
	CString m_strGain;
	CStatic m_strMe;
	CString m_strSend;


public:
	list<CARD> m_lstMyCard;
	list<CARD> m_lstMyThrownCard;
	list<CARD> m_lstOtherThrownCard;

public:
	/* 게임 진행관련 변수 */
	BOOL m_bStartSvr = FALSE;
	BOOL m_bWin;

	
	afx_msg void OnStnClickedImgBell();
};