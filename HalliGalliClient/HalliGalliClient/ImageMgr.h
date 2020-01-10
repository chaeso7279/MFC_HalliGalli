#ifndef ImageMgr_h__
#define ImageMgr_h__

class CImageMgr
{
	DECLARE_SINGLETON(CImageMgr)

private:
	CImageMgr();
	~CImageMgr();

public:
	CImage* GetImage(const char* pTexName);
	CImage* GetCardImage(const int& iFruitID, const int& iFruitCnt = 1);

public:
	void Initialize();

private:
	void Load_Image();
	void Load_CardImage(const char* pPath);

private:
	map<const CString, CImage*> m_mapImage; // 카드이미지 제외한 이미지
	vector<CImage*> m_vecCardImage[FRUIT_END]; // 카드 이미지 vector
};

#endif // ImageMgr_h__