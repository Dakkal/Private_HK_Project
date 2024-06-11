#include "stdafx.h"
#include "MenuUI.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Effect_Player.h"
#include "SoundMgr.h"


CMenuUI::CMenuUI()
{
}


CMenuUI::~CMenuUI()
{
}

void CMenuUI::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_UI;

	m_tInfo.fCX = 99.f;
	m_tInfo.fCY = 72.f;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Menu/R_Menu_Poniter.bmp", L"R_Menu_Poniter");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Menu/L_Menu_Poniter.bmp", L"L_Menu_Poniter");

	if (m_eLRDir == DIR_TYPE::DIR_LEFT)
	{
		m_pFrameKey = L"L_Menu_Poniter";
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 10;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 30;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
	else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
	{
		m_pFrameKey = L"R_Menu_Poniter";
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 10;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 30;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
}

int CMenuUI::Update(void)
{
	__super::Update_Rect();

	return 0;
}

void CMenuUI::Late_Update()
{

	__super::Move_Frame();
	Set_Motion();
}

void CMenuUI::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(hDC,
			int(m_tRect.left),
			int(m_tRect.top),
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iFrameStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			RGB(0, 0, 0));
}

void CMenuUI::Release(void)
{
}

void CMenuUI::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
}

void CMenuUI::Tile_Collision()
{
}

void CMenuUI::Set_Motion()
{
}

void CMenuUI::OnCollision(CObj * _pObj, RECT _rc)
{
}
