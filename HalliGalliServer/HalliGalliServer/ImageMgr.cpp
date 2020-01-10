#include "stdafx.h"
#include "ImageMgr.h"

IMPLEMENT_SINGLETON(CImageMgr)

CImageMgr::CImageMgr()
{
}

CImageMgr::~CImageMgr()
{
	/* 사용한 이미지 삭제 */
	for (auto& pair : m_mapImage)
		delete pair.second;
	m_mapImage.clear();

	for (int i = 0; i < FRUIT_END; ++i)
	{
		for (auto& pImg : m_vecCardImage[i])
			delete pImg;

		m_vecCardImage[i].shrink_to_fit();
		m_vecCardImage[i].clear();
	}
}

CImage * CImageMgr::GetImage(const char * pTexName)
{
	auto& iter_find = m_mapImage.find(pTexName);

	if (iter_find == m_mapImage.end()) // 찾는 이미지가 없으면 null return
		return nullptr;

	return iter_find->second;
}

CImage * CImageMgr::GetCardImage(const int & iFruitID, const int & iFruitCnt)
{
	// 과일 ID 가 잘못됐으면 null return
	if (iFruitID < 0 || iFruitID >= FRUIT_END)
		return nullptr;
	// 과일 개수가 잘못됐으면 null return
	if (iFruitCnt < 0 || iFruitCnt > 5)
		return nullptr;

	return m_vecCardImage[iFruitID][iFruitCnt - 1];
}

void CImageMgr::Initialize()
{
	Load_Image();
	Load_CardImage("../../Image/Card/");

	AfxMessageBox("이미지 로드 성공");
}

void CImageMgr::Load_Image()
{
	// 하위 폴더 검사 안함
	char szFilePath[MAX_PATH] = "../../Image/";
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

			CImage* pImage = new CImage;
			pImage->Load(find.GetFilePath());

			CString strFileName = find.GetFileTitle(); // 파일명만 온전히 얻어옴
			m_mapImage.emplace(strFileName, pImage);
		}
	}
}

void CImageMgr::Load_CardImage(const char * pPath)
{
	// 하위 폴더 검사
	char szFilePath[MAX_PATH] = "";
	strcpy_s(szFilePath, pPath);
	strcat_s(szFilePath, "\\*.*");

	CFileFind find;
	BOOL bContinue = find.FindFile(szFilePath);
	
	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			Load_CardImage(find.GetFilePath()); // 폴더일 경우 재귀호출
		else
		{
			if(find.IsSystem())
				continue; // 시스템 파일일 경우 넘어감
			
			char szFileName[MAX_PATH] = "";
			strcpy_s(szFileName, find.GetFilePath());

			PathRemoveFileSpec(szFileName); // 파일명과 확장자 제거
			CString strImgName = PathFindFileName(szFileName); // 말단 폴더명

			if (strImgName == "Back")
			{
				CImage* pImage = new CImage;
				pImage->Load(find.GetFilePath());

				m_vecCardImage[FRUIT_BACK].push_back(pImage);
			}
			else if (strImgName == "Cherry")
			{
				CImage* pImage = new CImage;
				pImage->Load(find.GetFilePath());

				m_vecCardImage[FRUIT_CHEERY].push_back(pImage);
			}
			else if (strImgName = "Pear")
			{
				CImage* pImage = new CImage;
				pImage->Load(find.GetFilePath());

				m_vecCardImage[FRUIT_PEAR].push_back(pImage);
			}
		}
	}
}