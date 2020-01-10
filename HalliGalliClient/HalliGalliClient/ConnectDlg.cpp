// ConnectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "ConnectDlg.h"
#include "afxdialogex.h"


// CConnectDlg 대화 상자

IMPLEMENT_DYNAMIC(CConnectDlg, CDialogEx)

CConnectDlg::CConnectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONNET_DLG, pParent)
	, m_strIP(_T(DEFAULT_IP))
{

}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialogEx)
END_MESSAGE_MAP()


// CConnectDlg 메시지 처리기
