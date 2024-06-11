#include "stdafx.h"
#include "Scene_Logo.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"


CScene_Logo::CScene_Logo()
{
}


CScene_Logo::~CScene_Logo()
{
	Release();
}

void CScene_Logo::Initialize(void)
{
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");
}

int CScene_Logo::Update(void)
{


	return 0;
}

void CScene_Logo::Late_Update(void)
{
	//m_dwTime + 3000 <= GetTickCount();

	/*if (m_dwTime + 3000 <= GetTickCount())
	{
		CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_MENU);
	}*/
	if (CKeyMgr::Get_Inst()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_MENU);
	}
}

void CScene_Logo::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(L"Logo");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CScene_Logo::Release(void)
{
}
