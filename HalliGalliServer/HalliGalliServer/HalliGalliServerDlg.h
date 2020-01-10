
// HalliGalliServerDlg.h : 헤더 파일
//

#pragma once

#include "SocSvr.h"

// CHalliGalliServerDlg 대화 상자
class CHalliGalliServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CHalliGalliServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HALLIGALLISERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();

public:
	/* 소켓 관련 함수 */
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lParam);
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);

	void InitSocket();

public:
	/* 소켓 관련 변수 */
	CSocSvr m_socServer;
	CSocCom* m_pSocCom = nullptr;
};
