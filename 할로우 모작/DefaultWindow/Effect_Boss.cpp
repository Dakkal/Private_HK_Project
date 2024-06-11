#include "stdafx.h"
#include "Effect_Boss.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "CollisionMgr.h"

CEffect_Boss::CEffect_Boss()
{
}


CEffect_Boss::~CEffect_Boss()
{
	Release();
}

void CEffect_Boss::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_EFFECT;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Boss/ForEffect.bmp", L"ForEffect");

	m_pFrameKey = L"ForEffect";
	m_tFrame.iFrameStart = 0;
	m_iCurFrame = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 80;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.bStopMotion = true;
	m_tBmp.x = 680.f;
	m_tBmp.y = 240.f;
}

int CEffect_Boss::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;



	__super::Update_Rect();

	return 0;
}

void CEffect_Boss::Late_Update()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;

	__super::Move_Frame();
}

void CEffect_Boss::Render(HDC hDC)
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

void CEffect_Boss::Release(void)
{
}
