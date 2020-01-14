#ifndef Timer_h__
#define Timer_h__

class CTimer
{
private:
	CTimer();
public:
	~CTimer();

public:
	float Get_DeltaTime() { return m_fDeltaTime; }

public:
	HRESULT Initialize(void);
	void Update(void);

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

private:
	float					m_fDeltaTime = 0.f;

public:
	static CTimer* Create();
};

#endif // Timer_h__