#include "stdafx.h"
#include "UI_Mana.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "UI_GatherBar.h"

CUI_Mana::CUI_Mana()
{
}


CUI_Mana::~CUI_Mana()
{
	Release();
}

void CUI_Mana::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_UI;

	m_tInfo = { 92.f, 112.f, 159.5f, 156.f };

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/UI/Mana2.bmp", L"Mana");

	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	m_pFrameKey = L"Mana";
	m_eCurState = MANA_STATE::FULL;
}

int CUI_Mana::Update(void)
{
	switch (Get_Target()->Get_Stat().iMana)
	{
	case 0:
		m_eCurState = MANA_STATE::END;
		break;

	case 1:
		m_eCurState = MANA_STATE::LOW;
		break;

	case 2:
		m_eCurState = MANA_STATE::MIDDLE;
		break;

	case 3:
		m_eCurState = MANA_STATE::HIGH;
		break;

	case 4:
		m_eCurState = MANA_STATE::FULL;
		break;


	default:
		break;
	}




	__super::Update_Rect();

	return 0;
}

void CUI_Mana::Late_Update()
{

	__super::Move_Frame();
	Set_Motion();
}

void CUI_Mana::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

	if (static_cast<CUI_GatherBar*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).front())->Get_BarState() != BAR_STATE::BREAK &&
		m_eCurState != MANA_STATE::END )
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

void CUI_Mana::Release(void)
{
}

void CUI_Mana::Set_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case MANA_STATE::LOW:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;
		case MANA_STATE::MIDDLE:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case MANA_STATE::HIGH:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = false;
			break;

		case MANA_STATE::FULL:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;
		}

		m_ePreState = m_eCurState;
	}
}
