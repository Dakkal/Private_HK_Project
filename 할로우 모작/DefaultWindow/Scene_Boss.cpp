#include "stdafx.h"
#include "Scene_Boss.h"
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
#include "Boss.h"
#include "Obstacle.h"
#include "SoundMgr.h"

CScene_Boss::CScene_Boss()
{
}


CScene_Boss::~CScene_Boss()
{
	Release();
}

void CScene_Boss::Initialize(void)
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
	CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front()->Set_Pos(300.f, 990.f);
	CScrollMgr::Get_Inst()->Set_PoinScroll(0.f, -535.f);

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Stage/BossStage.bmp", L"BossStage");
}

int CScene_Boss::Update(void)
{
	CObjMgr::Get_Inst()->Update();

	if (CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_BOSS).empty())
	{
		int iCount = 0;

		for (auto& iter : CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_MONSTER))
		{
			if (iter->Get_Dead() == true)
				iCount++;

			if (iCount >= 3)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_BOSS, CAbstractFactory<CBoss>::Create(600.f, 320.f));
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_OBS, CAbstractFactory<CObstacle>::Create(300.f, 950.f, DIR_TYPE::DIR_LEFT));
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_OBS, CAbstractFactory<CObstacle>::Create(1950.f, 950.f, DIR_TYPE::DIR_RIGHT));
				CSoundMgr::Get_Inst()->PlayBGM(L"Boss.wav", 0.4f);
			}
				
		}
	}



	return 0;
}

void CScene_Boss::Late_Update(void)
{
	CObjMgr::Get_Inst()->Late_Update();

	if (!CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_OBS).empty() &&
		CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_BOSS).front()->Get_Dead() == true)
	{
		CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_OBS);
		CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
	}


	if (static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Get_StageClear() == true)
	{
		static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Off_StageClear();
		CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_CLEAR);
	}
}

void CScene_Boss::Render(HDC hDC)
{
	HDC		hGroundDC = CBmpMgr::Get_Inst()->Find_Image(L"BossStage");

	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	BitBlt(hDC, (int)fScrollX, (int)fScrollY, STAGE2CX, STAGE2CY, hGroundDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Inst()->Render(hDC);
}

void CScene_Boss::Release(void)
{
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_MONSTER);
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_TILE);
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_BOSS);
}
