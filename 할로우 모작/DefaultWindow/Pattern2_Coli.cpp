#include "stdafx.h"
#include "Pattern2_Coli.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CPattern2_Coli::CPattern2_Coli()
{
}


CPattern2_Coli::~CPattern2_Coli()
{
	Release();
}

void CPattern2_Coli::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_ENEMYPROJECTILE;

	m_tStat.iDamage = 1.f;

	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 80.f;
	m_tStat.iDamage = 2;

	m_tBmp.x = 650;
	m_tBmp.y = 390;

	m_fSpeed = 8.f;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Boss/BossColi.bmp", L"BossColi");
	m_pFrameKey = L"BossColi";
	m_tFrame.iFrameStart = 0;
	m_iCurFrame = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.bStopMotion = false;

}

int CPattern2_Coli::Update(void)
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}
		

	m_tInfo.fY += m_fSpeed;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPattern2_Coli::Late_Update()
{

	__super::Move_Frame();
	Set_Motion();

	Tile_Collision();
}

void CPattern2_Coli::Render(HDC hDC)
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

void CPattern2_Coli::Release(void)
{
}

void CPattern2_Coli::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}

void CPattern2_Coli::Set_Motion()
{
}

void CPattern2_Coli::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	switch (eDir)
	{
	case COL_DIR::LEFT:
		break;
	case COL_DIR::TOP:
		m_bDead = true;
		break;
	case COL_DIR::RIGHT:
		break;
	case COL_DIR::BOTTOM:
		break;
	}
}

void CPattern2_Coli::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_COLLI:
		Set_Dead();
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword_Death.wav", SWORDSOUND_EFFECT, 1.0f);
		break;
	default:
		break;
	}
	
}
