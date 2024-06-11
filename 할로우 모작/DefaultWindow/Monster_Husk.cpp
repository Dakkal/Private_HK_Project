#include "stdafx.h"
#include "Monster_Husk.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CMonster_Husk::CMonster_Husk()
	: m_bDetect(false), m_bAttack(false), m_bSwitchPos(false), m_bIsSound(false), m_bSoundPlay(false)
{
	ZeroMemory(&m_tDetect, sizeof(POINT));
	ZeroMemory(&m_tDetectRect, sizeof(RECT));
}


CMonster_Husk::~CMonster_Husk()
{
	Release();
}

void CMonster_Husk::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_MONSTER;
	m_eMonType = MONSTER_TYPE::HUSK;

	m_tStat = { 1, 0, 1, 0, 1 };

	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_tBmp.x = 81;
	m_tBmp.y = 81;

	m_tDetect.x = 600;
	m_tDetect.y = 200;

	m_fSpeed = 0.5f;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Monster/Mob_Husk.bmp", L"Mob_Husk");
	m_pFrameKey = L"Mob_Husk";

	int iRandom = rand() % 2;
	if (iRandom == 0)
	{
		m_eCurMonState = MONSTER_STATE::L_MOVE;
		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 150;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;

	}
	else if (iRandom == 1)
	{
		m_eCurMonState = MONSTER_STATE::R_MOVE;
		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 150;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
}

int CMonster_Husk::Update(void)
{
	if (!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).empty())
		Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	Tile_Collision();
	Down();

	if (m_bDead)
	{
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}


	if (!m_bDetect)
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
	else
	{
		Target_Player();
		Attack();
	}

	m_tDetectRect.left = LONG(m_tInfo.fX - (m_tDetect.x * 0.5f));
	m_tDetectRect.top = LONG(m_tInfo.fY - (m_tDetect.y * 0.5f));
	m_tDetectRect.right = LONG(m_tInfo.fX + (m_tDetect.x  * 0.5f));
	m_tDetectRect.bottom = LONG(m_tInfo.fY + (m_tDetect.y * 0.5f));
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster_Husk::Late_Update()
{
	if (!m_bDead && m_tStat.iHp <= -1)
	{
		m_bDead = true;
		m_tStat.iDamage = 0;
		m_eCurMonState = MONSTER_STATE::DEAD;
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->StopSound(HUSKSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword_Death.wav", SWORDSOUND_EFFECT, 1.0f);
		CSoundMgr::Get_Inst()->PlaySound(L"Husk_Death.wav", HUSKSOUND_EFFECT, 1.0f);
	}
	else if(!m_bDead)
	{
		if (!m_bDetect)
		{
			if (m_bAttack)
				m_bAttack = false;

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

		if (m_bDetect)
		{
			if (!m_bAttack)
			{
				m_bAttack = true;
				m_eCurMonState = MONSTER_STATE::IDLE;
				if (!m_bSoundPlay)
				{
					int iRandom = rand() % 2;

					if (iRandom == 0)
					{
						CSoundMgr::Get_Inst()->StopSound(HUSKSOUND_EFFECT);
						CSoundMgr::Get_Inst()->PlaySound(L"Husk_Chase_1.wav", HUSKSOUND_EFFECT, 1.0f);
					}
				
					else if (iRandom == 1)
					{
						CSoundMgr::Get_Inst()->StopSound(HUSKSOUND_EFFECT);
						CSoundMgr::Get_Inst()->PlaySound(L"Husk_Chase_2.wav", HUSKSOUND_EFFECT, 1.0f);
					}
					m_bSoundPlay = true;
				}
			}
		}
			
		Detect_Player();
	}

	__super::Move_Frame();
	Set_Motion();
	Detect_Sound();

}

void CMonster_Husk::Render(HDC hDC)
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

void CMonster_Husk::Release(void)
{
	CSoundMgr::Get_Inst()->StopSound(HUSKSOUND_EFFECT);
}

void CMonster_Husk::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
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

void CMonster_Husk::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}

void CMonster_Husk::Set_Motion()
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
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 300;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::R_ATTACK:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 4;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				break;

			case MONSTER_STATE::IDLE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 2;
				m_tFrame.iMotion = 2;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 6;
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
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 1;
				m_tFrame.dwSpeed = 300;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::L_ATTACK:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 5;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				break;

			case MONSTER_STATE::IDLE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 2;
				m_tFrame.iMotion = 3;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 7;
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

void CMonster_Husk::OnCollision(CObj * _pObj, RECT _rc)
{
	if (!m_bDead)
	{
		switch (_pObj->Get_ObjType())
		{
		case OBJECT_TYPE::OBJ_COLLI:
			if (!m_bDead)
			{
				Get_Damaged(_pObj->Get_Stat().iDamage);
				CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"Sword_Damage.wav", SWORDSOUND_EFFECT, 1.0f);
			}
			break;
		case OBJECT_TYPE::OBJ_PROJECTILE:
			if (!m_bDead)
				Get_Damaged(_pObj->Get_Stat().iDamage);
			break;
		default:
			break;
		}
	}

	
}

void CMonster_Husk::Detect_Player()
{
	RECT rc;

	if (IntersectRect(&rc, &Get_Target()->Get_Rect(), &m_tDetectRect) && Get_Target()->Get_Dead() == false)
	{
		m_bDetect = true;
	}
	
	else
	{
		m_bDetect = false;
		m_bSoundPlay = false;

		if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			m_eCurMonState = MONSTER_STATE::L_MOVE;

		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			m_eCurMonState = MONSTER_STATE::R_MOVE;
	}
		
}

void CMonster_Husk::Down()
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

void CMonster_Husk::Attack()
{
	if (m_bAttack && m_eCurMonState == MONSTER_STATE::IDLE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		if (m_eLRDir == DIR_TYPE::DIR_LEFT)
		{
			m_eCurMonState = MONSTER_STATE::L_ATTACK;
			m_eCurDir = DIR_TYPE::DIR_LEFT;
		}
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
		{
			m_eCurMonState = MONSTER_STATE::R_ATTACK;
			m_eCurDir = DIR_TYPE::DIR_RIGHT;
		}

	}

	if (m_bAttack && m_eCurDir == DIR_TYPE::DIR_LEFT)
	{
		m_eCurMonState = MONSTER_STATE::L_ATTACK;
		m_tInfo.fX -= 4.f;
	}
		
	else if (m_bAttack && m_eCurDir == DIR_TYPE::DIR_RIGHT)
	{
		m_eCurMonState = MONSTER_STATE::R_ATTACK;
		m_tInfo.fX += 4.f;
	}
		
}

void CMonster_Husk::Target_Player()
{
	if (!m_bDead)
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
	}
}

void CMonster_Husk::Detect_Sound()
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
		//CSoundMgr::Get_Inst()->StopSound(HUSKSOUND_EFFECT);
		//CSoundMgr::Get_Inst()->StopSound(HUSKSOUND_LOOP);
	}
}
