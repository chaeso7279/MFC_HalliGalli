// GameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "GameDlg.h"
#include "afxdialogex.h"


// CGameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DLG, pParent)
{

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
END_MESSAGE_MAP()


// CGameDlg 메시지 처리기입니다.
