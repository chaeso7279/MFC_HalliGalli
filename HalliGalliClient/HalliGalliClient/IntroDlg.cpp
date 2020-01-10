// IntroDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "IntroDlg.h"
#include "afxdialogex.h"


// CIntroDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CIntroDlg, CDialogEx)

CIntroDlg::CIntroDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_INTRO, pParent)
{

}

CIntroDlg::~CIntroDlg()
{
}

void CIntroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIntroDlg, CDialogEx)
END_MESSAGE_MAP()


// CIntroDlg 메시지 처리기입니다.
