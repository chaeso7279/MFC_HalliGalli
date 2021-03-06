
// IntroDlg.h : 헤더 파일
//

#pragma once

#include "Timer.h"
#include "Frame.h"
#include "Rule.h"
// CIntroDlg 대화 상자
class CIntroDlg : public CDialogEx
{
	enum RADIO_INDEX { RADIO_2P, RADIO_4P, RADIO_END };

// 생성입니다.
public:
	CIntroDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTRO_DLG };
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
	afx_msg LPARAM OnKickIdle(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio2p();
	afx_msg void OnBnClickedRadio4p();

	afx_msg void OnBnClickedButtonConnect(); 
	afx_msg void OnClickedButtonHelp();

public:
	void GameDlgEnd();

public:
	CButton m_Radio[RADIO_END];
	CPictureEx m_LogoPicCtrl;

public:
	/* 타이머 관련 변수 */
	CTimer* m_pImmediateTimer = nullptr;
	CTimer* m_p60Timer = nullptr;
	CFrame* m_pFrame = nullptr;
};