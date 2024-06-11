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
	m_hVideo = MCIWndCreate(g_hWnd,			// �θ� ������ �ڵ�
		nullptr,		// mci �����츦 ����ϴ� �ν��Ͻ� �ڵ�
		WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, // WS_CHILD : �ڽ� â, WS_VISIBLE : �� ��� ȭ�鿡 �ÿ�, MCIWNDF_NOPLAYBAR : �÷��� �ٸ� �������� ����
		L"../Video/Intro.wmv");	// ����� ������ ���

									//MoveWindow : �������� ����� ũ�⸦ ����
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
