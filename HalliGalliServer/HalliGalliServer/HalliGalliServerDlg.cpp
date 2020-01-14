
// HalliGalliServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "HalliGalliServer.h"
#include "HalliGalliServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int wcnt = 5;
int ccnt = 5;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHalliGalliServerDlg 대화 상자



CHalliGalliServerDlg::CHalliGalliServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HALLIGALLISERVER_DIALOG, pParent)
	, m_strGain(_T(""))
	, m_strSend(_T(""))
	, m_bTakeCard{}
	, m_strMe(_T("클라이언트 접속 대기중"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHalliGalliServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_IMG_BELL, m_BellPicCtrl);
	DDX_Control(pDX, IDC_IMG_OTHER_OWN, m_CardPicCtrl[USER_OTHER][OWN]);
	DDX_Control(pDX, IDC_IMG_OTHER_THROWN, m_CardPicCtrl[USER_OTHER][THROWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_OWN, m_CardPicCtrl[USER_PLAYER][OWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_THROWN, m_CardPicCtrl[USER_PLAYER][THROWN]);
	DDX_Control(pDX, IDC_EDIT_CARDCOUNTNUM, m_strCardCountNum);
	DDX_Control(pDX, IDC_EDIT_WHOLECOUNTNUM, m_strWholeCountNum);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_STATIC_WHOLECOUNT, m_strWholeCount);
	DDX_Control(pDX, IDC_STATIC_CARDCOUNT, m_strCardCount);
	DDX_Text(pDX, IDC_STATIC_ME, m_strMe);
	DDX_Text(pDX, IDC_STATIC_GAIN, m_strGain);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
}

BEGIN_MESSAGE_MAP(CHalliGalliServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CHalliGalliServerDlg::OnClickedImgPlayerOwn)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CHalliGalliServerDlg::OnClickedButtonSend)
	ON_STN_CLICKED(IDC_IMG_BELL, &CHalliGalliServerDlg::OnClickedImgBell)
END_MESSAGE_MAP()


// CHalliGalliServerDlg 메시지 처리기

BOOL CHalliGalliServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	/* 이미지 관리 매니저 초기화 => Image폴더 내의 이미지 로드함 */
	m_pImgMgr = CImageMgr::GetInstance();
	m_pImgMgr->Initialize();

	/* Picture Ctrl 초기화 */
	InitPicCtrl();

	/* 소켓 초기화 */
	InitSocket();

	/* 타이머 및 프레임 생성 */
	m_pImmediateTimer = CTimer::Create();
	m_p60Timer = CTimer::Create();
	m_pFrame = CFrame::Create(60.f);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHalliGalliServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHalliGalliServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHalliGalliServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CHalliGalliServerDlg::DestroyWindow()
{
	/* 이미지 매니저 삭제 */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	/* 타이머 삭제 */
	if (m_pImmediateTimer)
		delete m_pImmediateTimer;
	if (m_p60Timer)
		delete m_p60Timer;
	if (m_pFrame)
		delete m_pFrame;

	return CDialogEx::DestroyWindow();
}

LPARAM CHalliGalliServerDlg::OnAccept(UINT wParam, LPARAM lParam)
{
	AfxMessageBox("클라이언트 접속");
	//m_strStatus = "접속 성공";

	m_pSocCom = m_socServer.GetAcceptSocCom();
	m_pSocCom->Init(this->m_hWnd);

	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
	UpdateData(FALSE);
	
	// 클라이언트 연결 후 게임 초기화
	m_bConnect = TRUE;
	InitGame();

	return TRUE;
}

LPARAM CHalliGalliServerDlg::OnReceive(UINT wParam, LPARAM lParam)
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

	switch (iType)
	{
	case SOC_GAMESTART:
		m_bStartCnt = TRUE;
		break;
	case SOC_THROWNCARD:
		strFruitID.Format("%c", (pTemp + 1)[0]);
		strFruitCnt.Format("%c", (pTemp + 1)[1]);

		tCard.iFruitID = atoi(strFruitID.GetString());
		tCard.iFruitCnt = atoi(strFruitCnt.GetString());

		AddOtherThrownCard(tCard);
		ChangeCardImage(USER_OTHER, THROWN, tCard);
		ChangeMyTurn(TRUE);
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
		break;
	case SOC_TEXT:
		str.Format("%s", pTemp + 1);
		m_list.AddString("(상대방) : " + str);
		break;
	case SOC_GAMEEND:
		break;
	}

	return TRUE;
}

LRESULT CHalliGalliServerDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	if (m_pImmediateTimer && m_p60Timer && m_pFrame)
	{
		m_pImmediateTimer->Update();

		float fDeltaTime = m_pImmediateTimer->Get_DeltaTime();

		if (m_pFrame->Is_PermitCall(fDeltaTime))
		{
			m_p60Timer->Update();
			float fTime60 = m_p60Timer->Get_DeltaTime();
			g_fDeltaTime = fTime60;
		}

	}

	return (LRESULT)1;
}

void CHalliGalliServerDlg::InitSocket()
{
	/* SocCom, SocServer 초기화 및 생성 함수 */
	m_pSocCom = NULL;
	m_socServer.Create(DEFAULT_PORT);
	m_socServer.Listen();
	m_socServer.Init(this->m_hWnd);
}

void CHalliGalliServerDlg::OnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	SendGame(SOC_TEXT, m_strSend);

	m_list.AddString("  (나) : " + m_strSend);
	m_strSend = (_T(""));
	UpdateData(FALSE);
}

void CHalliGalliServerDlg::SendGame(int iType, CString strTemp)
{
	char pTemp[MAX_STR];

	memset(pTemp, '\0', MAX_STR);
	sprintf_s(pTemp, "%d%s", iType, strTemp.GetString());

	m_pSocCom->Send(pTemp, MAX_STR);
}

void CHalliGalliServerDlg::InitPicCtrl()
{
	ChangeCardImage(USER_PLAYER, OWN);
	ChangeCardImage(USER_OTHER, OWN);

	ChangeCardImage(USER_PLAYER, THROWN, CARD(FRUIT_BACK, 2));
	ChangeCardImage(USER_OTHER, THROWN, CARD(FRUIT_BACK, 2));

	CImage* pImage = nullptr;

	pImage = m_pImgMgr->GetImage("Bell");
	if(pImage != nullptr)
		m_BellPicCtrl.SetBitmap(*pImage);
}

void CHalliGalliServerDlg::ChangeCardImage(const USER_ID & eID, const CARD_STATUS & eStatus, const CARD & tCard)
{
	CImage* pImage = m_pImgMgr->GetCardImage(tCard.iFruitID, tCard.iFruitCnt);

	if (pImage != nullptr)
	{
		m_CardPicCtrl[eID][eStatus].SetBitmap(*pImage);
		Invalidate(TRUE);
	}
		
}

void CHalliGalliServerDlg::InitCardDeck()
{
	// 1:체리 2:배 카드 덱 초기화
	int nIndex = 0;
	for (int i = 1; i <= 2; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			if (j == 1)
			{
				for (int k = 0; k < 5; k++)
				{
					m_cardDec[nIndex].iFruitID = i;
					m_cardDec[nIndex].iFruitCnt = j;
					nIndex++;
				}
			}
			else if (j == 4)
			{
				for (int k = 0; k < 2; k++)
				{
					m_cardDec[nIndex].iFruitID = i;
					m_cardDec[nIndex].iFruitCnt = j;
					nIndex++;
				}
			}
			else if (j == 5)
			{
				m_cardDec[nIndex].iFruitID = i;
				m_cardDec[nIndex].iFruitCnt = j;
				nIndex++;
			}
			else
			{
				for (int k = 0; k < 3; k++)
				{
					m_cardDec[nIndex].iFruitID = i;
					m_cardDec[nIndex].iFruitCnt = j;
					nIndex++;
				}
			}

		}
	}

	// 인덱스 셔플 
	for (int i = 0; i < CARD_CNT; i++)
		m_shuffleCardIndex[i] = i;

	int nRandomIndex1 = 0;
	int nRandomIndex2 = 0;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < 84; i++)
	{
		nRandomIndex1 = rand() % CARD_CNT;
		nRandomIndex2 = rand() % CARD_CNT;

		int nTemp = 0;
		nTemp = m_shuffleCardIndex[nRandomIndex1];
		m_shuffleCardIndex[nRandomIndex1] = m_shuffleCardIndex[nRandomIndex2];
		m_shuffleCardIndex[nRandomIndex2] = nTemp;
	}
}

void CHalliGalliServerDlg::InitGame()
{
	if (!m_bConnect)
		return;

	/* 카드 덱 초기화 및 섞기 */
	InitCardDeck();
	/* 클라이언트에 카드 보냄 */
	SendCardToClient();

	/* 서버 먼저 턴 */
	ChangeMyTurn(TRUE);
}

void CHalliGalliServerDlg::SendCardToClient()
{
	for (int i = 0; i < CARD_HALF_CNT; ++i)
	{
		int iSuffleIndex = m_shuffleCardIndex[i];
		CARD tCard = m_cardDec[iSuffleIndex];

		char pCardInfo[MID_STR] = "";

		sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);

		SendGame(SOC_INITGAME, pCardInfo);
	}

	/* 클라에 카드 보낸 후 내 카드 저장 */
	for (int i = CARD_HALF_CNT; i < CARD_CNT; ++i)
	{
		int iSuffleIndex = m_shuffleCardIndex[i];
		CARD tCard = m_cardDec[iSuffleIndex];

		m_lstMyCard.push_back(tCard);
	}

	/* 카드를 다 보내고 나면 GAMESTART 메시지를 클라에 보냄 */
	SendGame(SOC_GAMESTART);
}

void CHalliGalliServerDlg::CheckThrownCard()
{
	int iMyFruitCnt = 0;
	int iOtherFruitCnt = 0;

	if (!m_lstMyThrownCard.empty())
		iMyFruitCnt = m_lstMyThrownCard.back().iFruitCnt;
	if (!m_lstOtherThrownCard.empty())
		iOtherFruitCnt = m_lstOtherThrownCard.back().iFruitCnt;
	
	if (iMyFruitCnt + iOtherFruitCnt == 5)
		m_bTakeCard = TRUE;
	else
		m_bTakeCard = FALSE;
}

void CHalliGalliServerDlg::TakeThrownCard()
{
	DeleteAllMyThrownCard();
	DeleteAllOtherThrownCard();
}

void CHalliGalliServerDlg::AddMyThrownCard(const CARD sCard)
{
	m_lstMyThrownCard.push_back(sCard);
}

void CHalliGalliServerDlg::AddOtherThrownCard(const CARD sCard)
{
	m_lstOtherThrownCard.push_back(sCard);
}

void CHalliGalliServerDlg::DeleteAllMyThrownCard()
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

void CHalliGalliServerDlg::DeleteAllOtherThrownCard()
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

void CHalliGalliServerDlg::ChangeMyTurn(BOOL bMyTurn)
{
	m_bMyTurn = bMyTurn;
	if (m_bMyTurn)
		m_strMe = "당신의 차례입니다";
	else
		m_strMe = "상대방의 차례입니다";

	GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(m_bMyTurn);

	UpdateData(FALSE);
}

BOOL CHalliGalliServerDlg::IsGameEnd()
{
	/* 게임이 끝났는지 검사하는 함수 */
	//if(m_lstMyCard.size() <= 0)


	return 0;
}

void CHalliGalliServerDlg::OnClickedImgPlayerOwn()
{
	if (!m_bConnect || !m_bStartCnt || !m_bMyTurn)
		return;

	/* 카드 내기 */
	SetDlgItemInt(IDC_EDIT_WHOLECOUNTNUM, wcnt);
	SetDlgItemInt(IDC_EDIT_CARDCOUNTNUM, ccnt);
	
	CARD tCard;

	tCard.iFruitID = m_lstMyCard.back().iFruitID;
	tCard.iFruitCnt = m_lstMyCard.back().iFruitCnt;
	
	AddMyThrownCard(tCard);
	ChangeCardImage(USER_PLAYER, THROWN, tCard);
	
	m_lstMyCard.pop_back();

	/* 클라에 낸 카드 전달 */
	char pCardInfo[MID_STR] = "";
	sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);
	SendGame(SOC_THROWNCARD, pCardInfo);

	/* 턴 변경 */
	ChangeMyTurn(FALSE);

	wcnt--;
	ccnt--;
}

void CHalliGalliServerDlg::OnClickedImgBell()
{
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
		SendGame(SOC_NOTAKECARD); // 상대에게 벨을 잘못 때렸음을 알림
	}
}