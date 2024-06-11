#include "stdafx.h"
#include "Scene_Clear.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"

CScene_Clear::CScene_Clear()
{
}


CScene_Clear::~CScene_Clear()
{
	Release();
}

void CScene_Clear::Initialize(void)
{
	m_hVideo = MCIWndCreate(g_hWnd,			// 부모 윈도우 핸들
		nullptr,		// mci 윈도우를 사용하는 인스턴스 핸들
		WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, // WS_CHILD : 자식 창, WS_VISIBLE : 그 즉시 화면에 시연, MCIWNDF_NOPLAYBAR : 플레이 바를 생성하지 않음
		L"../Video/Intro.wmv");	// 재생할 파일의 경로

									//MoveWindow : 동영상을 재생할 크기를 설정
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);

	MCIWndPlay(m_hVideo);
}

int CScene_Clear::Update(void)
{
	return 0;
}

void CScene_Clear::Late_Update(void)
{
	if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo)
		|| CKeyMgr::Get_Inst()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Inst()->Scene_Change(SCENE_TYPE::SC_MENU);
	}
}

void CScene_Clear::Render(HDC hDC)
{
}

void CScene_Clear::Release(void)
{
	MCIWndClose(m_hVideo);
	CObjMgr::Get_Inst()->Release();
}
