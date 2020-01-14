// GameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "GameDlg.h"
#include "afxdialogex.h"

#include "IntroDlg.h"


// CGameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DLG, pParent)
	, m_strSend(_T(""))
	, m_bTakeCard{}
	, m_strMe(_T("상대방의 차례입니다"))
	, m_strWholeCountNum(_T(""))
	, m_strCardCountNum(_T("0"))
{
}

CGameDlg::~CGameDlg()
{
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CGameDlg::OnClickedImgPlayerOwn)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CGameDlg::OnBnClickedButtonSend)	
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_STN_CLICKED(IDC_IMG_BELL, &CGameDlg::OnClickedImgBell)
END_MESSAGE_MAP()

BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	/* 이미지 관리 매니저 초기화 => Image폴더 내의 이미지 로드함 */
	m_pImgMgr = CImageMgr::GetInstance();
	m_pImgMgr->Initialize();

	/* Picture Ctrl 초기화 */
	InitPicCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
BOOL CGameDlg::DestroyWindow()
{
	/* 이미지 매니저 삭제 */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	return CDialogEx::DestroyWindow();
}

void CGameDlg::OnCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();

	/* 인트로 다이얼로그 삭제 */
	g_pIntroDlg->DestroyWindow();
}

LPARAM CGameDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	char pTemp[MAX_STR];
	CString strTemp, str;

	memset(pTemp, '\0', MAX_STR);
	m_pSocCom->Receive(pTemp, MAX_STR);

	strTemp.Format("%c", pTemp[0]);
	int iType = atoi(strTemp.GetString());
	
	CString strFruitID;
	CString	strFruitCnt;
	CARD tCard;
	

	CString strOtherCardCnt;
	CString strCardCnt;
	CString strGameEvent;

	int iOtherCardCnt = 0;
	int iEvent = 0;

	m_strWholeCountNum.Format("%d", m_iTurnCnt);

	switch (iType)
	{
	case SOC_INITGAME:
		/* 서버로부터 카드 받기 */
		if (ReceiveCard(pTemp + 1) == TRUE)
		{
			SendGame(SOC_GAMESTART);
			ChangeMyTurn(FALSE);
		}	
		break;
	case SOC_GAMESTART:
		m_iTurnCnt = TURN_CNT;
		m_bStartSvr = TRUE;
		break;
	case SOC_THROWNCARD:	
		strFruitID.Format("%c", (pTemp+1)[0]);
		strFruitCnt.Format("%c", (pTemp + 1)[1]);

		tCard.iFruitID = atoi(strFruitID.GetString());
		tCard.iFruitCnt = atoi(strFruitCnt.GetString());

		AddOtherThrownCard(tCard);
		ChangeCardImage(USER_OTHER, THROWN, tCard);
		ChangeMyTurn(TRUE);
		m_iTurnCnt--;
		m_strWholeCountNum.Format("%d", m_iTurnCnt);
		UpdateData(FALSE);

		break;
	case SOC_BELL:
		/* 상대가 벨을 눌렀을 경우 */
		m_bOtherBell = TRUE;
		break;
	case SOC_TAKECARD:
		/* 상대가 카드를 가져갔을 경우 */
		m_bOtherBell = FALSE;
		DeleteAllMyThrownCard();
		DeleteAllOtherThrownCard();
		AfxMessageBox("상대가 카드를 획득했습니다!");
		break;
	case SOC_NOTAKECARD:
		/* 상대가 벨을 잘못친 경우 */
		m_bOtherBell = FALSE;
		strFruitID.Format("%c", (pTemp + 1)[0]);
		strFruitCnt.Format("%c", (pTemp + 1)[1]);

		tCard.iFruitID = atoi(strFruitID.GetString());
		tCard.iFruitCnt = atoi(strFruitCnt.GetString());

		m_lstMyCard.push_back(tCard);
		m_strCardCountNum.Format("%d", m_lstMyCard.size());
		UpdateData(FALSE);

		AfxMessageBox("상대방의 카드를 획득했습니다!");
		break;
	case SOC_TEXT:
		str.Format("%s", pTemp + 1);
		m_list.AddString("(상대방) : " + str);
		break;
	case SOC_NOTURN:
		strOtherCardCnt.Format("%s", pTemp + 1);
		iOtherCardCnt = atoi(strOtherCardCnt.GetString());
		CheckWin(iOtherCardCnt);
		break;
	case SOC_GAMEEND:
		strGameEvent.Format("%s", pTemp + 1);
		iEvent = atoi(strGameEvent.GetString());
		switch (iEvent)
		{
		case GAME_WIN:
			/* 상대가 이김 */
			AfxMessageBox("패배했습니다");
			break;
		case GAME_LOSE:
			/* 상대가 짐 */
			AfxMessageBox("승리했습니다");
			break;
		case GAME_DRAW:
			/* 무승부 */
			AfxMessageBox("무승부입니다");
			break;
		}
		m_bGameEnd = TRUE;
		break;
	}

	return TRUE;
}

void CGameDlg::InitSocket(CSocCom * pSocCom)
{
	if (pSocCom == nullptr)
	{
		AfxMessageBox("Socket이 NULL 입니다");
		return;
	}

	m_pSocCom = pSocCom;
	m_pSocCom->Init(this->m_hWnd);

	m_bConnect = TRUE;
	//InitGame(); // 서버와 연결 후 게임 초기화
}

void CGameDlg::SendGame(int iType, CString strTemp)
{
	UpdateData(TRUE);
	char pTemp[MAX_STR];

	memset(pTemp, '\0', MAX_STR);
	sprintf_s(pTemp, "%d%s", iType, strTemp.GetString());

	m_pSocCom->Send(pTemp, MAX_STR);
}

void CGameDlg::InitPicCtrl()
{
	ChangeCardImage(USER_PLAYER, OWN);
	ChangeCardImage(USER_OTHER, OWN);

	ChangeCardImage(USER_PLAYER, THROWN, CARD(FRUIT_BACK, 2));
	ChangeCardImage(USER_OTHER, THROWN, CARD(FRUIT_BACK, 2));

	CImage* pImage = nullptr;

	pImage = m_pImgMgr->GetImage("Bell");
	if (pImage != nullptr)
		m_BellPicCtrl.SetBitmap(*pImage);
}

void CGameDlg::ChangeCardImage(const USER_ID & eID, const CARD_STATUS & eStatus, const CARD & tCard)
{
	CImage* pImage = m_pImgMgr->GetCardImage(tCard.iFruitID, tCard.iFruitCnt);

	if (pImage != nullptr)
	{
		m_CardPicCtrl[eID][eStatus].SetBitmap(*pImage);
		Invalidate(TRUE);
	}
}


void CGameDlg::OnClickedImgPlayerOwn()
{
	if (!m_bConnect || !m_bStartSvr || !m_bMyTurn || m_bGameEnd)
		return;

	CARD tCard;

	tCard.iFruitID = m_lstMyCard.back().iFruitID;
	tCard.iFruitCnt = m_lstMyCard.back().iFruitCnt;
	
	AddMyThrownCard(tCard);
	ChangeCardImage(USER_PLAYER, THROWN, tCard); 
	
	m_lstMyCard.pop_back();

	/* 서버에 낸 카드 전달 */
	char pCardInfo[MID_STR] = "";
	sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);
	SendGame(SOC_THROWNCARD, pCardInfo);

	/* 카드 잔여수 및 턴수 변경 */
	m_strCardCountNum.Format("%d", m_lstMyCard.size());
	m_iTurnCnt--;
	m_strWholeCountNum.Format("%d", m_iTurnCnt);

	UpdateData(FALSE);

	/* 게임 끝났는지 검사 */
	if (IsGameEnd())
		return;

	/* 턴 변경 */
	ChangeMyTurn(FALSE);
}

void CGameDlg::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SendGame(SOC_TEXT, m_strSend);
	
	m_list.AddString("   (나) : " + m_strSend);
	m_strSend = (_T(""));
	
	UpdateData(FALSE);
	
}

void CGameDlg::InitGame()
{
}

BOOL CGameDlg::ReceiveCard(const char* pCardInfo)
{
	/* 카드 다 받으면 게임 Start */
	CARD tCard; 
	CString strFruitID, strFruitCnt; 
	
	strFruitID.Format("%c", pCardInfo[0]);
	strFruitCnt.Format("%c", pCardInfo[1]);

	tCard.iFruitID = atoi(strFruitID.GetString());
	tCard.iFruitCnt = atoi(strFruitCnt.GetString());

	m_lstMyCard.push_back(tCard);

	/* 카드 개수가 14이면 TRUE, 아니면 FALSE 리턴 */
	if (m_lstMyCard.size() == CARD_HALF_CNT)
	{
		m_strCardCountNum.Format("%d", m_lstMyCard.size());
		UpdateData(FALSE);

		return TRUE;
	}
	else
		return FALSE;
}

void CGameDlg::AddMyThrownCard(const CARD sCard)
{
	m_lstMyThrownCard.push_back(sCard);	
}

void CGameDlg::DeleteAllMyThrownCard()
{
	int nThrowCardCount = 0;
	/* 내가 카드를 가져갈 경우 */
	if (m_bTakeCard)
	{
		nThrowCardCount = m_lstMyThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_front(m_lstMyThrownCard.back());
			m_lstMyThrownCard.pop_back();
		}
	}
	/* 상대가 카드를 가져갈 경우 */
	else
	{
		m_lstMyThrownCard.clear();
	}

	ChangeCardImage(USER_PLAYER, THROWN, CARD(FRUIT_BACK, 2));
}

void CGameDlg::AddOtherThrownCard(const CARD sCard)
{
	m_lstOtherThrownCard.push_back(sCard);
}

void CGameDlg::DeleteAllOtherThrownCard()
{
	int nThrowCardCount = 0;
	/* 내가 카드를 가져갈 경우 */
	if (m_bTakeCard)
	{
		nThrowCardCount = m_lstOtherThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_front(m_lstOtherThrownCard.back());
			m_lstOtherThrownCard.pop_back();
		}
	}
	/* 상대가 카드를 가져갈 경우 */
	else
	{
		m_lstOtherThrownCard.clear();
	}

	ChangeCardImage(USER_OTHER, THROWN, CARD(FRUIT_BACK, 2));
}

void CGameDlg::ChangeMyTurn(BOOL bMyTurn)
{
	m_bMyTurn = bMyTurn;
	if (m_bMyTurn)
		m_strMe = "당신의 차례입니다";
	else
		m_strMe = "상대방의 차례입니다";

	GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(m_bMyTurn);

	UpdateData(FALSE);
}

void CGameDlg::CheckWin(const int & iOtherCnt)
{
	CString strEvent;

	if (iOtherCnt > int(m_lstMyCard.size())) // 상대의 카드가 나보다 많을 때
	{
		m_bWin = FALSE;
		strEvent.Format("%d", GAME_LOSE);
		SendGame(SOC_GAMEEND, strEvent);
		AfxMessageBox("패배했습니다");
	}
	else if (iOtherCnt < int(m_lstMyCard.size()))
	{
		m_bWin = TRUE;
		strEvent.Format("%d", GAME_WIN);
		SendGame(SOC_GAMEEND, strEvent);
		AfxMessageBox("승리했습니다");
	}
	else
	{
		m_bWin = FALSE;
		strEvent.Format("%d", GAME_DRAW);
		SendGame(SOC_GAMEEND, strEvent);
		AfxMessageBox("무승부입니다");
	}

	m_bGameEnd = TRUE;
}

BOOL CGameDlg::IsGameEnd()
{
	/* 게임이 끝났는지 검사하는 함수 */
	if (m_lstMyCard.size() <= 0) // 내가 가진 카드가 없을 때
	{
		m_bWin = FALSE;
		AfxMessageBox("패배했습니다");

		CString strEvent;
		strEvent.Format("%d", GAME_LOSE);
		SendGame(SOC_GAMEEND, strEvent);

		m_bGameEnd = TRUE;

		return TRUE;
	}

	if (m_iTurnCnt <= 0) // 전체 턴수가 없을 때 
	{
		CString strCardCnt;
		strCardCnt.Format("%d", m_lstMyCard.size());

		SendGame(SOC_NOTURN, strCardCnt);

		return TRUE;
	}

	return FALSE;
}

void CGameDlg::OnClickedImgBell()
{
	if (m_bGameEnd)
		return;

	/* 벨을 클릭 했을 경우 */
	if (m_bOtherBell) // 이미 상대방이 벨을 먼저 눌렀으면 return
		return;

	SendGame(SOC_BELL);
	CheckThrownCard(); // 카드 검사

	if (m_bTakeCard)
	{
		// 벨을 잘 때렸을 경우
		TakeThrownCard();
		SendGame(SOC_TAKECARD); // 카드를 가져갔음을 상대에게 알림

		AfxMessageBox("카드 획득!");

		m_bTakeCard = FALSE;
	}
	else
	{
		// 벨을 잘못 때렸을 경우
		CARD tCard;

		tCard.iFruitID = m_lstMyCard.back().iFruitID;
		tCard.iFruitCnt = m_lstMyCard.back().iFruitCnt;

		m_lstMyCard.pop_back();
		AfxMessageBox("종을 잘못쳤네요. 카드 하나를 잃었습니다!");
		
		/* 카드 전달 */
		char pCardInfo[MID_STR] = "";
		sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);
		SendGame(SOC_NOTAKECARD,pCardInfo); // 상대에게 벨을 잘못 때렸음을 알림

		m_strCardCountNum.Format("%d", m_lstMyCard.size());
		UpdateData(FALSE);
	}
}

void CGameDlg::CheckThrownCard()
{
	int iMyFruitCnt = 0;
	int iOtherFruitCnt = 0;
	
	int iMyFruitID = FRUIT_END;
	int iOhtherFruitID = FRUIT_END;

	if (!m_lstMyThrownCard.empty())
	{
		iMyFruitCnt = m_lstMyThrownCard.back().iFruitCnt;
		iMyFruitID = m_lstMyThrownCard.back().iFruitID;
	}
		
	if (!m_lstOtherThrownCard.empty())
	{
		iOtherFruitCnt = m_lstOtherThrownCard.back().iFruitCnt;
		iOhtherFruitID = m_lstOtherThrownCard.back().iFruitID;
	}
		
	if (iMyFruitID != FRUIT_END && iOhtherFruitID != FRUIT_END)
	{
		/* 서로 과일의 종류가 같을 때 */
		if (iMyFruitID == iOhtherFruitID)
		{
			if (iMyFruitCnt + iOtherFruitCnt == 5)
				m_bTakeCard = TRUE;
			else
				m_bTakeCard = FALSE;
		}
		else
		{
			/* 서로 과일의 종류가 다를 때 */
			if(iMyFruitCnt == 5 || iOtherFruitCnt == 5)
				m_bTakeCard = TRUE;
			else
				m_bTakeCard = FALSE;
		}
	}
	else
		m_bTakeCard = FALSE;
}

void CGameDlg::TakeThrownCard()
{
	DeleteAllMyThrownCard();
	DeleteAllOtherThrownCard();
	m_strCardCountNum.Format("%d", m_lstMyCard.size());
	UpdateData(FALSE);
}