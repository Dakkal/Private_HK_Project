#include "stdafx.h"
#include "Monster_Knight.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "MonKnighht_Coli.h"
#include "SoundMgr.h"

CMonster_Knight::CMonster_Knight()
: m_bDetect(false), m_bAttack(false), m_bGuard(false), m_bSwitchPos(false), m_bGrogy(false), m_pColi(nullptr), m_bIsParring(false), m_bIsSound(false)
{
	ZeroMemory(&m_tDetect, sizeof(POINT));
	ZeroMemory(&m_tDetectRect, sizeof(RECT));
	m_dwAttackTime = GetTickCount();
}


CMonster_Knight::~CMonster_Knight()
{
	Release();
}

void CMonster_Knight::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_MONSTER;
	m_eMonType = MONSTER_TYPE::KNIGHT;

	m_tStat = { 10, 0, 10, 0, 1 };

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 100.f;

	m_tBmp.x = 194.5;
	m_tBmp.y = 150;

	m_tDetect.x = 650;
	m_tDetect.y = 900;

	m_fSpeed = 0.5f;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Monster/Mob_Knight.bmp", L"Mob_Knight");
	m_pFrameKey = L"Mob_Knight";

	int iRandom = rand() % 2;
	if (iRandom == 0)
	{
		m_eCurMonState = MONSTER_STATE::L_MOVE;
		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_eUDDir == DIR_TYPE::DIR_END;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 150;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;

	}
	else if (iRandom == 1)
	{
		m_eCurMonState = MONSTER_STATE::R_MOVE;
		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_eUDDir == DIR_TYPE::DIR_END;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 150;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}

}

int CMonster_Knight::Update(void)
{
	if (m_bDead)
	{
		Tile_Collision();
		Down();
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}

	if (!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).empty())
		Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	Tile_Collision();
	Down();
	Grogy();

	

	if (!m_bDetect && !m_bGrogy)
	{
		if (m_eLRDir == DIR_TYPE::DIR_LEFT && !m_bSwitchPos)
		{
			m_eCurMonState = MONSTER_STATE::L_MOVE;
			m_tInfo.fX -= m_fSpeed;
		}

		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && !m_bSwitchPos)
		{
			m_eCurMonState = MONSTER_STATE::R_MOVE;
			m_tInfo.fX += m_fSpeed;
		}

	}
	else if (m_bDetect && !m_bGrogy)
	{
		Target_Player();
		Set_PlayerDis();

		if (!m_bGuard && m_bAttack)
			Attack();
		if (!m_bAttack && m_bGuard)
			Guard();
	}

	m_tDetectRect.left = LONG(m_tInfo.fX - (m_tDetect.x * 0.5f));
	m_tDetectRect.top = LONG(m_tInfo.fY - (m_tDetect.y * 0.5f));
	m_tDetectRect.right = LONG(m_tInfo.fX + (m_tDetect.x  * 0.5f));
	m_tDetectRect.bottom = LONG(m_tInfo.fY + (m_tDetect.y * 0.5f));
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster_Knight::Late_Update()
{
	if (!m_bDead && m_tStat.iHp <= -1)
	{
		m_bDead = true;
		m_tStat.iDamage = 0;
		m_eCurMonState = MONSTER_STATE::DEAD;
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword_Death.wav", SWORDSOUND_EFFECT, 1.0f);
		CSoundMgr::Get_Inst()->PlaySound(L"Husk_Death.wav", KNIGHTSOUND_EFFECT, 1.0f);
	}
	else if(!m_bDead)
	{
		if (!m_bDetect && !m_bGrogy)
		{
			if (m_bAttack || m_bGuard)
			{
				m_bAttack = false;
				m_bGuard = false;
			}

			if (m_eCurMonState == MONSTER_STATE::R_MOVE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				if (!m_bSwitchPos)
				{
					m_dwTime = GetTickCount();
					m_bSwitchPos = true;
				}

				else if (m_dwTime + 1500 <= GetTickCount() && m_bSwitchPos)
				{
					m_eCurMonState = MONSTER_STATE::L_MOVE;
					m_eLRDir = DIR_TYPE::DIR_LEFT;
					m_eCurDir = m_eLRDir;
					m_dwTime = GetTickCount();
					m_bSwitchPos = false;
				}

			}
			else if (m_eCurMonState == MONSTER_STATE::L_MOVE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				if (!m_bSwitchPos)
				{
					m_dwTime = GetTickCount();
					m_bSwitchPos = true;
				}

				else if (m_dwTime + 1500 <= GetTickCount() && m_bSwitchPos)
				{
					m_eCurMonState = MONSTER_STATE::R_MOVE;
					m_eLRDir = DIR_TYPE::DIR_RIGHT;
					m_eCurDir = m_eLRDir;
					m_dwTime = GetTickCount();
					m_bSwitchPos = false;
				}
			}
		}

		else if (m_bDetect && !m_bGrogy)
		{
			if (!m_bAttack && m_fPlayerDis >= 100.f && m_fPlayerDis <= 150.f)
			{
				if (m_dwAttackTime + 3000 <= GetTickCount())
				{
					m_bGuard = false;
					m_bAttack = true;
					if (m_eCurDir == DIR_TYPE::DIR_LEFT)
						m_eCurMonState = MONSTER_STATE::L_ATTACK;
					else if (m_eCurDir == DIR_TYPE::DIR_RIGHT)
						m_eCurMonState = MONSTER_STATE::R_ATTACK;
					
				}
				else 
					m_bGuard = true;
			}
			else
			{
				if(!m_bAttack)
					m_bGuard = true;
			}
		}

		Detect_Player();
	}


	__super::Move_Frame();
	Set_Motion();
	Detect_Sound();
}

void CMonster_Knight::Render(HDC hDC)
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

void CMonster_Knight::Release(void)
{
	CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
}

void CMonster_Knight::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	switch (eDir)
	{
	case COL_DIR::LEFT:
		Set_PosX(-fPush);
		break;
	case COL_DIR::TOP:
		Set_PosY(-fPush);
		m_bGround = true;
		break;
	case COL_DIR::RIGHT:
		Set_PosX(fPush);
		break;
	case COL_DIR::BOTTOM:
		Set_PosY(fPush);
		break;
	}
}

void CMonster_Knight::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}

void CMonster_Knight::Set_Motion()
{
	if (m_ePreDir != m_eCurDir || m_ePreMonState != m_eCurMonState)
	{
		if (m_eCurDir == DIR_TYPE::DIR_RIGHT)
		{
			switch (m_eCurMonState)
			{
			case MONSTER_STATE::R_MOVE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 7;
				m_tFrame.iMotion = 1;
				m_tFrame.dwSpeed = 300;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::R_ATTACK:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 10;
				m_tFrame.iMotion = 7;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::R_GUARD:
				m_tFrame.iFrameStart = 1;
				m_iCurFrame = 1;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 3;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Shield.wav", KNIGHTSOUND_EFFECT, 1.0f);
				break;

			case MONSTER_STATE::UR_GUARD:
				m_tFrame.iFrameStart = 4;
				m_iCurFrame = 4;
				m_tFrame.iFrameEnd = 7;
				m_tFrame.iMotion = 2;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Shield.wav", KNIGHTSOUND_EFFECT, 1.0f);
				break;

			case MONSTER_STATE::IDLE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 5;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 9;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;
			}

			m_ePreMonState = m_eCurMonState;
			m_ePreDir = m_eCurDir;
		}

		else if (m_eCurDir == DIR_TYPE::DIR_LEFT)
		{
			switch (m_eCurMonState)
			{
			case MONSTER_STATE::L_MOVE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 7;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 300;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::L_ATTACK:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 10;
				m_tFrame.iMotion = 6;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::L_GUARD:
				m_tFrame.iFrameStart = 1;
				m_iCurFrame = 1;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 2;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Shield.wav", KNIGHTSOUND_EFFECT, 1.0f);
				break;

			case MONSTER_STATE::UL_GUARD:
				m_tFrame.iFrameStart = 4;
				m_iCurFrame = 4;
				m_tFrame.iFrameEnd = 7;
				m_tFrame.iMotion = 3;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Shield.wav", KNIGHTSOUND_EFFECT, 1.0f);
				break;

			case MONSTER_STATE::IDLE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 4;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 8;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;
			}

			m_ePreMonState = m_eCurMonState;
			m_ePreDir = m_eCurDir;
		}
	}
}

void CMonster_Knight::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_COLLI:
		if (m_bGrogy)
		{
			Get_Damaged(_pObj->Get_Stat().iDamage + 2);
			CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Sword_Damage.wav", SWORDSOUND_EFFECT, 1.0f);
		}
		else if (m_bGuard)
		{
			CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Sword_Wall.wav", SWORDSOUND_EFFECT, 1.0f);
		}
		else if (!m_bDetect)
		{
			Get_Damaged(_pObj->Get_Stat().iDamage + 2);
			CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Sword_Damage.wav", SWORDSOUND_EFFECT, 1.0f);
		}
		break;
	case OBJECT_TYPE::OBJ_PROJECTILE:
		if (m_bGrogy)
		{
			Get_Damaged(_pObj->Get_Stat().iDamage + 2);
		}

		else if (!m_bDetect)
		{
			Get_Damaged(_pObj->Get_Stat().iDamage + 2);
		}
		break;
	default:
		break;
	}
}

void CMonster_Knight::Detect_Player()
{
	RECT rc;

	if (IntersectRect(&rc, &Get_Target()->Get_Rect(), &m_tDetectRect))
	{
		m_bDetect = true;
		Set_PlayerDis();
	}
		
		
	else
	{
		m_bDetect = false;

		if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			m_eCurMonState = MONSTER_STATE::L_MOVE;

		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			m_eCurMonState = MONSTER_STATE::R_MOVE;
	}
}

void CMonster_Knight::Down()
{
	if (!m_bGround)
	{
		m_fDownSpeed = (-9.8f * m_fTime * m_fTime * 0.5f);
		m_tInfo.fY -= m_fDownSpeed;
		m_fTime += 0.1f;
	}
	else if (m_bGround)
	{
		m_fTime = 0.f;
	}
}

void CMonster_Knight::Guard()
{
	if (m_bGuard)
	{
		Target_Player();


		if (m_eCurDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_END)
			m_eCurMonState = MONSTER_STATE::L_GUARD;
		else if (m_eCurDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
			m_eCurMonState = MONSTER_STATE::UL_GUARD;
		else if (m_eCurDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_END)
			m_eCurMonState = MONSTER_STATE::R_GUARD;
		else if (m_eCurDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
			m_eCurMonState = MONSTER_STATE::UR_GUARD;

		
	}

}

void CMonster_Knight::Attack()
{
	if (m_bAttack)
	{
		if (m_dwTime + 100 <= GetTickCount())
		{
			if (m_eLRDir == DIR_TYPE::DIR_RIGHT &&  m_tFrame.iFrameStart == 3)
			{
				m_pColi = CAbstractFactory<CMonKnighht_Coli>::Create(m_tInfo.fX + 50.f, m_tInfo.fY);
				static_cast<CMonKnighht_Coli*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYCOLLI, m_pColi);
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Sword_1.wav", KNIGHTSOUND_EFFECT, 1.0f);
			}
				
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT &&  m_tFrame.iFrameStart == 3)
			{
				m_pColi = CAbstractFactory<CMonKnighht_Coli>::Create(m_tInfo.fX - 50.f, m_tInfo.fY);
				static_cast<CMonKnighht_Coli*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYCOLLI, m_pColi);
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Sword_1.wav", KNIGHTSOUND_EFFECT, 1.0f);
			}
			else if (m_eLRDir == DIR_TYPE::DIR_RIGHT &&  m_tFrame.iFrameStart == 9)
			{
				m_pColi = CAbstractFactory<CMonKnighht_Coli>::Create(m_tInfo.fX + 50.f, m_tInfo.fY);
				static_cast<CMonKnighht_Coli*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYCOLLI, m_pColi);
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Sword_2.wav", KNIGHTSOUND_EFFECT, 1.0f);
			}
				
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT &&  m_tFrame.iFrameStart == 9)
			{
				m_pColi = CAbstractFactory<CMonKnighht_Coli>::Create(m_tInfo.fX - 50.f, m_tInfo.fY);
				static_cast<CMonKnighht_Coli*>(m_pColi)->Set_Owner(this);
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYCOLLI, m_pColi);
				CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Knight_Sword_2.wav", KNIGHTSOUND_EFFECT, 1.0f);
			}
			m_dwTime = GetTickCount();
		}

		if (m_eCurMonState == MONSTER_STATE::L_ATTACK  && m_tFrame.iFrameStart == m_tFrame.iFrameEnd ||
			m_eCurMonState == MONSTER_STATE::R_ATTACK  && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bAttack = false;
			m_dwAttackTime = GetTickCount();
		}
			
	}
	
}

void CMonster_Knight::Target_Player()
{
	if (!m_bDead && !m_bAttack && m_bDetect)
	{
		if (Get_Target()->Get_Pos().fX < m_tInfo.fX)
		{
			m_eLRDir = DIR_TYPE::DIR_LEFT;
			m_eCurDir = m_eLRDir;
		}
		else if (Get_Target()->Get_Pos().fX > m_tInfo.fX)
		{
			m_eLRDir = DIR_TYPE::DIR_RIGHT;
			m_eCurDir = m_eLRDir;
		}

		if (Get_Target()->Get_Pos().fY < m_tInfo.fY)
		{
			m_eUDDir = DIR_TYPE::DIR_UP;
		}
		else if (Get_Target()->Get_Pos().fY >= m_tInfo.fY)
		{
			m_eUDDir = DIR_TYPE::DIR_END;
		}
	}
}

void CMonster_Knight::Set_PlayerDis()
{
	m_fPlayerDis = fabs(Get_Target()->Get_Pos().fX - m_tInfo.fX);
}

void CMonster_Knight::Grogy()
{
	if (!m_bGrogy && m_bIsParring)
	{
			m_bGrogy = true;
			m_bIsParring = false;
			m_eCurMonState = MONSTER_STATE::IDLE;
			m_bAttack = false;
			m_bGuard = false;
			m_dwAttackTime = GetTickCount();
			m_dwTime = GetTickCount();
	}
	if (m_bGrogy)
	{
		if (m_dwTime + 1500 <= GetTickCount())
		{
			m_bGrogy = false;
			m_dwTime = GetTickCount();

		}
	}	
}

void CMonster_Knight::Detect_Sound()
{
	RECT rc;
	RECT rSound;

	rSound.left = LONG(m_tInfo.fX - (WINCX * 0.5f));
	rSound.top = LONG(m_tInfo.fY - (WINCY * 0.5f));
	rSound.right = LONG(m_tInfo.fX + (WINCX  * 0.5f));
	rSound.bottom = LONG(m_tInfo.fY + (WINCY * 0.5f));

	if (IntersectRect(&rc, &Get_Target()->Get_Rect(), &rSound))
	{
		m_bIsSound = true;
	}
	else
	{
		m_bIsSound = false;
		CSoundMgr::Get_Inst()->StopSound(KNIGHTSOUND_EFFECT);
	}
}
