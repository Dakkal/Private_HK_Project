#pragma once

#include "Scene.h"
class CSceneMgr
{
public:
	CSceneMgr();
	~CSceneMgr();

public:
	void		Scene_Change(SCENE_TYPE eID);
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);

	
public:
	SCENE_TYPE	Get_SceneType() { return m_eCurScene; }
	CScene*		Get_Scene()		{ return m_pScene; }

public:
	static CSceneMgr*		Get_Inst(void)
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

		return m_pInstance;
	}
	static void		Destroy_Inst(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}

	}
private:
	static CSceneMgr*		m_pInstance;
	CScene*					m_pScene;

	SCENE_TYPE				m_eCurScene;
	SCENE_TYPE				m_ePreScene;

};

