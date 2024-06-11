#include "stdafx.h"
#include "Scene_Stage1.h"
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

CScene_Stage1::CScene_Stage1()
{
}


CScene_Stage1::~CScene_Stage1()
{
	Release();
}

void CScene_Stage1::Initialize(void)
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

	CTileMgr::Get_Inst()->Load_Tile();
	CObjMgr::Get_Inst()->Load_Monster();
	CScrollMgr::Get_Inst()->Set_PoinScroll(-240.f, 0.f);

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Stage/TutoMap1.bmp", L"Stage1");
	CSoundMgr::Get_Inst()->PlayBGM(L"Stage.wav", 0.4f);
}

int CScene_Stage1::Update(void)
{
	CObjMgr::Get_Inst()->Update();


	//if (m_bPause)
	//{
	//	if (m_dwTime + 500.f <= GetTickCount())
	//	{
	//		m_bPause = false;
	//		m_dwTime = GetTickCount();
	//	}
	//	

	//}
	//if (!m_bPause)
	//{
	//	

	//	if (static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Get_Parring() == true)
	//	{
	//		static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Off_Parring();
	//		m_bPause = true;
	//		m_dwTime = GetTickCount();
	//	}

	//}
	//

	

	return 0;
}

void CScene_Stage1::Late_Update(void)
{
	CObjMgr::Get_Inst()->Late_Update();

	if (static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Get_StageClear() == true)
	{
		static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front())->Off_StageClear();
		CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_STAGE2);
	}
}

void CScene_Stage1::Render(HDC hDC)
{
	HDC		hGroundDC = CBmpMgr::Get_Inst()->Find_Image(L"Stage1");

	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	BitBlt(hDC, (int)fScrollX, (int)fScrollY, STAGE1CX, STAGE1CY, hGroundDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Inst()->Render(hDC);
}

void CScene_Stage1::Release(void)
{
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_MONSTER);
	CObjMgr::Get_Inst()->Delete_Objlist(OBJECT_TYPE::OBJ_TILE);
}
