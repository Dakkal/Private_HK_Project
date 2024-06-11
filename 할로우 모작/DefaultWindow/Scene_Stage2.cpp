#include "stdafx.h"
#include "Scene_Stage2.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "Tile.h"
#include "UI_GatherBar.h"
#include "UI_Mana.h"
#include "UI_Hp.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CScene_Stage2::CScene_Stage2()
{
}


CScene_Stage2::~CScene_Stage2()
{
	Release();
}

void CScene_Stage2::Initialize(void)
{
	if (CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).empty())
	{
		CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());
		CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CUI_GatherBar>::Create());
		CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_UI, CAbstractFactory<CUI_Mana>::Create());

		for (int i = 0; i <= CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front()->Get_Stat().iMaxHp; ++i)
		{
			CObj* pHp = CAbstractFactory<CUI_Hp>::Create();
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_HPUI, pHp);
			static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Get_vecHp().push_back(pHp);

		}
	}

	CTileMgr::Get_Inst()->Load_Tile();
	CObjMgr::Get_Inst()->Load_Monster();
	CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front()->Set_Pos(1600.f, 1820.f);
	CScrollMgr::Get_Inst()->Set_PoinScroll(-1190.f, -1370.f);
	

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Stage/TutoMap2.bmp", L"Stage2");
}

int CScene_Stage2::Update(void)
{
	CObjMgr::Get_Inst()->Update();

	

	return 0;
}

void CScene_Stage2::Late_Update(void)
{
	CObjMgr::Get_Inst()->Late_Update();

	if (static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Get_StageClear() == true)
	{
		static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Off_StageClear();
		CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
		CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_BOSS);
	}
}

void CScene_Stage2::Render(HDC hDC)
{
	HDC		hGroundDC = CBmpMgr::Get_Inst()->Find_Image(L"Stage2");

	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	BitBlt(hDC, (int)fScrollX, (int)fScrollY, STAGE2CX, STAGE2CY, hGroundDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Inst()->Render(hDC);
}

void CScene_Stage2::Release(void)
{
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_MONSTER);
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_TILE);
}
