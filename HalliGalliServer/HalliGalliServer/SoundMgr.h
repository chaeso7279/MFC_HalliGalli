#ifndef SoundMgr_h__
#define SoundMgr_h__

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	enum CHANNEL { CH_CARD, CH_BELL, CH_ETC, CH_END };

private:
	CSoundMgr();
	~CSoundMgr();

public:
	HRESULT Initialize();
	void UpdateSound();

	void PlaySound(const char* pSoundKey, CHANNEL eID);
	
private:
	HRESULT LoadSoundFile();

private:
	FMOD_SYSTEM* m_pSystem;
	FMOD_CHANNEL* m_pChannel[CH_END];
	map<const CString, FMOD_SOUND*> m_mapSound;
};

#endif // SoundMgr_h__
