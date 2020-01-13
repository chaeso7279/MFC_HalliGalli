#pragma once
class CPictureEx : public CStatic
{
	DECLARE_DYNAMIC(CPictureEx)

public:
	COLORREF Transparent;

public:
	CPictureEx();
	virtual ~CPictureEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};