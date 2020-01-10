#ifndef SocSvr_h__
#define SocSvr_h__

#include "SocCom.h"

class CSocSvr : public CSocket
{
public:
	CSocSvr();
	virtual ~CSocSvr();

public:
	void Init(HWND hWnd);
	CSocCom* GetAcceptSocCom();

public:
	virtual void OnAccept(int nErrorCode);

public:
	HWND m_hWnd;

public:
	CSocCom m_socCom;
};

#endif // SocSvr_h__