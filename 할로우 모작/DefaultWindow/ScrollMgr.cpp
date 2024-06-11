#include "stdafx.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"

CScrollMgr*		CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Set_PoinScroll(float _X, float _Y)
{
	m_fScrollX = _X;  
	m_fScrollY = _Y;
}

void CScrollMgr::Scroll_Lock()
{

	if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE)
	{
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;

		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCX - STAGE1CX > m_fScrollX)
			m_fScrollX = WINCX - STAGE1CX;

		if (WINCY - STAGE1CY > m_fScrollY)
			m_fScrollY = WINCY - STAGE1CY;
	}
	if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE2)
	{
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;

		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCX - STAGE2CX > m_fScrollX)
			m_fScrollX = WINCX - STAGE2CX;

		if (WINCY - STAGE2CY > m_fScrollY)
			m_fScrollY = WINCY - STAGE2CY;
	}
	if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_BOSS)
	{
		if (0.f < m_fScrollX)
			m_fScrollX = 0.f;

		if (0.f < m_fScrollY)
			m_fScrollY = 0.f;

		if (WINCX - BOSSSTAGECX > m_fScrollX)
			m_fScrollX = WINCX - BOSSSTAGECX;

		if (WINCY - BOSSSTAGECY > m_fScrollY)
			m_fScrollY = WINCY - BOSSSTAGECY;
	}
}
