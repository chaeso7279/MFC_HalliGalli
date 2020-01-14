#include "stdafx.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
	for (auto& pair : m_mapSound)
		FMOD_Sound_Release(pair.second);
	
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

HRESULT CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, CH_END, FMOD_INIT_NORMAL, nullptr);

	return LoadSoundFile();
}

void CSoundMgr::UpdateSound()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySound(const char * pSoundKey, CHANNEL eID)
{
	auto& iter_find = m_mapSound.find(pSoundKey);
	if(iter_find != m_mapSound.end())
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, FALSE, &m_pChannel[eID]);
}

HRESULT CSoundMgr::LoadSoundFile()
{
	char szFilePath[MAX_PATH] = "../../Sound/";
	strcat_s(szFilePath, "\\*.*");

	CFileFind find;
	BOOL bContinue = find.FindFile(szFilePath);

	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			continue;
		else
		{
			if (find.IsSystem())
				continue; // 시스템 파일일 경우 넘어감

			FMOD_SOUND* pSound = nullptr;
			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, find.GetFilePath(), FMOD_HARDWARE, nullptr, &pSound);

			if (eRes == FMOD_OK)
			{
				CString strFileName = find.GetFileTitle(); // 파일명만 온전히 얻어옴
				m_mapSound.emplace(strFileName, pSound);
			}
		}
	}

	find.Close();
	FMOD_System_Update(m_pSystem);

	return S_OK;
}