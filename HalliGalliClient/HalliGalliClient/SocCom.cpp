#include "stdafx.h"
#include "SocCom.h"


CSocCom::CSocCom()
{
}

CSocCom::~CSocCom()
{
}

void CSocCom::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CSocCom::OnReceive(int nErrorCode)
{
	SendMessage(m_hWnd, UM_RECEIVE, 0, 0);
	CSocket::OnReceive(nErrorCode);
}