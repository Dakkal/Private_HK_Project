#include "stdafx.h"
#include "Scene_Menu.h"
#include "SoundMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "MenuUI.h"

CScene_Menu::CScene_Menu()
{
}


CScene_Menu::~CScene_Menu()
{
	Release();
}

void CScene_Menu::Initialize(void)
{
	CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CMenuUI>::Create(300.f, 338.f, DIR_TYPE::DIR_RIGHT));
	CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CMenuUI>::Create(519.f, 338.f, DIR_TYPE::DIR_LEFT));

	for (auto& iter : CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI))
	{
		static_cast<CMenuUI*>(iter)->Set_MenuType(MENU_TYPE::START);
	}

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Menu/Menu.bmp", L"Menu");
	CSoundMgr::Get_Inst()->PlayBGM(L"Title.wav", 0.4f);
}

int CScene_Menu::Update(void)
{
	if (!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).empty() && 
		static_cast<CMenuUI*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).front())->Get_MenuType() == MENU_TYPE::START)
	{
		if (CKeyMgr::Get_Inst()->Key_Pressing(VK_DOWN))
		{
			CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_UI);
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CMenuUI>::Create(300.f, 410.f, DIR_TYPE::DIR_RIGHT));
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CMenuUI>::Create(519.f, 410.f, DIR_TYPE::DIR_LEFT));
			for (auto& iter : CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI))
			{
				static_cast<CMenuUI*>(iter)->Set_MenuType(MENU_TYPE::END);
			}

		}
	}
	else if (!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).empty() &&
		static_cast<CMenuUI*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).front())->Get_MenuType() == MENU_TYPE::END)
	{
		if (CKeyMgr::Get_Inst()->Key_Pressing(VK_UP))
		{
			CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_UI);
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CMenuUI>::Create(300.f, 338.f, DIR_TYPE::DIR_RIGHT));
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CMenuUI>::Create(519.f, 338.f, DIR_TYPE::DIR_LEFT));
			for (auto& iter : CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI))
			{
				static_cast<CMenuUI*>(iter)->Set_MenuType(MENU_TYPE::START);
			}

		}
	}

	CObjMgr::Get_Inst()->Update();
	return 0;
}

void CScene_Menu::Late_Update(void)
{
	CObjMgr::Get_Inst()->Late_Update();


	if (CKeyMgr::Get_Inst()->Key_Down(VK_RETURN))
	{
		if (static_cast<CMenuUI*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).front())->Get_MenuType() == MENU_TYPE::START)
		{
			CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
			CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_STAGE);
		}
		else if (static_cast<CMenuUI*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_UI).front())->Get_MenuType() == MENU_TYPE::END)
		{
			DestroyWindow(g_hWnd);
		}
	}

}

void CScene_Menu::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(L"Menu");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Inst()->Render(hDC);
}

void CScene_Menu::Release(void)
{
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_UI);
}
