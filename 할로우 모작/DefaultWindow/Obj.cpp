#include "stdafx.h"
#include "Obj.h"
#include "SoundMgr.h"

CObj::CObj()
	: m_fSpeed(0.f), m_fAngle(0.f), m_fDistance(0.f), m_fPower(0.f), m_fTime(0.f), m_bDead(false), m_iCurFrame(0)
	, m_eLRDir(DIR_TYPE::DIR_END), m_eUDDir(DIR_TYPE::DIR_END), m_eRender(RENDER_TYPE::RENDER_END), m_bCollision(false), m_bHit(false), m_bGround(false)
	, m_bDown(false), m_fDownSpeed(0.f), m_eMonType(MONSTER_TYPE::END), m_eChannel(MAXCHANNEL)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tStat, sizeof(STAT));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tBmpRect, sizeof(RECT));
	ZeroMemory(&m_tBmp, sizeof(POINT));
	
	m_dwTime = GetTickCount();
	m_dwSoundTime = GetTickCount();
}


CObj::~CObj()
{
}

void CObj::Late_Update()
{
}

void CObj::Update_Rect(void)
{
		m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
		m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
		m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
		m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));

		m_tBmpRect.left = LONG(m_tInfo.fX - (m_tBmp.x * 0.5f));
		m_tBmpRect.top = LONG(m_tInfo.fY - (m_tBmp.y * 0.5f));
		m_tBmpRect.right = LONG(m_tInfo.fX + (m_tBmp.x  * 0.5f));
		m_tBmpRect.bottom = LONG(m_tInfo.fY + (m_tBmp.y * 0.5f));
}

void CObj::Update_BmpRect(void)
{
}

void CObj::Move_Frame(void)
{
	if (!m_tFrame.bStopMotion)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = m_iCurFrame;

			m_tFrame.dwTime = GetTickCount();
		}
	}

	if (m_tFrame.bStopMotion)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;

			m_tFrame.dwTime = GetTickCount();
		}
	}
	
}
