// IntroDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "IntroDlg.h"
#include "afxdialogex.h"

#include "ConnectDlg.h"
#include "GameDlg.h"
#include "Rule.h"

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
public:
	virtual BOOL DestroyWindow();
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


// CIntroDlg 대화 상자



CIntroDlg::CIntroDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INTRO_DLG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIntroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_2P, m_Radio[RADIO_2P]);
	DDX_Control(pDX, IDC_RADIO_4P, m_Radio[RADIO_4P]);
}

BEGIN_MESSAGE_MAP(CIntroDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_2P, &CIntroDlg::OnBnClickedRadio2p)
	ON_BN_CLICKED(IDC_RADIO_4P, &CIntroDlg::OnBnClickedRadio4p)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CIntroDlg::OnBnClickedButtonConnect)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CIntroDlg::OnClickedButtonHelp)
END_MESSAGE_MAP()


// CIntroDlg 메시지 처리기

BOOL CIntroDlg::OnInitDialog()
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
	/* 라디오 버튼 2인용이 기본으로 선택돼있게 초기화 
	   둘 다 동시에 체크될 수 없음					*/
	m_Radio[RADIO_2P].SetCheck(TRUE);
	m_Radio[RADIO_4P].SetCheck(FALSE);

	/* 타이머 및 프레임 생성 */
	m_pImmediateTimer = CTimer::Create();
	m_p60Timer = CTimer::Create();
	m_pFrame = CFrame::Create(60.f);

	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CIntroDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIntroDlg::OnPaint()
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
HCURSOR CIntroDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CAboutDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CDialogEx::DestroyWindow();
}

LRESULT CIntroDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
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

void CIntroDlg::OnBnClickedRadio2p()
{
	/* 2인용 체크 시 들어오는 함수 */
}

void CIntroDlg::OnBnClickedRadio4p()
{
	/* 4인용 체크 시 들어오는 함수 */
	AfxMessageBox("추후 개발 예정입니다^_^");

	/* 4인용 체크해도 무시하도록 설정 */
	m_Radio[RADIO_2P].SetCheck(TRUE);
	m_Radio[RADIO_4P].SetCheck(FALSE);
}

void CIntroDlg::OnBnClickedButtonConnect()
{
	/* 접속 버튼 클릭 시 들어오는 함수 */
	CConnectDlg* pConnectDlg = new CConnectDlg;

	if (pConnectDlg->DoModal() == IDOK)
	{
		//pConnectDlg->UpdateData(TRUE);

		CSocCom* pSocCom = new CSocCom;
		pSocCom->Create();

		BOOL bResult = pSocCom->Connect(pConnectDlg->m_strIP, DEFAULT_PORT);
		if (bResult)
		{
			/* 연결 성공시 GameDlg 로 전환 */
			CGameDlg* pGameDlg = new CGameDlg;
			pGameDlg->Create(IDD_GAME_DLG);

			/* 소켓 포인터 전달 */
			pGameDlg->InitSocket(pSocCom);

			pGameDlg->ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
		else
		{
			/* 연결 실패 */
			AfxMessageBox("접속 실패");
		}
	}
}

void CIntroDlg::GameDlgEnd()
{
	/* 타이머 삭제 */
	if (m_pImmediateTimer)
		delete m_pImmediateTimer;
	if (m_p60Timer)
		delete m_p60Timer;
	if (m_pFrame)
		delete m_pFrame;

	SendMessage(WM_CLOSE);



}


void CIntroDlg::OnClickedButtonHelp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_rulerule = new CRule();
	m_rulerule->Create(IDD_RULE,this);
	m_rulerule->ShowWindow(SW_SHOW);
}
