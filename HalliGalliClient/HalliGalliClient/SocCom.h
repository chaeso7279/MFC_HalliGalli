#ifndef SocCom_h__
#define SocCom_h__

class CSocCom : public CSocket
{
public:
	CSocCom();
	virtual ~CSocCom();

public:
	void Init(HWND hWnd);
	virtual void OnReceive(int nErrorCode);

public:
	HWND m_hWnd;
};

#endif // SocCom_h__