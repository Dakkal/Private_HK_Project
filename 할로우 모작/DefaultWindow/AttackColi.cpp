#include "stdafx.h"
#include "AttackColi.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "Effect_Player.h"
#include "AbstractFactory.h"
#include "SoundMgr.h"


CAttackColi::CAttackColi()
	: m_bIsParring(false), m_pOwner(nullptr)
{
}


CAttackColi::~CAttackColi()
{
	Release();
}

void CAttackColi::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_COLLI;
	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/RL_Attack.bmp", L"RL_Attack");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/UD_Attack.bmp", L"UD_Attack");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/D_Attack.bmp", L"D_Attack");

	m_tStat.iDamage = 1;

	

	m_eLRDir = Get_Target()->Get_LRDirType();

	if (Get_Target()->Get_UDDirType() == DIR_TYPE::DIR_END)
	{
		m_eUDDir = DIR_TYPE::DIR_END;
		m_pFrameKey = L"RL_Attack";
		m_tBmp.x = 200.f;
		m_tBmp.y = 100.f;
		m_tInfo.fCX = 200.f;
		m_tInfo.fCY = 50.f;
	}
	else if (Get_Target()->Get_UDDirType() == DIR_TYPE::DIR_UP)
	{
		m_eUDDir = Get_Target()->Get_UDDirType();
		m_pFrameKey = L"UD_Attack";
		m_tBmp.x = 100.f;
		m_tBmp.y = 120.f;
		m_tInfo.fCX = 100.f;
		m_tInfo.fCY = 120.f;
	}
	else if (Get_Target()->Get_UDDirType() == DIR_TYPE::DIR_DOWN)
	{
		m_eUDDir = Get_Target()->Get_UDDirType();
		m_pFrameKey = L"D_Attack";
		m_tBmp.x = 100.f;
		m_tBmp.y = 129.f;
		m_tInfo.fCX = 100.f;
		m_tInfo.fCY = 129.f;
	}

	if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}

	else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}

	else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}

	else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
	{
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}

	else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN)
	{
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}

	else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN)
	{
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 1;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 80;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
}

int CAttackColi::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

		
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CAttackColi::Late_Update()
{
	if (m_tFrame.iFrameEnd == 1)
			m_bDead = true;

		

	__super::Move_Frame();
	Set_Motion();
}

void CAttackColi::Render(HDC hDC)
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

void CAttackColi::Release(void)
{
}

void CAttackColi::Tile_Collision()
{
}

void CAttackColi::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
}

void CAttackColi::Set_Motion()
{
	
}

void CAttackColi::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_MONSTER:
		if (_pObj->Get_Dead() != true)
		{
			static_cast<CPlayer*>(m_pTarget)->Hit_Count();
			static_cast<CPlayer*>(m_pTarget)->On_CameraShaking();
			if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.left, m_tInfo.fY, EFFECT_PLAYER::R_ATTACK));
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.right, m_tInfo.fY, EFFECT_PLAYER::L_ATTACK));
			else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UR_ATTACK));
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UL_ATTACK));
			else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			{
				static_cast<CPlayer*>(m_pOwner)->Is_ParringJump();
				static_cast<CPlayer*>(m_pTarget)->Off_CameraShaking();
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DR_ATTACK));
			}

			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			{
				static_cast<CPlayer*>(m_pOwner)->Is_ParringJump();
				static_cast<CPlayer*>(m_pTarget)->Off_CameraShaking();
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DL_ATTACK));
			}
		}
		break;
	case OBJECT_TYPE::OBJ_BOSS:
		static_cast<CPlayer*>(m_pTarget)->Hit_Count();
		static_cast<CPlayer*>(m_pTarget)->On_CameraShaking();
		if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.left, m_tInfo.fY, EFFECT_PLAYER::R_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.right, m_tInfo.fY, EFFECT_PLAYER::L_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UR_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UL_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN)
		{
			static_cast<CPlayer*>(m_pOwner)->Is_ParringJump();
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DR_ATTACK));
		}

		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN)
		{
			static_cast<CPlayer*>(m_pOwner)->Is_ParringJump();
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DL_ATTACK));
		}
		break;
	case OBJECT_TYPE::OBJ_ENEMYCOLLI:
		static_cast<CPlayer*>(m_pTarget)->Parring_Count();
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Parry.wav", SWORDSOUND_EFFECT, 1.0f);
		//static_cast<CPlayer*>(m_pTarget)->On_Parring();
		if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.left, m_tInfo.fY, EFFECT_PLAYER::R_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.right, m_tInfo.fY, EFFECT_PLAYER::L_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UR_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UL_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DR_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DL_ATTACK));
		break;
	case OBJECT_TYPE::OBJ_ENEMYPROJECTILE:
		static_cast<CPlayer*>(m_pTarget)->Hit_Count();
		if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.left, m_tInfo.fY, EFFECT_PLAYER::R_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.right, m_tInfo.fY, EFFECT_PLAYER::L_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UR_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UL_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DR_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DL_ATTACK));
		break;
	case OBJECT_TYPE::OBJ_OBS:
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword_Wall.wav", SWORDSOUND_EFFECT, 1.0f);
		if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.left, m_tInfo.fY, EFFECT_PLAYER::R_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(_rc.right, m_tInfo.fY, EFFECT_PLAYER::L_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UR_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.bottom, EFFECT_PLAYER::UL_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DR_ATTACK));
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, _rc.top, EFFECT_PLAYER::DL_ATTACK));
		break;
	default:
		break;
	}
}
