#include "stdafx.h"
#include "MonKnighht_Coli.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"
#include "Effect_Player.h"
#include "AbstractFactory.h"
#include "Monster_Knight.h"

CMonKnighht_Coli::CMonKnighht_Coli()
{
}


CMonKnighht_Coli::~CMonKnighht_Coli()
{
	Release();
}

void CMonKnighht_Coli::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_ENEMYCOLLI;

	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	m_tStat.iDamage = 1;

	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 30.f;
	m_dwTime = GetTickCount();

}

int CMonKnighht_Coli::Update(void)
{
	if (m_dwTime + 400 > GetTickCount() && m_bDead)
	{
		static_cast<CMonster_Knight*>(m_pOwner)->IsParring();
		return OBJ_DEAD;
	}
	else if (m_dwTime + 400 <= GetTickCount())
	{
		return OBJ_DEAD;
	}
		

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMonKnighht_Coli::Late_Update()
{
	

}

void CMonKnighht_Coli::Render(HDC hDC)
{
	/*float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	Rectangle(hDC,
	(int)m_tRect.left + fScrollX,
	(int)m_tRect.top + fScrollY,
	(int)m_tRect.right + fScrollX,
	(int)m_tRect.bottom + fScrollY);*/
}

void CMonKnighht_Coli::Release(void)
{
}

void CMonKnighht_Coli::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
}

void CMonKnighht_Coli::Tile_Collision()
{
}

void CMonKnighht_Coli::Set_Motion()
{
}

void CMonKnighht_Coli::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_COLLI:
		Set_Dead();
		break;
	default:
		break;
	}
}
