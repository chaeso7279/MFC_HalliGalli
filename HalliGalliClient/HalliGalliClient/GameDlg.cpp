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
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_STN_CLICKED(IDC_STATIC_GAIN, &CGameDlg::OnStnClickedStaticGain)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CGameDlg::OnClieckedImgPlayerOwn)
	ON_STN_CLICKED(IDC_IMG_OTHER_OWN, &CGameDlg::OnClickedImgOtherOwn)
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
	static bool bTemp = false;
	CARD tCard;
	if (bTemp)
	{
		tCard.iFruitID = FRUIT_CHEERY;
		tCard.iFruitCnt = 1;
		ChangeCardImage(USER_PLAYER, THROWN, tCard);
	}
	else
	{
		tCard.iFruitID = FRUIT_PEAR;
		tCard.iFruitCnt = 5;
		ChangeCardImage(USER_PLAYER, THROWN, tCard);
	}

	bTemp = !bTemp;
}


void CGameDlg::OnClickedImgOtherOwn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static bool bTemp2 = false;
	CARD tCard;
	if (bTemp2)
	{
		tCard.iFruitID = FRUIT_PEAR;
		tCard.iFruitCnt = 3;
		ChangeCardImage(USER_OTHER, THROWN, tCard);
	}
	else
	{
		tCard.iFruitID = FRUIT_CHEERY;
		tCard.iFruitCnt = 2;
		ChangeCardImage(USER_OTHER, THROWN, tCard);
	}

	bTemp2 = !bTemp2;
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