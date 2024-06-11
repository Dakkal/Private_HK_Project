#pragma once
#include "Bmp.h"

class CBmpMgr
{
public:
	CBmpMgr();
	~CBmpMgr();

public:
	void Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC  Find_Image(const TCHAR* pImgKey);
	void Release();

public:
	static	CBmpMgr*	Get_Inst()
	{
		if (!m_pInstance)
			m_pInstance = new CBmpMgr;

		return m_pInstance;
	}
	static	void Destory_Inst()
	{
		if (m_pInstance)
			Safe_Delete<CBmpMgr*>(m_pInstance);
	}

private:
	static CBmpMgr*					m_pInstance;
	map<const TCHAR*, CBmp*>		m_mapBit;
};

