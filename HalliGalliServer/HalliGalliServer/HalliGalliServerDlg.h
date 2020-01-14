
// HalliGalliServerDlg.h : 헤더 파일
//

#pragma once

#include "SocSvr.h"
#include "PictureEx.h"

#include "Timer.h"
#include "Frame.h"

// CHalliGalliServerDlg 대화 상자
class CHalliGalliServerDlg : public CDialogEx
{
	/* Picture Ctrl 용 상수값 */
	enum USER_ID { USER_PLAYER, USER_OTHER, USER_END };
	enum CARD_STATUS { OWN, THROWN, CARD_END };

// 생성입니다.
public:
	CHalliGalliServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HALLIGALLISERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lParam);
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam); // 루프문
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();

public:
	/* 소켓 관련 함수 */
	void InitSocket();
	void SendGame(int iType, CString strTemp = "");

public:
	/* 이미지 관련 함수 */
	void InitPicCtrl();
	void ChangeCardImage(const USER_ID& eID, const CARD_STATUS& eStatus = THROWN, const CARD& tCard = { FRUIT_BACK, 1 });
	
	afx_msg void OnClickedImgPlayerOwn();
	afx_msg void OnClickedImgBell();

public:
	/* 게임 진행 관련 함수 */
	void InitGame(); // 게임 초기화 함수 (변수 등 초기화)
	void InitCardDeck(); // 카드 초기화 및 셔플 함수
	void SendCardToClient(); // 섞은 카드 클라에 보내는 함수

	void CheckThrownCard(); // THROWN CARD 5개인지 검사
	void TakeThrownCard(); // 종을 먼저 쳐서 카드를 모두 가져감
	
	void AddMyThrownCard(const CARD sCard);
	void AddOtherThrownCard(const CARD sCard);
	void DeleteAllMyThrownCard();
	void DeleteAllOtherThrownCard();

	void ChangeMyTurn(BOOL bMyTurn);

	void CheckWin(const int& iOtherCnt);
	BOOL IsGameEnd(); // 게임이 끝났는지 검사하는 함수

public:
	/* 채팅 관련 함수 */
	afx_msg void OnClickedButtonSend();

public:
	/* 소켓 관련 변수 */
	CSocSvr m_socServer;
	CSocCom* m_pSocCom = nullptr;

	BOOL m_bConnect = FALSE;

public:
	/* 이미지 관련 변수 */
	CPictureEx m_BellPicCtrl; // 종 이미지 컨트롤
	CPictureEx m_CardPicCtrl[USER_END][CARD_END]; 	// 카드 이미지 컨트롤

	CImageMgr* m_pImgMgr = nullptr; // 이미지 매니저 포인터 변수
	
public:
	/* 카드 초기 셔플 */
	CARD m_cardDec[CARD_CNT];
	int  m_shuffleCardIndex[CARD_CNT];

	/* 카드 관련 자료구조 */
	list<CARD> m_lstMyCard;
	list<CARD> m_lstMyThrownCard;
	list<CARD> m_lstOtherThrownCard;

public:
	/* 게임 진행관련 변수 */
	BOOL m_bStartCnt = FALSE;
	BOOL m_bTakeCard = FALSE;
	BOOL m_bMyTurn = FALSE;
	BOOL m_bOtherBell = FALSE;
	
	BOOL m_bWin = FALSE;
	BOOL m_bGameEnd = FALSE;

	CEdit m_strCardCountNum;
	CEdit m_strWholeCountNum;
	
	CStatic m_strWholeCount;
	CStatic m_strCardCount;
	CString m_strMe;
	CString m_strGain;

public:
	/* 채팅 관련 변수 */
	CListBox m_list;
	CString m_strSend;

public:
	/* 타이머 관련 변수 */
	CTimer* m_pImmediateTimer = nullptr;
	CTimer* m_p60Timer = nullptr;
	CFrame* m_pFrame = nullptr;
};