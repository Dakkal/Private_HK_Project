#include "stdafx.h"
#include "UI_Hp.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "UI_GatherBar.h"

CUI_Hp::CUI_Hp()
{
}


CUI_Hp::~CUI_Hp()
{
	Release();
}

void CUI_Hp::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_UI;
	
	if (CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_HPUI).empty())
		m_tInfo.fX = 200.f;
	else
		m_tInfo.fX = 40 + CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_HPUI).back()->Get_Pos().fX;

	m_tInfo.fY = 85.f;
	m_tInfo.fCX = 70.f;
	m_tInfo.fCY = 130.f;

	m_tBmp.x = 128;
	m_tBmp.y = 256;

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/UI/HP_INTRO.bmp", L"HP_INTRO");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/UI/HP_DESTROY.bmp", L"HP_DESTROY");

	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());

	m_eCurState = HP_STATE::SAFE;
	m_pFrameKey = L"HP_INTRO";
}

int CUI_Hp::Update(void)
{
	if (m_eCurState == HP_STATE::HIT)
		m_pFrameKey = L"HP_DESTROY";

	else if (m_eCurState == HP_STATE::SAFE)
		m_pFrameKey = L"HP_INTRO";



	__super::Update_Rect();

	return 0;
}

void CUI_Hp::Late_Update()
{
	if (static_cast<CUI_GatherBar*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).front())->Get_BarState() == BAR_STATE::BREAK)
		return;


	__super::Move_Frame();
	Set_Motion();

}

void CUI_Hp::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

		GdiTransparentBlt(hDC,
			int(m_tRect.left),
			int(m_tRect.top),
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			m_tFrame.iMotion * (int)m_tBmp.x,
			m_tFrame.iFrameStart * (int)m_tBmp.y,
			(int)m_tBmp.x,
			(int)m_tBmp.y,
			RGB(100, 100, 100));
}

void CUI_Hp::Release(void)
{
}

void CUI_Hp::Set_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case HP_STATE::SAFE:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;

		case HP_STATE::HIT:
			m_tFrame.iFrameStart = 0;
			m_iCurFrame = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.bStopMotion = true;
			break;
		}

		m_ePreState = m_eCurState;
	}
}
