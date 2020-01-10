#include "stdafx.h"
#include "SocSvr.h"

CSocSvr::CSocSvr()
{
}

CSocSvr::~CSocSvr()
{
}

void CSocSvr::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

CSocCom * CSocSvr::GetAcceptSocCom()
{
	return &m_socCom;
}

void CSocSvr::OnAccept(int nErrorCode)
{
	// 클라가 접속하면 메인 윈도우에 UM_ACCEPT 메시지 전달
	Accept(m_socCom);
	SendMessage(m_hWnd, UM_ACCEPT, 0, 0);

	CSocket::OnAccept(nErrorCode);
}