#include "stdafx.h"
#include "Scene_Edit.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Monster_Fly.h"
#include "Monster_Mos.h"
#include "Monster_Husk.h"
#include "Monster_Knight.h"
#include "Monster_NoFly.h"


CScene_Edit::CScene_Edit()
{
}


CScene_Edit::~CScene_Edit()
{
	Release();
}

void CScene_Edit::Initialize(void)
{
	//CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Stage/TutoMap1.bmp", L"Stage1");
	//CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Stage/TutoMap2.bmp", L"Stage2");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Stage/BossStage.bmp", L"BossStage");
	//m_pStageKey = L"Stage1";
	//m_pStageKey = L"Stage2";
	m_pStageKey = L"BossStage";

	CTileMgr::Get_Inst()->Initialize();
}

int CScene_Edit::Update(void)
{
	CTileMgr::Get_Inst()->Update();

	if (CKeyMgr::Get_Inst()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Inst()->Set_ScrollX(30.f);

	if (CKeyMgr::Get_Inst()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Inst()->Set_ScrollX(-30.f);

	if (CKeyMgr::Get_Inst()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Inst()->Set_ScrollY(30.f);

	if (CKeyMgr::Get_Inst()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Inst()->Set_ScrollY(-30.f);

	if (CKeyMgr::Get_Inst()->Key_Pressing(VK_LBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CTileMgr::Get_Inst()->Picking_Tile(pt, 1);
	}
	else if (CKeyMgr::Get_Inst()->Key_Pressing(VK_RBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CTileMgr::Get_Inst()->Picking_Tile(pt, 0);
	}
	else if (CKeyMgr::Get_Inst()->Key_Pressing('Z'))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CTileMgr::Get_Inst()->Picking_Tile(pt, 2);
	}
	else if (CKeyMgr::Get_Inst()->Key_Down('Q'))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CObjMgr::Get_Inst()->AddMon(CAbstractFactory<CMonster_Fly>::Create(pt.x, pt.y));
	}
	else if (CKeyMgr::Get_Inst()->Key_Down('W'))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CObjMgr::Get_Inst()->AddMon(CAbstractFactory<CMonster_Mos>::Create(pt.x, pt.y));
	}
	else if (CKeyMgr::Get_Inst()->Key_Down('E'))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CObjMgr::Get_Inst()->AddMon(CAbstractFactory<CMonster_Husk>::Create(pt.x, pt.y));
	}
	else if (CKeyMgr::Get_Inst()->Key_Down('R'))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CObjMgr::Get_Inst()->AddMon(CAbstractFactory<CMonster_Knight>::Create(pt.x, pt.y));
	}
	else if (CKeyMgr::Get_Inst()->Key_Down('T'))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (long)CScrollMgr::Get_Inst()->Get_ScrollX();
		pt.y -= (long)CScrollMgr::Get_Inst()->Get_ScrollY();

		CObjMgr::Get_Inst()->AddMon(CAbstractFactory<CMonster_NoFly>::Create(pt.x, pt.y));
	}
	else if (CKeyMgr::Get_Inst()->Key_Down('P'))
	{
		CObjMgr::Get_Inst()->Get_vecMon().pop_back();
	}

	if (CKeyMgr::Get_Inst()->Key_Down('S'))
	{
		CTileMgr::Get_Inst()->Save_Tile();
		CObjMgr::Get_Inst()->Save_Monster();
	}
	if (CKeyMgr::Get_Inst()->Key_Down('L'))
	{
		//CTileMgr::Get_Inst()->Load_EditTile();
		//CObjMgr::Get_Inst()->Load_EditMonster();
	}
		

	return 0;
}

void CScene_Edit::Late_Update(void)
{
	CTileMgr::Get_Inst()->Late_Update();
	CObjMgr::Get_Inst()->Update_Mon();
}

void CScene_Edit::Render(HDC hDC)
{
	HDC		hGroundDC = CBmpMgr::Get_Inst()->Find_Image(m_pStageKey);

	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	//BitBlt(hDC, (int)fScrollX, (int)fScrollY, STAGE1CX, STAGE1CY, hGroundDC, 0, 0, SRCCOPY);
	//BitBlt(hDC, (int)fScrollX, (int)fScrollY, STAGE2CX, STAGE2CY, hGroundDC, 0, 0, SRCCOPY);
	BitBlt(hDC, (int)fScrollX, (int)fScrollY, BOSSSTAGECX, BOSSSTAGECY, hGroundDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Inst()->Render(hDC);
	CObjMgr::Get_Inst()->Render_Mon(hDC);
}

void CScene_Edit::Release(void)
{
	CObjMgr::Get_Inst()->Delete_vecMon();
}
