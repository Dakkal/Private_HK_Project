#include "stdafx.h"
#include "Pattern1_Coli.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"

CPattern1_Coli::CPattern1_Coli()
	: m_fDistance(-1)
{
}


CPattern1_Coli::~CPattern1_Coli()
{
	Release();
}

void CPattern1_Coli::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_ENEMYCOLLI;
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Boss/R_Brase.bmp", L"R_Brase");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Boss/L_Brase.bmp", L"L_Brase");

	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_BOSS).front());

	m_eLRDir = Get_Target()->Get_LRDirType();

	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 50.f;
	m_tStat.iDamage = 2;

	m_fSpeed = 0.f;

	if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
	{
		m_tBmp.x = 520.f;
		m_tBmp.y = 300.f;

		m_pFrameKey = L"R_Brase";
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 50;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}

	else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
	{
		m_tBmp.x = 433.f;
		m_tBmp.y = 300.f;

		m_pFrameKey = L"L_Brase";
		m_tFrame.iFrameStart = 1;
		m_iCurFrame = 1;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 50;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
}

int CPattern1_Coli::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_fDistance == -1 && m_eLRDir == DIR_TYPE::DIR_RIGHT)
		m_fDistance = (m_tInfo.fX + 1000.f);

	else if (m_fDistance == -1 && m_eLRDir == DIR_TYPE::DIR_LEFT)
		m_fDistance = (m_tInfo.fX - 1000.f);
		


	if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
		m_tInfo.fX += m_fSpeed;

	else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
		m_tInfo.fX -= m_fSpeed;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPattern1_Coli::Late_Update()
{
	m_fSpeed += 1.f;

	if (m_tInfo.fX >= m_fDistance && m_eLRDir == DIR_TYPE::DIR_RIGHT)
		m_bDead = true;
		
	else if (m_tInfo.fX <= m_fDistance && m_eLRDir == DIR_TYPE::DIR_LEFT)
		m_bDead = true;
		

	__super::Move_Frame();

}

void CPattern1_Coli::Render(HDC hDC)
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

void CPattern1_Coli::Release(void)
{
}

void CPattern1_Coli::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
}

void CPattern1_Coli::Tile_Collision()
{
}

void CPattern1_Coli::Set_Motion()
{
}

void CPattern1_Coli::OnCollision(CObj * _pObj, RECT _rc)
{
}
