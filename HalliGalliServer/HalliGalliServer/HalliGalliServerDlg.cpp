
// HalliGalliServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "HalliGalliServer.h"
#include "HalliGalliServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
}

BEGIN_MESSAGE_MAP(CHalliGalliServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CHalliGalliServerDlg::OnClickedImgPlayerOwn)
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	/* 이미지 매니저 삭제 */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	return CDialogEx::DestroyWindow();
}

LPARAM CHalliGalliServerDlg::OnAccept(UINT wParam, LPARAM lParam)
{
	AfxMessageBox("클라이언트 접속");
	//m_strStatus = "접속 성공";

	m_pSocCom = m_socServer.GetAcceptSocCom();
	m_pSocCom->Init(this->m_hWnd);

	//UpdateData(FALSE);

	// 클라이언트 연결 후 게임 초기화
	m_bConnect = TRUE;
	InitGame();

	return TRUE;
}

LPARAM CHalliGalliServerDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	char pTemp[MAX_STR];
	CString strTemp;

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
		GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(TRUE);
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

void CHalliGalliServerDlg::InitSocket()
{
	/* SocCom, SocServer 초기화 및 생성 함수 */
	m_pSocCom = NULL;
	m_socServer.Create(DEFAULT_PORT);
	m_socServer.Listen();
	m_socServer.Init(this->m_hWnd);
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

	CImage* pImage = nullptr;

	pImage = m_pImgMgr->GetImage("Bell");
	if(pImage != nullptr)
		m_BellPicCtrl.SetBitmap(*pImage);
}

void CHalliGalliServerDlg::ChangeCardImage(const USER_ID & eID, const CARD_STATUS & eStatus, const CARD & tCard)
{
	CImage* pImage = m_pImgMgr->GetCardImage(tCard.iFruitID, tCard.iFruitCnt);

	if (pImage != nullptr)
		m_CardPicCtrl[eID][eStatus].SetBitmap(*pImage);
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

void CHalliGalliServerDlg::InitGame()
{
	if (!m_bConnect)
		return;

	/* 카드 덱 초기화 및 섞기 */
	InitCardDeck();
	/* 클라이언트에 카드 보냄 */
	SendCardToClient();
}

void CHalliGalliServerDlg::CheckFive()
{
	if ((m_lstMyThrownCard.back().iFruitCnt + m_lstOtherThrownCard.back().iFruitCnt) == 5)
		m_bWin = TRUE;
}

void CHalliGalliServerDlg::Win()
{
	DeleteAllMyThrownCard();
	DeleteAllOtherThrownCard();
	m_bWin = FALSE;
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

void CHalliGalliServerDlg::DeleteAllOtherThrownCard()
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

void CHalliGalliServerDlg::OnClickedImgPlayerOwn()
{
	/* 카드 내기 */
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

	GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(FALSE);
}