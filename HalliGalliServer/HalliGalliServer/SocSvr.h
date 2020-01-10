#ifndef SocSvr_h__
#define SocSvr_h__



class CSocSvr : public CSocket
{
public:
	CSocSvr();
	virtual ~CSocSvr();

public:
	void Init(HWND hWnd);
	CSocCom* GetAcceptSocCom();
};

#endif // SocSvr_h__