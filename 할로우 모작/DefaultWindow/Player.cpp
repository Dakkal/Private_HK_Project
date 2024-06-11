#include "stdafx.h"
#include "Player.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AttackColi.h"
#include "EnergyPo_Coli.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "UI_Hp.h"
#include "Effect_Player.h"
#include "SoundMgr.h"

CPlayer::CPlayer()
	: m_bJump(false), m_bAttack(false), m_bSceneStart(true), m_bDash(false), m_bDoubleJump(false), m_bHeal(false), m_bParring(false), m_bClimb(false), m_bLClimb(false), m_bRClimb(false)
	, m_bEnergyPo(false), m_bHitTime(false), m_bInvicible(false), m_bDashInvic(false), m_iHitCount(0), m_pColi(nullptr), m_bParringJump(false), m_bStageClear(false)
	, m_bCameraShaking(false)
	
{
	m_dwHealTime = GetTickCount();
	m_dwHitTime = GetTickCount();
	m_dwInvicTime = GetTickCount();
	m_dwCameraTime = GetTickCount();

	m_vecHp.reserve(m_tStat.iMaxHp);
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_PLAYER;

	m_tStat = { 5, 4, 5, 4, 1 };

	m_tInfo = { 650.f, 1000.f, 64.f, 80.f };

	m_tBmp.x = 110;
	m_tBmp.y = 91;

	m_fSpeed = 5.f;
	m_fPower = 17.f;
	m_fDash = 0.f;
	m_bInvicible = false;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Player/Player.bmp", L"Player");

	m_pFrameKey = L"Player";
	m_eCurState = STATE::ST_IDLE;
	m_eRender = RENDER_TYPE::RENDER_GAMEOBJECT;
	m_eLRDir = DIR_TYPE::DIR_RIGHT;
	m_eUDDir = DIR_TYPE::DIR_END;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
}

int CPlayer::Update(void)
{
	Tile_Collision();

	if (m_bDead)
	{
		if (CKeyMgr::Get_Inst()->Key_Down('R'))
			Revive();

		__super::Update_Rect();

		return OBJ_NOEVENT;
	}

	if (!m_bDead)
	{
		Attack();

		Energy_Po();

		Heal();

		IsHit();

		Jump();

		DoubleJump();

		Dash();

		Key_Input();
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{	
	if (!m_bDead && m_tStat.iHp < 0)
	{
		m_bDead = true;
		m_eCurState = STATE::ST_DEAD;

		// Á×¾úÀ»½Ã »óÅÂÃÊ±âÈ­ 
		m_bInvicible = false;
		m_bHitTime = false;
		m_bAttack = false;
		m_bDash = false;
		m_bDashInvic = false;
		m_bDoubleJump = false;
		m_bDown = false;
		m_bEnergyPo = false;
		m_bHeal = false;
		m_bHit = false;
		m_bParring = false;
		m_bParringJump = false;
		m_bClimb = false;
		m_dwHealTime = GetTickCount();
		m_dwHitTime = GetTickCount();
		m_dwTime = GetTickCount();
	}
	Down();


	OffSet();
	
	Get_Mana();

	__super::Move_Frame();
	Set_Motion();
	
	Tile_Collision();
}

void CPlayer::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

	if (!m_bInvicible)
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

	else if (m_bInvicible)
	{
		if (GetTickCount() % 2 == 0)
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
		
	}
}

void CPlayer::Release(void)
{
	m_vecHp.clear();
	m_vecHp.shrink_to_fit();
}

void CPlayer::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	OBJECT_TYPE eType = _pObj->Get_ObjType();
	CObj* pColi = _pObj;

	switch (eDir)
	{
	case COL_DIR::LEFT:
		if (static_cast<CTile*>(pColi)->Get_DrawID() == 2)
			m_bStageClear = true;
		Set_PosX(-fPush);
		if (m_bDoubleJump || m_bJump)
		{
			m_bClimb = true;
			m_bRClimb = true;
		}	
		break;
	case COL_DIR::TOP:
		Set_PosY(-fPush);
		m_bGround = true;
		break;
	case COL_DIR::RIGHT:
		if (static_cast<CTile*>(pColi)->Get_DrawID() == 2)
			m_bStageClear = true;
		Set_PosX(fPush);
		if (m_bDoubleJump || m_bJump)
		{
			m_bClimb = true;
			m_bLClimb = true;
		}
		break;
	case COL_DIR::BOTTOM:
		Set_PosY(fPush);
		break;
	}
}

void CPlayer::Key_Input(void)
{
	if (CKeyMgr::Get_Inst()->Key_Down('R'))
	{
		m_tStat.iMana = m_tStat.iMaxMana;
	}

	if (!m_bHitTime && !m_bJump && CKeyMgr::Get_Inst()->Key_Down(VK_SPACE))
	{
		m_bJump = true;
		m_eCurState = STATE::ST_JUMP;
		CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Jump.wav", PLAYERSOUND_EFFECT, 1.0f);
	}

	if (!m_bHitTime && !m_bDoubleJump && m_bJump && CKeyMgr::Get_Inst()->Key_Down(VK_SPACE)||
		!m_bHitTime && !m_bDoubleJump  && m_bParringJump && CKeyMgr::Get_Inst()->Key_Down(VK_SPACE))
	{
		m_bDoubleJump = true;
		m_fTime = 0.f;
		m_eCurState = STATE::ST_DOUBLEJUMP;
		CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Player_DoubleJump.wav", PLAYERSOUND_EFFECT, 1.0f);
	}

	if (!m_bClimb && !m_bHitTime && !m_bHeal && CKeyMgr::Get_Inst()->Key_Down(VK_SHIFT))
	{
		m_bDashInvic = true;
		m_bDash = true;
		m_eCurState = STATE::ST_DASH;
		CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Dash.wav", PLAYERSOUND_EFFECT, 1.0f);
	}

	if (!m_bClimb && !m_bHitTime && CKeyMgr::Get_Inst()->Key_Down('A'))
	{
		m_bAttack = true;
		CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword.wav", PLAYERSOUND_EFFECT, 1.0f);

		if (m_eUDDir == DIR_TYPE::DIR_UP) 
			m_eCurState = STATE::ST_TOPATTACK; 

		else if (m_eUDDir == DIR_TYPE::DIR_DOWN)
			m_eCurState = STATE::ST_DOWNATTACK;

		else
			m_eCurState = STATE::ST_ATTACK;
	}

	if (!m_bClimb && !m_bHitTime && m_tStat.iMana > 0 && CKeyMgr::Get_Inst()->Key_Down('S'))
	{
		m_bEnergyPo = true;
		Use_Mana();
		m_eCurState = STATE::ST_ENERGY_PO;
		CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
		CSoundMgr::Get_Inst()->StopSound(ENERGYSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Player_EnergyPo.wav", ENERGYSOUND_EFFECT, 1.0f);
	}

	if (!m_bClimb && !m_bHitTime && m_tStat.iMana > 0 && m_tStat.iHp < m_tStat.iMaxHp &&
		!m_bJump && CKeyMgr::Get_Inst()->Key_Pressing('D'))
	{
		if(!m_bHeal)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX, m_tInfo.fY - 20.f, EFFECT_PLAYER::RECOVER));
		m_bHeal = true;
		m_eCurState = STATE::ST_HEAL;
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Health_Charging.wav", HEALSOUND_EFFECT, 1.0f);
	}
	else
	{
		m_bHeal = false;
		m_dwHealTime = GetTickCount();
		CSoundMgr::Get_Inst()->StopSound(HEALSOUND_EFFECT);
	}
		

	if (!m_bClimb && !m_bHitTime && !m_bDash && !m_bEnergyPo && CKeyMgr::Get_Inst()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
		m_pFrameKey = L"Player";

		if (!m_bAttack && !m_bJump && !m_bHit)
			m_eCurState = STATE::ST_WALK;

		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_eUDDir = DIR_TYPE::DIR_END;
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Walk.wav", PLAYERSOUND_EFFECT, 1.0f);
	}
	else if (!m_bClimb && !m_bHitTime && !m_bDash && !m_bEnergyPo && GetAsyncKeyState(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
		m_pFrameKey = L"Player";
		
		if (!m_bAttack && !m_bJump && !m_bHit)
			m_eCurState = STATE::ST_WALK;

		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_eUDDir = DIR_TYPE::DIR_END;
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Walk.wav", PLAYERSOUND_EFFECT, 1.0f);

	}
	else if (!m_bHitTime && GetAsyncKeyState(VK_UP))
	{
		m_pFrameKey = L"Player";
		if (!m_bAttack && !m_bJump && !m_bHit)
			m_eCurState = STATE::ST_LOOKUP;

		m_eUDDir = DIR_TYPE::DIR_UP;
	
	}

	else if (!m_bHitTime && GetAsyncKeyState(VK_DOWN))
	{
		m_pFrameKey = L"Player";
		if (!m_bAttack && !m_bJump && !m_bHit)
			m_eCurState = STATE::ST_LOOKDOWN;

		m_eUDDir = DIR_TYPE::DIR_DOWN;

	}

	else if (m_bClimb && m_bLClimb && !m_bHitTime && GetAsyncKeyState(VK_LEFT))
	{
		m_pFrameKey = L"Player";
		m_bDoubleJump = false;
		m_bJump = false;
		m_fTime = 0.f;
		m_tInfo.fY += 0.2f;
		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_eCurState = STATE::ST_CLIMB;
	}

	else if (m_bClimb && m_bRClimb && !m_bHitTime && GetAsyncKeyState(VK_RIGHT))
	{
		m_pFrameKey = L"Player";
		m_bDoubleJump = false;
		m_bJump = false;
		m_fTime = 0.f;
		m_tInfo.fY += 0.2f;
		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_eCurState = STATE::ST_CLIMB;
	}

	else
	{
		if (!m_bClimb && !m_bAttack && !m_bJump && !m_bDown && !m_bDead && !m_bHitTime && !m_bEnergyPo && !m_bHeal && !m_bDash)
		{
			m_eCurState = STATE::ST_IDLE;
			m_eUDDir = DIR_TYPE::DIR_END;
			CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
		}
		m_bClimb = false;
		m_bLClimb = false;
		m_bRClimb = false;
	}
		
}

void CPlayer::Jump()
{
	if (!m_bDoubleJump && m_bJump)
	{
		float JumpHeight = m_fPower * m_fTime;
		float Gravity = 9.8f * m_fTime * m_fTime * 0.5f;

		if (Gravity < -30.f)
		{
			Gravity = -30.f;
		}

		if(!m_bEnergyPo)
		{
			m_tInfo.fY -= JumpHeight - Gravity;
			m_fTime += 0.15f;
		}
		

		if (JumpHeight < Gravity)
		{
			if (!m_bAttack && !m_bHit && !m_bDead && !m_bDash && !m_bEnergyPo && !m_bHitTime)
			m_eCurState = STATE::ST_DOWN;
			m_fTime -= 0.11f;
		}
			
		if (JumpHeight < Gravity && m_bGround)
		{
			m_bJump = false;
			m_fTime = 0.f;
			CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Player_land.wav", PLAYERSOUND_EFFECT, 1.0f);
		}
	}
}

void CPlayer::DoubleJump()
{
	if (m_bDoubleJump)
	{
		float JumpHeight = m_fPower * m_fTime;
		float Gravity = 9.8f * m_fTime * m_fTime * 0.5f;

		if (Gravity < -30.f)
		{
			Gravity = -30.f;
		}

		if (!m_bEnergyPo)
		{
			m_tInfo.fY -= JumpHeight - Gravity;
			m_fTime += 0.15f;
		}

		if (JumpHeight < Gravity)
		{
			if (!m_bAttack && !m_bHit && !m_bDead && !m_bDash && !m_bEnergyPo && !m_bHitTime)
				m_eCurState = STATE::ST_DOWN;
			m_fTime -= 0.11f;
		}

		if (JumpHeight < Gravity && m_bGround)
		{
			m_bDoubleJump = false;
			m_bJump = false;
			m_fTime = 0.f;
			CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Player_land.wav", PLAYERSOUND_EFFECT, 1.0f);
		}
	}
}

void CPlayer::ParringJump()
{
	if (m_bParringJump)
	{

		float JumpHeight = 20 * m_fTime;
		float Gravity = 9.8f * m_fTime * m_fTime * 0.5f;

		if (Gravity < -30.f)
		{
			Gravity = -30.f;
		}

		if (!m_bEnergyPo)
		{
			m_tInfo.fY -= JumpHeight - Gravity;
			m_fTime += 0.15f;
		}

		if (JumpHeight < Gravity)
		{
			m_bParringJump = false;
			m_fTime = 0.f;
		}
	}
}

void CPlayer::Dash()
{
	if (m_bDash)
	{
		if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			m_tInfo.fX += m_fDash;
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			m_tInfo.fX -= m_fDash;

		m_fDash += 2.f;

		if (m_fDash >= 40.f)
		{
			m_bDash = false;
			m_bDashInvic = false;
			m_fDash = 0.f;
		}
	}
}

void CPlayer::Attack()
{
	if (m_bAttack)
	{
		if (m_dwTime + 50 <= GetTickCount())
		{
			if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END && m_bAttack && m_tFrame.iFrameStart == 1)
			{
				m_pColi = CAbstractFactory<CAttackColi>::Create(m_tInfo.fX + 50.f, m_tInfo.fY);
				static_cast<CAttackColi*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_COLLI, m_pColi);
			}
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END && m_bAttack && m_tFrame.iFrameStart == 1)
			{
				m_pColi = CAbstractFactory<CAttackColi>::Create(m_tInfo.fX - 50.f, m_tInfo.fY);
				static_cast<CAttackColi*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_COLLI, m_pColi);
			}
			else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP && m_bAttack && m_tFrame.iFrameStart == 1)
			{
				m_pColi = CAbstractFactory<CAttackColi>::Create(m_tInfo.fX, m_tInfo.fY - 80.f);
				static_cast<CAttackColi*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_COLLI, m_pColi);
			}	
			else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN && m_bAttack && m_tFrame.iFrameStart == 1)
			{
				m_pColi = CAbstractFactory<CAttackColi>::Create(m_tInfo.fX, m_tInfo.fY + 80.f);
				static_cast<CAttackColi*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_COLLI, m_pColi);
			}	
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP && m_bAttack && m_tFrame.iFrameStart == 1)
			{
				m_pColi = CAbstractFactory<CAttackColi>::Create(m_tInfo.fX, m_tInfo.fY - 80.f);
				static_cast<CAttackColi*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_COLLI, m_pColi);
			}
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN && m_bAttack && m_tFrame.iFrameStart == 1)
			{
				m_pColi = CAbstractFactory<CAttackColi>::Create(m_tInfo.fX, m_tInfo.fY + 80.f);
				static_cast<CAttackColi*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_COLLI, m_pColi);
			}
				

			m_dwTime = GetTickCount();
		}

		

		if (m_eCurState == STATE::ST_ATTACK  && m_tFrame.iFrameStart == m_tFrame.iFrameEnd ||
			m_eCurState == STATE::ST_TOPATTACK  && m_tFrame.iFrameStart == m_tFrame.iFrameEnd ||
			m_eCurState == STATE::ST_DOWNATTACK && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
				m_bAttack = false;
	}
	
}

void CPlayer::Heal()
{
	if (m_bHeal)
	{
		if (m_dwHealTime + 1500 <= GetTickCount())
		{
			if (m_tStat.iHp < m_tStat.iMaxHp)
				m_tStat.iHp += 1;

			static_cast<CUI_Hp*>(m_vecHp[m_tStat.iHp])->Set_CurHpState(HP_STATE::SAFE);
			Use_Mana();
			m_dwHealTime = GetTickCount();
			CSoundMgr::Get_Inst()->PlaySound(L"Player_Health_Heal.wav", HEALSOUND_EFFECT, 1.0f);
		}

	}
}

void CPlayer::Energy_Po()
{
	if (m_bEnergyPo)
	{
		if (m_dwTime + 50 <= GetTickCount())
		{
			if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_tFrame.iFrameStart == 4)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_PROJECTILE, CAbstractFactory<CEnergyPo_Coli>::Create(m_tInfo.fX + 50.f, m_tInfo.fY));
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_tFrame.iFrameStart == 4)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_PROJECTILE, CAbstractFactory<CEnergyPo_Coli>::Create(m_tInfo.fX - 50.f, m_tInfo.fY));

			m_dwTime = GetTickCount();
		}
		if (m_eCurState == STATE::ST_ENERGY_PO && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_bEnergyPo = false;

	}
}

void CPlayer::Down()
{
	// ³«ÇÏ
	if (!m_bHitTime && !m_bGround && !m_bJump && !m_bClimb)
	{
		m_fDownSpeed = (-9.8f * m_fTime * m_fTime * 0.5f);

		if (m_fDownSpeed < -30.f)
		{
			m_fDownSpeed = -30.f;
		}

		m_tInfo.fY -= m_fDownSpeed;
		m_fTime += 0.1f;
		m_bDown = true;
		m_eCurState = STATE::ST_DOWN;
	}
	// ³«ÇÏÁ¾·á
	if (m_bDown && !m_bHitTime && m_bGround && !m_bJump && !m_bClimb)
	{
		m_fTime = 0.f;
		m_bDown = false;
	}
}

void CPlayer::OffSet()
{
	float fCameraShakeX = 0.f;
	float fCameraShakeY = 0.f;
	float fIntensity = 0.f;

	float fOffSetminX = 390;
	float fOffSetmaxX = 410.f;

	float fOffSetminY = 370.f;
	float fOffSetmaxY = 380.f;

	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();
	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();


	if (m_bCameraShaking)
	{
		fCameraShakeX = float(rand() % 21 - 10); 
		fCameraShakeY = float(rand() % 21 - 10);
		
		if(m_bHitTime)
			fIntensity = 0.6f;
		else
			fIntensity = 0.15f;

		if (m_dwCameraTime + 300 <= GetTickCount())
		{
			m_dwCameraTime = GetTickCount();
			m_bCameraShaking = false;
		}
		else
		{
			fCameraShakeX *= fIntensity;
			fCameraShakeY *= fIntensity;

			if (fOffSetminX > m_tInfo.fX + fCameraShakeX)
				CScrollMgr::Get_Inst()->Set_ScrollX(fCameraShakeX);
			if (fOffSetmaxX < m_tInfo.fX + fCameraShakeX)
				CScrollMgr::Get_Inst()->Set_ScrollX(-fCameraShakeX);
			if (fOffSetminY > m_tInfo.fY + fCameraShakeY)
				CScrollMgr::Get_Inst()->Set_ScrollY(fCameraShakeY);
			if (fOffSetmaxY < m_tInfo.fY + fCameraShakeY)
				CScrollMgr::Get_Inst()->Set_ScrollY(-fCameraShakeY);
		}

		return;
	}


	if (m_dwTime + 2000 <= GetTickCount())
	{
		m_bSceneStart = false;
		m_dwTime = GetTickCount();
	}
		

	if (fOffSetminX > m_tInfo.fX + fScrollX)
	{
		if(m_bDash)
			CScrollMgr::Get_Inst()->Set_ScrollX(m_fDash);
		else
			CScrollMgr::Get_Inst()->Set_ScrollX(m_fSpeed);
	}
		

	if (fOffSetmaxX < m_tInfo.fX + fScrollX)
	{
		if (m_bDash)
			CScrollMgr::Get_Inst()->Set_ScrollX(-m_fDash);
		else
			CScrollMgr::Get_Inst()->Set_ScrollX(-m_fSpeed);
	}

	if (fOffSetminY > m_tInfo.fY + fScrollY)
	{
		if (m_bSceneStart == true)
			CScrollMgr::Get_Inst()->Set_ScrollY(10.f);
		else
			CScrollMgr::Get_Inst()->Set_ScrollY(m_fSpeed);
	}
	

	if (fOffSetmaxY < m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Inst()->Set_ScrollY(-m_fSpeed);
}

void CPlayer::IsHit()
{
	if (m_bHitTime)
	{
		if (m_eCurState == STATE::ST_HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bHitTime = false;
			m_eCurState = STATE::ST_IDLE;
		}
			

		if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			m_tInfo.fX -= 3.f;
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			m_tInfo.fX += 3.f;
	}

	if (m_bInvicible)
	{
		if (m_dwInvicTime + 2500 <= GetTickCount())
		{
			m_bInvicible = false;
			m_dwInvicTime = GetTickCount();
			
		}
	}
}

void CPlayer::Climb()
{
}

void CPlayer::Set_Motion()
{
	if (m_ePreState != m_eCurState && m_eLRDir == DIR_TYPE::DIR_RIGHT)
	{
		switch (m_eCurState)
		{
		case STATE::ST_IDLE:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case STATE::ST_LOOKDOWN:
			m_tFrame.iFrameStart = 4;
			m_iCurFrame = 4;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_LOOKUP:
			m_tFrame.iFrameStart = 8;
			m_iCurFrame = 8;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_WALK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case STATE::ST_JUMP:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DOUBLEJUMP:
			m_tFrame.iFrameStart = 5;
			m_iCurFrame = 5;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 12;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DOWN:
			m_tFrame.iFrameStart = 6;
			m_iCurFrame = 6;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 5;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_TOPATTACK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 7;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DOWNATTACK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 9;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DASH:
			m_tFrame.iFrameStart = 5;
			m_iCurFrame = 5;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 9;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_ENERGY_PO:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 15;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_CLIMB:
			m_tFrame.iFrameStart = 7;
			m_iCurFrame = 7;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iMotion = 15;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_HEAL:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 11;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iMotion = 22;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case STATE::ST_HIT:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 17;
			m_tFrame.dwSpeed = 80;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DEAD:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 19;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Player_Death.wav", PLAYERSOUND_EFFECT, 1.0f);
			break;
		}

		m_ePreState = m_eCurState;
	}

	else if (m_ePreState != m_eCurState && m_eLRDir == DIR_TYPE::DIR_LEFT)
	{
		switch (m_eCurState)
		{
		case STATE::ST_IDLE:
			m_tFrame.iFrameStart = 2;
			m_iCurFrame = 2;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case STATE::ST_LOOKDOWN:
			m_tFrame.iFrameStart = 6;
			m_iCurFrame = 6;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_LOOKUP:
			m_tFrame.iFrameStart = 10;
			m_iCurFrame = 10;
			m_tFrame.iFrameEnd = 11;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_WALK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case STATE::ST_JUMP:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 4;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DOUBLEJUMP:
			m_tFrame.iFrameStart = 5;
			m_iCurFrame = 5;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 11;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DOWN:
			m_tFrame.iFrameStart = 6;
			m_iCurFrame = 6;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 4;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 6;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_TOPATTACK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 8;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DOWNATTACK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 10;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DASH:
			m_tFrame.iFrameStart = 5;
			m_iCurFrame = 5;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 8;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_ENERGY_PO:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 16;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_CLIMB:
			m_tFrame.iFrameStart = 7;
			m_iCurFrame = 7;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iMotion = 16;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_HEAL:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 11;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iMotion = 23;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case STATE::ST_HIT:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 18;
			m_tFrame.dwSpeed = 80;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case STATE::ST_DEAD:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = 20;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Player_Death.wav", PLAYERSOUND_EFFECT, 1.0f);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPlayer::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_MONSTER:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		break;
	case OBJECT_TYPE::OBJ_BOSS:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		break;
	case OBJECT_TYPE::OBJ_ENEMYCOLLI:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		break;
	case OBJECT_TYPE::OBJ_ENEMYPROJECTILE:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		break;
	default:
		break;
	}
}

void CPlayer::Get_Damaged(int _iDamage)
{
	if (m_tStat.iHp <= -1 || _iDamage <= 0)
		return;

	if (!m_bInvicible && !m_bDashInvic && !m_bHitTime && _iDamage >= 2 && m_tStat.iHp <= 0)
	{
		CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_HIT);
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Damage.wav", PLAYERSOUND_HIT, 1.0f);

		static_cast<CUI_Hp*>(m_vecHp[m_tStat.iHp])->Set_CurHpState(HP_STATE::HIT);

		m_tStat.iHp -= 1;

		m_bHitTime = true;
		m_bInvicible = true;
		m_dwInvicTime = GetTickCount();

		m_bAttack = false;
		m_bEnergyPo = false;

		if (!m_bCameraShaking)
		{
			m_bCameraShaking = true;
			m_dwCameraTime = GetTickCount();
		}

		m_eCurState = STATE::ST_HIT;
		if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX - 20.f, m_tInfo.fY, EFFECT_PLAYER::L_HIT));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX + 20.f, m_tInfo.fY, EFFECT_PLAYER::R_HIT));
	}
	else if (!m_bInvicible && !m_bDashInvic && !m_bHitTime)
	{
		CSoundMgr::Get_Inst()->StopSound(PLAYERSOUND_HIT);
		CSoundMgr::Get_Inst()->PlaySound(L"Player_Damage.wav", PLAYERSOUND_HIT, 1.0f);

		for (int i = 0; i < _iDamage; ++i)
		{
			static_cast<CUI_Hp*>(m_vecHp[m_tStat.iHp])->Set_CurHpState(HP_STATE::HIT);

			m_tStat.iHp -= 1;
		}

		m_bHitTime = true;
		m_bInvicible = true;
		m_dwInvicTime = GetTickCount();

		m_bAttack = false;
		m_bEnergyPo = false;

		if (!m_bCameraShaking)
		{
			m_bCameraShaking = true;
			m_dwCameraTime = GetTickCount();
		}
			

		m_eCurState = STATE::ST_HIT;
		if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX - 20.f, m_tInfo.fY, EFFECT_PLAYER::L_HIT));
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Player>::Create(m_tInfo.fX + 20.f, m_tInfo.fY, EFFECT_PLAYER::R_HIT));
	}
	
}

void CPlayer::Get_Mana()
{
	if (m_iHitCount >= 10 && m_tStat.iMana < m_tStat.iMaxMana)
	{
		++m_tStat.iMana;
		m_iHitCount = 0;
	}
	else if (m_iHitCount >= 10 && m_tStat.iMana >= m_tStat.iMaxMana)
	{
		m_iHitCount = 0;
	}
		
}

void CPlayer::Revive()
{
	if (m_bDead)
	{
		m_bDead = false;
		m_tStat.iHp = 0;

		for (int i = 0; i <= m_tStat.iMaxHp; ++i)
		{
			static_cast<CUI_Hp*>(m_vecHp[m_tStat.iHp])->Set_CurHpState(HP_STATE::SAFE);

			if(m_tStat.iHp < m_tStat.iMaxHp)
				m_tStat.iHp += 1;
		}

		m_tStat.iMana = m_tStat.iMaxMana;
		m_eCurState = STATE::ST_IDLE;
	}
}

void CPlayer::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}
