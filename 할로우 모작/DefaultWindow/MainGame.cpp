#include "stdafx.h"
#include "MainGame.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "SceneMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"

CMainGame::CMainGame()
{
	m_dwTime = GetTickCount();
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize(void)
{
	m_DC = GetDC(g_hWnd);

	CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_LOGO);
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"Back");
	CSoundMgr::Get_Inst()->Initialize();

}

void CMainGame::Update(void)
{
	CSceneMgr::Get_Inst()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Inst()->Late_Update();
	CScrollMgr::Get_Inst()->Scroll_Lock();
}

void CMainGame::Render()
{
	if (SCENE_TYPE::SC_CLEAR == CSceneMgr::Get_Inst()->Get_SceneType())
		return;

	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(L"Back");
	
	BitBlt(m_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	
	CSceneMgr::Get_Inst()->Render(hMemDC);
}

void CMainGame::Release(void)
{
	CScrollMgr::Get_Inst()->Destory_Inst();
	CKeyMgr::Get_Inst()->Destory_Inst();
	CBmpMgr::Get_Inst()->Destory_Inst();
	CSceneMgr::Get_Inst()->Destroy_Inst();
	CObjMgr::Get_Inst()->Destroy_Inst();
	CTileMgr::Get_Inst()->Destroy_Inst();
	CSoundMgr::Get_Inst()->Destroy_Inst();
	
	ReleaseDC(g_hWnd, m_DC);
	
}

