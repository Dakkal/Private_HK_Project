#include "stdafx.h"
#include "Effect_Player.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"
#include "Player.h"

CEffect_Player::CEffect_Player()
	: m_bIsEffect(false), m_ePlayerEffect(EFFECT_PLAYER::END)
{
}


CEffect_Player::~CEffect_Player()
{
	Release();
}

void CEffect_Player::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_EFFECT;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/Effect/UD_AttackEffect.bmp", L"UD_AttackEffect");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/Effect/RL_AttackEffect.bmp", L"RL_AttackEffect");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/Effect/DamageEffect.bmp", L"DamageEffect");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/Effect/Recovery.bmp", L"Recovery");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/Effect/R_Soul_Effect.bmp", L"R_Soul_Effect");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/Effect/L_Soul_Effect .bmp", L"L_Soul_Effect");
	
	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	//m_eLRDir = Get_Target()->Get_LRDirType();

	if (!m_bIsEffect)
	{
		switch (m_ePlayerEffect)
		{
		case EFFECT_PLAYER::R_ATTACK:
			m_bIsEffect = true;
			m_pFrameKey = L"RL_AttackEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 225.f;
			m_tBmp.y = 150.f;
			break;
		case EFFECT_PLAYER::L_ATTACK:
			m_bIsEffect = true;
			m_pFrameKey = L"RL_AttackEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 225.f;
			m_tBmp.y = 150.f;
			break;
		case EFFECT_PLAYER::UR_ATTACK:
			m_bIsEffect = true;
			m_pFrameKey = L"UD_AttackEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 91.f;
			m_tBmp.y = 145.f;
			break;
		case EFFECT_PLAYER::UL_ATTACK:
			m_bIsEffect = true;
			m_pFrameKey = L"UD_AttackEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 91.f;
			m_tBmp.y = 145.f;
			break;
		case EFFECT_PLAYER::DR_ATTACK:
			m_bIsEffect = true;
			m_pFrameKey = L"UD_AttackEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 91.f;
			m_tBmp.y = 145.f;
			break;
		case EFFECT_PLAYER::DL_ATTACK:
			m_bIsEffect = true;
			m_pFrameKey = L"UD_AttackEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 91.f;
			m_tBmp.y = 145.f;
			break;
		case EFFECT_PLAYER::L_HIT:
			m_bIsEffect = true;
			m_pFrameKey = L"DamageEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 518.f;
			m_tBmp.y = 180.f;
			break;
		case EFFECT_PLAYER::R_HIT:
			m_bIsEffect = true;
			m_pFrameKey = L"DamageEffect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			m_tBmp.x = 518.f;
			m_tBmp.y = 180.f;
			break;
		case EFFECT_PLAYER::RECOVER:
			m_bIsEffect = true;
			m_pFrameKey = L"Recovery";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			m_tBmp.x = 115.f;
			m_tBmp.y = 150.f;
			break;
		case EFFECT_PLAYER::R_Soul:
			m_bIsEffect = true;
			m_pFrameKey = L"R_Soul_Effect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			m_tBmp.x = 110.f;
			m_tBmp.y = 150.f;
			break;
		case EFFECT_PLAYER::L_Soul:
			m_bIsEffect = true;
			m_pFrameKey = L"L_Soul_Effect";
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			m_tBmp.x = 110.f;
			m_tBmp.y = 150.f;
			break;
		default:
			break;
		}
	}


}

int CEffect_Player::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;



	__super::Update_Rect();

	return 0;
}

void CEffect_Player::Late_Update()
{
	if (m_ePlayerEffect == EFFECT_PLAYER::RECOVER && !static_cast<CPlayer*>(Get_Target())->Get_Heal())
		m_bDead = true;

	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd && m_ePlayerEffect != EFFECT_PLAYER::RECOVER)
		m_bDead = true;

	__super::Move_Frame();
}

void CEffect_Player::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

	if (m_pFrameKey == L"DamageEffect")
	{
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
			RGB(2, 2, 2));
	}
	else if (m_pFrameKey == L"R_Soul_Effect" || m_pFrameKey == L"L_Soul_Effect")
	{
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
	else
	{
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
			RGB(0, 0, 0));
	}
	

	

}

void CEffect_Player::Release(void)
{
}