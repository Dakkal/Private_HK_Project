#include "stdafx.h"
#include "Obstacle.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

CObstacle::CObstacle()
{
}


CObstacle::~CObstacle()
{
	Release();
}

void CObstacle::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_OBS;

	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 200.f;

	m_tBmp.x = 150;
	m_tBmp.y = 200;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Obstacle/L_SmallDoor.bmp", L"L_SmallDoor");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Obstacle/R_SmallDoor.bmp", L"R_SmallDoor");

	if (m_eLRDir == DIR_TYPE::DIR_LEFT)
	{
		m_pFrameKey = L"L_SmallDoor";
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
	else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
	{
		m_pFrameKey = L"R_SmallDoor";
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
}

int CObstacle::Update(void)
{
	



	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CObstacle::Late_Update()
{

	__super::Move_Frame();
	Set_Motion();
}

void CObstacle::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		int(m_tBmpRect.left + fScrollX),
		int(m_tBmpRect.top + fScrollY),
		(int)m_tBmp.x,
		(int)m_tBmp.y,
		hMemDC,
		m_tFrame.iFrameStart * (int)m_tBmp.x,
		m_tFrame.iMotion * (int)m_tBmp.y,
		(int)m_tBmp.x,
		(int)m_tBmp.y,
		RGB(100, 100, 100));
}

void CObstacle::Release(void)
{
}

void CObstacle::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
}

void CObstacle::Tile_Collision()
{
}

void CObstacle::Set_Motion()
{
}

void CObstacle::OnCollision(CObj * _pObj, RECT _rc)
{
}
