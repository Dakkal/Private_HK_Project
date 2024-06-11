#include "stdafx.h"
#include "UI_GatherBar.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"


CUI_GatherBar::CUI_GatherBar()
{
}


CUI_GatherBar::~CUI_GatherBar()
{
	Release();
}

void CUI_GatherBar::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_UI;
	m_tInfo = { 160.f, 120.f, 230.f, 200.f };

	m_tBmp.x = 230;
	m_tBmp.y = 200;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/UI/GatherBar.bmp", L"GatherBar");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/UI/GatherBar_Start.bmp", L"GatherBar_Start");

	

	m_pFrameKey = L"GatherBar_Start";
	m_eCurState = BAR_STATE::BREAK;
	m_tFrame.iFrameStart = 0;
	m_iCurFrame = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 240;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.bStopMotion = true;
}

int CUI_GatherBar::Update(void)
{
	if (m_pFrameKey == L"GatherBar_Start" && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_pFrameKey = L"GatherBar";
		m_eCurState = BAR_STATE::IDLE;
	}

	__super::Update_Rect();

	return 0;
}

void CUI_GatherBar::Late_Update()
{

	__super::Move_Frame();
	Set_Motion();
}

void CUI_GatherBar::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

	if (m_pFrameKey == L"GatherBar_Start")
	{
		GdiTransparentBlt(hDC,
			int(m_tBmpRect.left),
			int(m_tBmpRect.top),
			(int)m_tBmp.x,
			(int)m_tBmp.y,
			hMemDC,
			m_tFrame.iMotion * (int)m_tBmp.x,
			m_tFrame.iFrameStart * (int)m_tBmp.y,
			(int)m_tBmp.x,
			(int)m_tBmp.y,
			RGB(100, 100, 100));
	}
	else
	{
		GdiTransparentBlt(hDC,
			int(m_tRect.left), 
			int(m_tRect.top), 
			(int)m_tInfo.fCX, 
			(int)m_tInfo.fCY, 
			hMemDC,
			m_tFrame.iFrameStart * (int)m_tInfo.fCX,
			m_tFrame.iMotion * (int)m_tInfo.fCY,
			(int)m_tInfo.fCX, 
			(int)m_tInfo.fCY,
			RGB(100, 100, 100));
	}
	

}

void CUI_GatherBar::Release(void)
{
}

void CUI_GatherBar::Set_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case BAR_STATE::IDLE:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;
		case BAR_STATE::BREAK:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 240;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;
		}

		m_ePreState = m_eCurState;
	}
	
}
	
