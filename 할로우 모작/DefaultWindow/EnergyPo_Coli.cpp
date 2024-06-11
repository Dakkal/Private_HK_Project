#include "stdafx.h"
#include "EnergyPo_Coli.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"
#include "Effect_Player.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"
#include "Player.h"


CEnergyPo_Coli::CEnergyPo_Coli()
{
}


CEnergyPo_Coli::~CEnergyPo_Coli()
{
	Release();
}

void CEnergyPo_Coli::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_PROJECTILE;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/L_EnergyPo.bmp", L"L_EnergyPo");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/R_EnergyPo.bmp", L"R_EnergyPo");

	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	m_eLRDir = Get_Target()->Get_LRDirType();

	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 80.f;
	m_tStat.iDamage = 30;

	m_tBmp.x = 280.f;
	m_tBmp.y = 150.f;

	m_fSpeed = 0.f;
	m_fDistance = -1.f;

	if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
	{
		m_pFrameKey = L"R_EnergyPo";
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 2;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
		
	else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
	{
		m_pFrameKey = L"L_EnergyPo";
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 2;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
		
}

int CEnergyPo_Coli::Update(void)
{
	if (m_bDead)
	{
		if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, m_tInfo.fY, EFFECT_PLAYER::R_Soul));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, m_tInfo.fY, EFFECT_PLAYER::L_Soul));
		return OBJ_DEAD;
	}
		

	if (m_fDistance == -1 && m_eLRDir == DIR_TYPE::DIR_RIGHT)
		m_fDistance = m_tInfo.fX + 1000.f;
	else if (m_fDistance == -1 && m_eLRDir == DIR_TYPE::DIR_LEFT)
		m_fDistance = m_tInfo.fX - 1000.f;


	if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
		m_tInfo.fX += m_fSpeed;


	else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
		m_tInfo.fX -= m_fSpeed;
		




	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CEnergyPo_Coli::Late_Update()
{
	m_fSpeed += 3.f;

	if (m_tInfo.fX >= m_fDistance && m_eLRDir == DIR_TYPE::DIR_RIGHT)
		m_bDead = true;

	else if (m_tInfo.fX <= m_fDistance && m_eLRDir == DIR_TYPE::DIR_LEFT)
		m_bDead = true;

	Tile_Collision();

	__super::Move_Frame();
	Set_Motion();

	
}

void CEnergyPo_Coli::Render(HDC hDC)
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

void CEnergyPo_Coli::Release(void)
{
}

void CEnergyPo_Coli::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}

void CEnergyPo_Coli::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	switch (eDir)
	{
	case COL_DIR::LEFT:
		m_bDead = true;
		CSoundMgr::Get_Inst()->StopSound(ENERGYSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"EnergyPo_Explosion.wav", ENERGYSOUND_EFFECT, 0.8f);
		break;
	case COL_DIR::TOP:
		break;
	case COL_DIR::RIGHT:
		m_bDead = true;
		CSoundMgr::Get_Inst()->StopSound(ENERGYSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"EnergyPo_Explosion.wav", ENERGYSOUND_EFFECT, 0.8f);
		break;
	case COL_DIR::BOTTOM:
		break;
	}
}

void CEnergyPo_Coli::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_MONSTER:
		if (_pObj->Get_Dead() != true)
		{
			Set_Dead();
			static_cast<CPlayer*>(Get_Target())->On_CameraShaking();
			CSoundMgr::Get_Inst()->StopSound(ENERGYSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"EnergyPo_Explosion.wav", ENERGYSOUND_EFFECT, 0.8f);
		}
		
		break;
	case OBJECT_TYPE::OBJ_BOSS:
		Set_Dead();
		static_cast<CPlayer*>(Get_Target())->On_CameraShaking();
		CSoundMgr::Get_Inst()->StopSound(ENERGYSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"EnergyPo_Explosion.wav", ENERGYSOUND_EFFECT, 0.8f);
		break;
	default:
		break;
	}
}

void CEnergyPo_Coli::Set_Motion()
{
}
