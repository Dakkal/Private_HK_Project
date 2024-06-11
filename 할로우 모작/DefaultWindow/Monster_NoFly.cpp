#include "stdafx.h"
#include "Monster_NoFly.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CMonster_NoFly::CMonster_NoFly()
	: m_bDetect(false), m_bIsSound(false)
{
	ZeroMemory(&m_tDetect, sizeof(POINT));
	ZeroMemory(&m_tDetectRect, sizeof(RECT));
}


CMonster_NoFly::~CMonster_NoFly()
{
	Release();
}

void CMonster_NoFly::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_MONSTER;
	m_eMonType = MONSTER_TYPE::NOMOVE_FLY;

	m_tStat = { 100, 0, 100, 0, 0 };

	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 80.f;

	m_tBmp.x = 115;
	m_tBmp.y = 120;

	m_fSpeed = 1.f;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Monster/Mob_Fly.bmp", L"Mob_Fly");
	m_pFrameKey = L"Mob_Fly";


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

int CMonster_NoFly::Update(void)
{
	if (!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).empty())
		Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	if (!m_bSoundPlay && m_bIsSound)
	{
		CSoundMgr::Get_Inst()->PlayLoop(L"Fly_Flying.wav", m_eChannel, 1.0f);
		m_bSoundPlay = true;
	}


	if (m_bDead && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		if (m_dwTime + 100 <= GetTickCount())
		{
			m_dwTime = GetTickCount();
			return OBJ_DEAD;
		}

	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster_NoFly::Late_Update()
{
	if (!m_bDead && m_tStat.iHp < 0)
	{
		m_bDead = true;
		m_dwTime = GetTickCount();
		m_eCurMonState = MONSTER_STATE::DEAD;
	}



	__super::Move_Frame();
	Set_Motion();
	Detect_Sound();
}

void CMonster_NoFly::Render(HDC hDC)
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

void CMonster_NoFly::Release(void)
{
	CSoundMgr::Get_Inst()->StopSound(m_eChannel);
}

void CMonster_NoFly::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{

}

void CMonster_NoFly::Tile_Collision()
{
}

void CMonster_NoFly::Set_Motion()
{
}

void CMonster_NoFly::OnCollision(CObj * _pObj, RECT _rc)
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

void CMonster_NoFly::Detect_Sound()
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
