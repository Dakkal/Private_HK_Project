#include "stdafx.h"
#include "Monster_Mos.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CMonster_Mos::CMonster_Mos()
	: m_bDetect(false), m_bAttack(false), m_bIsSound(false), m_bSoundPlay(false), m_bEffectPlay(false)
{
	ZeroMemory(&m_tDetect, sizeof(POINT));
	ZeroMemory(&m_tDetectRect, sizeof(RECT));
}


CMonster_Mos::~CMonster_Mos()
{
	Release();
}

void CMonster_Mos::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_MONSTER;
	m_eMonType = MONSTER_TYPE::MOS;

	m_tStat = { 1, 0, 1, 0, 1 };

	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 80.f;

	m_tBmp.x = 151;
	m_tBmp.y = 150;

	m_tDetect.x = 400;
	m_tDetect.y = 400;

	m_fSpeed = 3.f;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Monster/Mob_Mos.bmp", L"Mob_Mos");
	m_pFrameKey = L"Mob_Mos";

	int iRandom = rand() % 2;
	if (iRandom == 0)
	{
		m_eCurMonState = MONSTER_STATE::L_MOVE;
		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 8;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;

	}
	else if (iRandom == 1)
	{
		m_eCurMonState = MONSTER_STATE::R_MOVE;
		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 8;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = true;
	}
}

int CMonster_Mos::Update(void)
{
	if (!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).empty())
		Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	if (m_bDead)
	{
		Dead_Down();
		Tile_Collision();
		__super::Update_Rect();
		return OBJ_NOEVENT;
	}
		

	if (!m_bSoundPlay && m_bIsSound)
	{
		CSoundMgr::Get_Inst()->PlayLoop(L"Mos_fly.wav", m_eChannel, 1.0f);
		m_bSoundPlay = true;
	}


	if (!m_bDetect)
	{
		if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			m_tInfo.fX -= m_fSpeed;
		else if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			m_tInfo.fX += m_fSpeed;

	}
	else
	{
		Attack();	

		if (m_bEffectPlay && m_bIsSound)
		{
			if (m_dwSoundTime + 1500 <= GetTickCount())
			{
				m_bEffectPlay = false;
				m_dwSoundTime = GetTickCount();
			}
		}
		if (!m_bEffectPlay && m_bIsSound)
		{
			int iRandom = rand() % 3;

			if (iRandom == 0)
				CSoundMgr::Get_Inst()->PlaySound(L"buzzer_startle_01.wav", MOSSOUND_EFFECT, 1.0f);
			else if (iRandom == 1)
				CSoundMgr::Get_Inst()->PlaySound(L"buzzer_startle_02.wav", MOSSOUND_EFFECT, 1.0f);
			else if (iRandom == 2)
				CSoundMgr::Get_Inst()->PlaySound(L"buzzer_startle_03.wav", MOSSOUND_EFFECT, 1.0f);

			m_bEffectPlay = true;
		}
	}
		

	m_tDetectRect.left = LONG(m_tInfo.fX - (m_tDetect.x * 0.5f));
	m_tDetectRect.top = LONG(m_tInfo.fY - (m_tDetect.y * 0.5f));
	m_tDetectRect.right = LONG(m_tInfo.fX + (m_tDetect.x  * 0.5f));
	m_tDetectRect.bottom = LONG(m_tInfo.fY + (m_tDetect.y * 0.5f));
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster_Mos::Late_Update()
{
	if (!m_bDead && m_tStat.iHp <= -1)
	{
		m_bDead = true;
		m_tStat.iDamage = 0;
		m_eCurMonState = MONSTER_STATE::DEAD;
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->StopSound(m_eChannel);
		CSoundMgr::Get_Inst()->StopSound(MOSSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword_Death.wav", SWORDSOUND_EFFECT, 1.0f);
	}
		
	else
	{
		if (!m_bDetect)
		{
			if (m_eCurMonState == MONSTER_STATE::R_MOVE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				m_eCurMonState = MONSTER_STATE::L_MOVE;
				m_eLRDir = DIR_TYPE::DIR_LEFT;
				m_eCurDir = m_eLRDir;
			}
			else if (m_eCurMonState == MONSTER_STATE::L_MOVE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			{
				m_eCurMonState = MONSTER_STATE::R_MOVE;
				m_eLRDir = DIR_TYPE::DIR_RIGHT;
				m_eCurDir = m_eLRDir;
			}
		}

		if (m_bDetect)
		{
			m_bAttack = true;
		}
		
		Detect_Player();
	}
	
	__super::Move_Frame();
	Set_Motion();
	Detect_Sound();
}

void CMonster_Mos::Render(HDC hDC)
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

void CMonster_Mos::Release(void)
{
}

void CMonster_Mos::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	switch (eDir)
	{
	case COL_DIR::LEFT:
		break;
	case COL_DIR::TOP:
		if (m_bDead)
		{
			Set_PosY(-fPush);
			m_bGround = true;
		}
		break;
	case COL_DIR::RIGHT:
		break;
	case COL_DIR::BOTTOM:
		break;
	}

}

void CMonster_Mos::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}

void CMonster_Mos::Set_Motion()
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
				m_tFrame.iFrameEnd = 8;
				m_tFrame.iMotion = 1;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::R_ATTACK:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 3;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				break;

			case MONSTER_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 4;
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
				m_tFrame.iFrameEnd = 8;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case MONSTER_STATE::L_ATTACK:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 2;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				break;

			case MONSTER_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 5;
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

void CMonster_Mos::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_COLLI:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword_Damage.wav", SWORDSOUND_EFFECT, 1.0f);
		break;
	case OBJECT_TYPE::OBJ_PROJECTILE:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		break;
	default:
		break;
	}
}

void CMonster_Mos::Detect_Player()
{
	RECT rc;
	
	if (IntersectRect(&rc, &Get_Target()->Get_Rect(), &m_tDetectRect))
		m_bDetect = true;
}

void CMonster_Mos::Dead_Down()
{
	if (m_bDead && !m_bGround)
	{
		m_fDownSpeed = (-9.8f * m_fTime * m_fTime * 0.5f);
		m_tInfo.fY -= m_fDownSpeed;
		m_fTime += 0.1f;
		m_eCurMonState = MONSTER_STATE::DEAD;
	}
	else if (m_bDead && m_bGround)
	{
		m_fTime = 0.f;
		m_eCurMonState = MONSTER_STATE::DEAD;
	}
}

void CMonster_Mos::Attack()
{
	if (m_bAttack)
	{
		Target_Player();
		if (m_eCurDir == DIR_TYPE::DIR_LEFT)
			m_eCurMonState = MONSTER_STATE::L_ATTACK;
		else if (m_eCurDir == DIR_TYPE::DIR_RIGHT)
			m_eCurMonState = MONSTER_STATE::R_ATTACK;


		float fDiagonal = 0.f;
		float fWidth = 0.f;
		float fHeight = 0.f;
		float fRadian = 0.f;

		fWidth = Get_Target()->Get_Pos().fX - m_tInfo.fX;
		fHeight = Get_Target()->Get_Pos().fY - m_tInfo.fY;
		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		fRadian = acosf(fWidth / fDiagonal);
		if (fHeight < 0.f)
			fRadian *= -1.f;

		m_tInfo.fX += m_fSpeed * cosf(fRadian);
		m_tInfo.fY += m_fSpeed * sinf(fRadian);
	}
}

void CMonster_Mos::Target_Player()
{
	if (!m_bDead && m_bDetect)
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

void CMonster_Mos::Detect_Sound()
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
		/*m_bIsSound = false;
		m_bSoundPlay = false;
		m_bEffectPlay = false;
		CSoundMgr::Get_Inst()->StopSound(MOSSOUND_EFFECT);
		CSoundMgr::Get_Inst()->StopSound(m_eChannel);*/
	}
}
