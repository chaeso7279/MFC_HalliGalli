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
	, m_strCardCount(_T(""))
	, m_strGain(_T(""))
	, m_strSend(_T(""))
	, m_bWin{}
{

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_CARDCOUNT, m_strCardCount);
	DDX_Text(pDX, IDC_STATIC_GAIN, m_strGain);
	DDX_Control(pDX, IDC_STATIC_ME, m_strMe);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Control(pDX, IDC_IMG_BELL, m_BellPicCtrl);
	DDX_Control(pDX, IDC_IMG_OTHER_OWN, m_CardPicCtrl[USER_OTHER][OWN]);
	DDX_Control(pDX, IDC_IMG_OTHER_THROWN, m_CardPicCtrl[USER_OTHER][THROWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_OWN, m_CardPicCtrl[USER_PLAYER][OWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_THROWN, m_CardPicCtrl[USER_PLAYER][THROWN]);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_GAIN, &CGameDlg::OnStnClickedStaticGain)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CGameDlg::OnClieckedImgPlayerOwn)
	
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CGameDlg::OnBnClickedButtonSub)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_STN_CLICKED(IDC_IMG_BELL, &CGameDlg::OnStnClickedImgBell)
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	/* 이미지 매니저 삭제 */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	return CDialogEx::DestroyWindow();
}

LPARAM CGameDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	char pTemp[MAX_STR];
	CString strTemp;

	memset(pTemp, '\0', MAX_STR);
	m_pSocCom->Receive(pTemp, MAX_STR);

	strTemp.Format("%c", pTemp[0]);
	int iType = atoi(strTemp.GetString());

	switch (iType)
	{
	case SOC_INITGAME:
		/* 서버로부터 카드 받기 */
		if (ReceiveCard(pTemp + 1) == TRUE)
			SendGame(SOC_GAMESTART);
		break;
	case SOC_GAMESTART:
		m_bStartSvr = TRUE;
		//cout << "게임 준비 완료" << endl;
		break;
	case SOC_THROWNCARD:
		CARD tCard;

		
		break;
	case SOC_BELL:
		break;
	case SOC_TAKECARD:
		break;
	case SOC_TEXT:
		break;
	case SOC_GAMEEND:
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
	char pTemp[MAX_STR];

	memset(pTemp, '\0', MAX_STR);
	sprintf_s(pTemp, "%d%s", iType, strTemp.GetString());

	m_pSocCom->Send(pTemp, MAX_STR);
}

void CGameDlg::InitPicCtrl()
{
	ChangeCardImage(USER_PLAYER, OWN);
	ChangeCardImage(USER_OTHER, OWN);
}

void CGameDlg::ChangeCardImage(const USER_ID & eID, const CARD_STATUS & eStatus, const CARD & tCard)
{
	CImage* pImage = m_pImgMgr->GetCardImage(tCard.iFruitID, tCard.iFruitCnt);

	if(pImage != nullptr)
		m_CardPicCtrl[eID][eStatus].SetBitmap(*pImage);
}

// CGameDlg 메시지 처리기입니다.
void CGameDlg::OnClieckedImgPlayerOwn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// 자신이 낸 카드 이미지 처리
	CARD tCard;
	
	tCard.iFruitID = m_lstMyCard.back().iFruitID;
	tCard.iFruitCnt = m_lstMyCard.back().iFruitCnt;
	addMyThrownCard(tCard);
	m_lstMyCard.pop_back();
	ChangeCardImage(USER_PLAYER, THROWN, tCard);
	//SendGame(SOC_THROWNCARD);
}


void CGameDlg::OnStnClickedStaticGain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGameDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDialogEx::OnCancel();
	DestroyWindow();

	/* 인트로 다이얼로그 삭제 */
	g_pIntroDlg->DestroyWindow();
}

void CGameDlg::OnBnClickedButtonSub()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	deleteAllMyThrownCard();


	ChangeCardImage(USER_OTHER, THROWN, m_lstMyThrownCard.front());
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
		return TRUE;
	else
		return FALSE;
}

void CGameDlg::addMyThrownCard(const CARD sCard)
{
	m_lstMyThrownCard.push_back(sCard);
	
}

void CGameDlg::deleteAllMyThrownCard()
{
	int nThrowCardCount = 0;
	//내가 이겼을 때
	if (m_bWin)
	{
		nThrowCardCount = m_lstMyThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_back(m_lstMyThrownCard.back());
			m_lstMyThrownCard.pop_back();
		}
	}
	//졌을 때
	else
	{
		m_lstMyThrownCard.clear();
	}
	
}

void CGameDlg::addOtherThrownCard(const CARD sCard)
{
	m_lstOtherThrownCard.push_back(sCard);
}

void CGameDlg::deleteAllOtherThrownCard()
{
	int nThrowCardCount = 0;
	//내가 이겼을때
	if (m_bWin)
	{
		nThrowCardCount = m_lstOtherThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_back(m_lstOtherThrownCard.back());
			m_lstOtherThrownCard.pop_back();
		}
	}
	//내가 졌을때
	else
	{
		m_lstOtherThrownCard.clear();
	}
}


void CGameDlg::OnStnClickedImgBell()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckFive();
	if (m_bWin)
	{
		Win();
		SendGame(SOC_BELL);
	}
}

void CGameDlg::CheckFive()
{
	if ((m_lstMyThrownCard.back().iFruitCnt + m_lstOtherThrownCard.back().iFruitCnt) == 5)
		m_bWin = TRUE;

}

void CGameDlg::Win()
{
	deleteAllMyThrownCard();
	deleteAllOtherThrownCard();
	m_bWin = FALSE;
}