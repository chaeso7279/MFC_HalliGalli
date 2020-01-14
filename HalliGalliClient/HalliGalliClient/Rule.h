#pragma once


// CRule 대화 상자
#include "PictureEx.h"
class CRule : public CDialogEx
{
	DECLARE_DYNAMIC(CRule)

public:
	CRule(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRule();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RULE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CPictureEx m_HelpPicCtrl;
};