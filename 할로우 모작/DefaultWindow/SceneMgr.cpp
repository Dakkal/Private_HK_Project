#include "stdafx.h"

#include "SceneMgr.h"
#include "Scene_Logo.h"
#include "Scene_Menu.h"
#include "Scene_Stage1.h"
#include "Scene_Stage2.h"
#include "Scene_Boss.h"
#include "Scene_Clear.h"
#include "Scene_Edit.h"

CSceneMgr*		CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENE_TYPE eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		if(m_pScene != nullptr)
			Safe_Delete<CScene*>(m_pScene);

		switch (m_eCurScene)
		{
		case SCENE_TYPE::SC_LOGO:
			m_pScene = new CScene_Logo;
			break;

		case SCENE_TYPE::SC_MENU:
			m_pScene = new CScene_Menu;
			break;

		case SCENE_TYPE::SC_STAGE:
			m_pScene = new CScene_Stage1;
			break;

		case SCENE_TYPE::SC_STAGE2:
			m_pScene = new CScene_Stage2;
			break;

		case SCENE_TYPE::SC_BOSS:
			m_pScene = new CScene_Boss;
			break;

		case SCENE_TYPE::SC_CLEAR:
			m_pScene = new CScene_Clear;
			break;

		case SCENE_TYPE::SC_EDIT:
			m_pScene = new CScene_Edit;
			break;
		}

		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}

}

void CSceneMgr::Update(void)
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update(void)
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release(void)
{
	Safe_Delete(m_pScene);
}
