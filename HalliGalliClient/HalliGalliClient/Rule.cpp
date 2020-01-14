// Rule.cpp: 구현 파일
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "Rule.h"
#include "afxdialogex.h"



// CRule 대화 상자

IMPLEMENT_DYNAMIC(CRule, CDialogEx)

CRule::CRule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RULE_DLG, pParent)
{
	

}

CRule::~CRule()
{
}

void CRule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMG_HELP, m_HelpPicCtrl);
}


BEGIN_MESSAGE_MAP(CRule, CDialogEx)
END_MESSAGE_MAP()


// CRule 메시지 처리기


BOOL CRule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	/* 이미지 관리 매니저 초기화 => Image폴더 내의 이미지 로드함 */
	CImage* pImage = CImageMgr::GetInstance()->GetImage("rule");

	if(pImage)
		m_HelpPicCtrl.SetBitmap(*pImage);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}