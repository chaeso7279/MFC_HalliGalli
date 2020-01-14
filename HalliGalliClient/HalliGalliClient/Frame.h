#ifndef Frame_h__
#define Frame_h__

class CFrame
{
private:
	CFrame();
public:
	~CFrame();

public:
	bool Is_PermitCall(const float& fDeltaTime);

public:
	HRESULT Initialize(const float& fCallLimit);

private:
	float m_fCallLimit = 0.f;
	float m_fAccDeltaTime = 0.f;

public:
	static CFrame* Create(const float& fCallLimit);
};

#endif // Frame_h__