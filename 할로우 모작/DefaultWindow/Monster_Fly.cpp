#include "stdafx.h"
#include "Monster_Fly.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"


CMonster_Fly::CMonster_Fly()
	: m_bIsSound(false), m_bSoundPlay(false)
{
	ZeroMemory(&m_tDetect, sizeof(POINT));
	ZeroMemory(&m_tDetectRect, sizeof(RECT));
}


CMonster_Fly::~CMonster_Fly()
{
	Release();
}

void CMonster_Fly::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_MONSTER;
	m_eMonType = MONSTER_TYPE::FLY;

	m_tStat = { 1, 0, 1, 0, 1 };

	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 80.f;

	m_tBmp.x = 115;
	m_tBmp.y = 120;

	m_fSpeed = 1.f;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Monster/Mob_Fly.bmp", L"Mob_Fly");
	m_pFrameKey = L"Mob_Fly";
	

	int iRandom = (rand() % 4);

	if (iRandom == 0)
	{
		m_eCurMonState = MONSTER_STATE::L_MOVE;
		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_eUDDir = DIR_TYPE::DIR_UP;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = false;
	}
	else if (iRandom == 1)
	{
		m_eCurMonState = MONSTER_STATE::L_MOVE;
		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_eUDDir = DIR_TYPE::DIR_DOWN;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 0;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = false;
	}
	else if (iRandom == 2)
	{
		m_eCurMonState = MONSTER_STATE::R_MOVE;
		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_eUDDir = DIR_TYPE::DIR_UP;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = false;
	}
	else if (iRandom == 3)
	{
		m_eCurMonState = MONSTER_STATE::R_MOVE;
		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_eUDDir = DIR_TYPE::DIR_DOWN;
		m_tFrame.iFrameStart = 0;
		m_iCurFrame = 0;
		m_tFrame.iFrameEnd = 3;
		m_tFrame.iMotion = 1;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.bStopMotion = false;
	}


}

int CMonster_Fly::Update(void)
{
	if(!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).empty())
		Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());


	if (m_bDead && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		if (m_dwTime + 100 <= GetTickCount())
		{
			m_dwTime = GetTickCount();
		
			return OBJ_DEAD;
		}
			
	}

	if (!m_bSoundPlay && m_bIsSound)
	{
		CSoundMgr::Get_Inst()->PlayLoop(L"Fly_Flying.wav", m_eChannel, 1.0f);
		m_bSoundPlay = true;
	}

	if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_UP)
	{
		m_tInfo.fX -= m_fSpeed;
		m_tInfo.fY -= m_fSpeed;
	}
	else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_UP)
	{
		m_tInfo.fX += m_fSpeed;
		m_tInfo.fY -= m_fSpeed;
	}
	else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_eUDDir == DIR_TYPE::DIR_DOWN)
	{
		m_tInfo.fX -= m_fSpeed;
		m_tInfo.fY += m_fSpeed;
	}
	else if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_eUDDir == DIR_TYPE::DIR_DOWN)
	{
		m_tInfo.fX += m_fSpeed;
		m_tInfo.fY += m_fSpeed;
	}


	/*m_tDetectRect.left = LONG(m_tInfo.fX - (m_tDetect.x * 0.5f));
	m_tDetectRect.top = LONG(m_tInfo.fY - (m_tDetect.y * 0.5f));
	m_tDetectRect.right = LONG(m_tInfo.fX + (m_tDetect.x  * 0.5f));
	m_tDetectRect.bottom = LONG(m_tInfo.fY + (m_tDetect.y * 0.5f));*/

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster_Fly::Late_Update()
{
	if (!m_bDead && m_tStat.iHp < 0)
	{
		m_bDead = true;
		m_dwTime = GetTickCount();
		m_eCurMonState = MONSTER_STATE::DEAD;
		CSoundMgr::Get_Inst()->StopSound(SWORDSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"Sword_Death.wav", SWORDSOUND_EFFECT, 1.0f);
		CSoundMgr::Get_Inst()->StopSound(m_eChannel);
	}
		


	__super::Move_Frame();
	Set_Motion();

	Tile_Collision();
	Detect_Sound();
}

void CMonster_Fly::Render(HDC hDC)
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

void CMonster_Fly::Release(void)
{
	CSoundMgr::Get_Inst()->StopSound(m_eChannel);
}

void CMonster_Fly::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	switch (eDir)
	{
	case COL_DIR::LEFT:
		m_eLRDir = DIR_TYPE::DIR_LEFT;
		m_eCurMonState = MONSTER_STATE::L_MOVE;
		break;
	case COL_DIR::TOP:
		m_eUDDir = DIR_TYPE::DIR_UP;
		break;
	case COL_DIR::RIGHT:
		m_eLRDir = DIR_TYPE::DIR_RIGHT;
		m_eCurMonState = MONSTER_STATE::R_MOVE;
		break;
	case COL_DIR::BOTTOM:
		m_eUDDir = DIR_TYPE::DIR_DOWN;
		break;
	}
}

void CMonster_Fly::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}

void CMonster_Fly::Set_Motion()
{
	if (m_ePreMonState != m_eCurMonState && m_eLRDir == DIR_TYPE::DIR_RIGHT)
	{
		switch (m_eCurMonState)
		{
		case MONSTER_STATE::R_MOVE:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case MONSTER_STATE::DEAD:
			m_tFrame.iFrameStart = 1;
			m_iCurFrame = 1;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;
		}

			m_ePreMonState = m_eCurMonState;
	}

	else if (m_ePreMonState != m_eCurMonState && m_eLRDir == DIR_TYPE::DIR_LEFT)
	{
		switch (m_eCurMonState)
		{
		case MONSTER_STATE::L_MOVE:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case MONSTER_STATE::DEAD:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;
		}

		m_ePreMonState = m_eCurMonState;
	}
}

void CMonster_Fly::OnCollision(CObj * _pObj, RECT _rc)
{
	if (!m_bDead)
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
	
}

void CMonster_Fly::Detect_Sound()
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
		m_bSoundPlay = false;
		CSoundMgr::Get_Inst()->StopSound(m_eChannel);
	}
}
