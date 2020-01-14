#include "stdafx.h"
#include "Frame.h"


CFrame::CFrame()
{
}

CFrame::~CFrame()
{
}

bool CFrame::Is_PermitCall(const float & fDeltaTime)
{
	m_fAccDeltaTime += fDeltaTime;

	if (m_fAccDeltaTime >= m_fCallLimit)
	{
		m_fAccDeltaTime = 0.f;
		return true;
	}

	return false;
}

HRESULT CFrame::Initialize(const float & fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

CFrame * CFrame::Create(const float & fCallLimit)
{
	CFrame* pInstance = new CFrame;
	if (FAILED(pInstance->Initialize(fCallLimit)))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	
	return pInstance;
}