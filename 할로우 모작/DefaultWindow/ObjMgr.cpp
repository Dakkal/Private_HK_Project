#include "stdafx.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "AbstractFactory.h"
#include "Monster_Fly.h"
#include "Monster_Mos.h"
#include "Monster_Husk.h"
#include "Monster_Knight.h"
#include "Monster_NoFly.h"

CObjMgr*	CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::AddObj(OBJECT_TYPE _eType, CObj * _pObj)
{
	if (OBJECT_TYPE::END <= _eType || _pObj == nullptr)
		return;

	m_ObjList[(int)_eType].push_back(_pObj);
}

void CObjMgr::Update(void)
{
	Set_Channel();

	for (size_t i = 0; i < (int)OBJECT_TYPE::END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end();)
		{
			int iEvent = (*iter)->Update();

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

}

void CObjMgr::Late_Update()
{
	for (size_t i = 0; i < (size_t)OBJECT_TYPE::END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Late_Update();
	}

	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_PLAYER], m_ObjList[(int)OBJECT_TYPE::OBJ_MONSTER]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_PLAYER], m_ObjList[(int)OBJECT_TYPE::OBJ_BOSS]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_PLAYER], m_ObjList[(int)OBJECT_TYPE::OBJ_ENEMYCOLLI]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_PLAYER], m_ObjList[(int)OBJECT_TYPE::OBJ_ENEMYPROJECTILE]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_MONSTER], m_ObjList[(int)OBJECT_TYPE::OBJ_COLLI]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_MONSTER], m_ObjList[(int)OBJECT_TYPE::OBJ_PROJECTILE]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_BOSS], m_ObjList[(int)OBJECT_TYPE::OBJ_COLLI]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_BOSS], m_ObjList[(int)OBJECT_TYPE::OBJ_PROJECTILE]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_COLLI], m_ObjList[(int)OBJECT_TYPE::OBJ_ENEMYPROJECTILE]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_COLLI], m_ObjList[(int)OBJECT_TYPE::OBJ_ENEMYCOLLI]);
	CCollisionMgr::Collision_Rect(m_ObjList[(int)OBJECT_TYPE::OBJ_COLLI], m_ObjList[(int)OBJECT_TYPE::OBJ_OBS]);

	CCollisionMgr::Collision_RectEx(m_ObjList[(int)OBJECT_TYPE::OBJ_PLAYER], m_ObjList[(int)OBJECT_TYPE::OBJ_OBS]);
}

void CObjMgr::Render(HDC hDc)
{
	for (size_t i = 0; i < (size_t)OBJECT_TYPE::END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render(hDc);
	}
}

void CObjMgr::Release(void)
{
	for (size_t i = 0; i < (size_t)OBJECT_TYPE::END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_DeleteObj());
		m_ObjList[i].clear();
	}

	g_FlyCount = 0;
	g_MosCount = 0;
	g_HuskCount = 0;
	g_KnightCount = 0;
	g_NoFlyCount = 0;
}

CObj * CObjMgr::Get_Target(OBJECT_TYPE eType, CObj * pObj)
{
	if (m_ObjList[(int)eType].empty())
		return nullptr;

	CObj*	pTarget = nullptr;
	float	fDistance = 0.f;

	for (auto& iter : m_ObjList[(int)eType])
	{
		if (iter->Get_Dead())
			continue;

		float	fWidth = fabs(iter->Get_Pos().fX - pObj->Get_Pos().fX);
		float	fHeight = fabs(iter->Get_Pos().fY - pObj->Get_Pos().fY);

		float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || fDistance > fDiagonal)
		{
			pTarget = iter;
			fDistance = fDiagonal;
		}
	}

	return pTarget;
}

void CObjMgr::AddMon(CObj * _pObj)
{
	if (_pObj == nullptr)
		return;

	m_vecMon.push_back(_pObj);
}

void CObjMgr::Update_Mon()
{
	if (!m_vecMon.empty())
	{
		for (auto& iter : m_vecMon)
			iter->Update_Rect();
	}
}

void CObjMgr::Late_Update_Mon()
{
}

void CObjMgr::Render_Mon(HDC hDc)
{
	if (!m_vecMon.empty())
	{
		for (auto& iter : m_vecMon)
			iter->Render(hDc);
	}
	
}

void CObjMgr::Delete_Objlist(OBJECT_TYPE _eObjType)
{
	for (auto& iter : m_ObjList[(int)_eObjType])
		Safe_Delete(iter);

	m_ObjList[(int)_eObjType].clear();

	g_FlyCount = 0;
	g_MosCount = 0;
	g_HuskCount = 0;
	g_KnightCount = 0;
	g_NoFlyCount = 0;
}

void CObjMgr::Delete_vecMon()
{
	for (auto& iter : m_vecMon)
		Safe_Delete(iter);

	m_vecMon.clear();
}

void CObjMgr::Save_Monster(void)
{
	if (CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"Stage1")
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage1_Monster.dat",
			GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("save File"), L"Fail", MB_OK);
			return;
		}

		int iMonType = 0;
		DWORD	dwByte = 0;

		for (auto& iter : m_vecMon)
		{
			iMonType = int(iter->Get_MonType());

			WriteFile(hFile, &(iter->Get_Pos()), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"Stage2")
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage2_Monster.dat",
			GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("save File"), L"Fail", MB_OK);
			return;
		}

		int iMonType = 0;
		DWORD	dwByte = 0;

		for (auto& iter : m_vecMon)
		{
			iMonType = int(iter->Get_MonType());

			WriteFile(hFile, &(iter->Get_Pos()), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"BossStage")
	{
		HANDLE	hFile = CreateFile(L"../Data/BossStage_Monster.dat",
			GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("save File"), L"Fail", MB_OK);
			return;
		}

		int iMonType = 0;
		DWORD	dwByte = 0;

		for (auto& iter : m_vecMon)
		{
			iMonType = int(iter->Get_MonType());

			WriteFile(hFile, &(iter->Get_Pos()), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}

void CObjMgr::Load_Monster(void)
{
	if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE)
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage1_Monster.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
			return;
		}

		INFO	tInfo{};
		int iMonType = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			if (iMonType == (int)MONSTER_TYPE::FLY)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Fly>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::MOS)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Mos>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::HUSK)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Husk>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::KNIGHT)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Knight>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::NOMOVE_FLY)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_NoFly>::Create(tInfo.fX, tInfo.fY));
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE2)
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage2_Monster.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
			return;
		}

		INFO	tInfo{};
		int iMonType = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			if (iMonType == (int)MONSTER_TYPE::FLY)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Fly>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::MOS)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Mos>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::HUSK)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Husk>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::KNIGHT)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Knight>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::NOMOVE_FLY)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_NoFly>::Create(tInfo.fX, tInfo.fY));
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_BOSS ||
		CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"BossStage")
	{
		HANDLE	hFile = CreateFile(L"../Data/BossStage_Monster.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
			return;
		}

		INFO	tInfo{};
		int iMonType = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			if (iMonType == (int)MONSTER_TYPE::FLY)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Fly>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::MOS)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Mos>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::HUSK)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Husk>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::KNIGHT)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_Knight>::Create(tInfo.fX, tInfo.fY));
			else if (iMonType == (int)MONSTER_TYPE::NOMOVE_FLY)
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_MONSTER, CAbstractFactory<CMonster_NoFly>::Create(tInfo.fX, tInfo.fY));
		}

		CloseHandle(hFile);
	}
}

void CObjMgr::Set_Channel()
{
	if (m_ObjList[(int)OBJECT_TYPE::OBJ_MONSTER].empty())
	{
		g_FlyCount = 0;
		g_MosCount = 0;
		g_HuskCount = 0;
		g_KnightCount = 0;
		g_NoFlyCount = 0;
		return;
	}
		


	for (auto& iter : m_ObjList[(int)OBJECT_TYPE::OBJ_MONSTER])
	{
		switch (iter->Get_MonType())
		{
		case MONSTER_TYPE::FLY:
			switch (g_FlyCount)
			{
			case 0:
				iter->Set_Channel(FLYSOUND_LOOP_0);
				++g_FlyCount;
				break;
			case 1:
				iter->Set_Channel(FLYSOUND_LOOP_1);
				++g_FlyCount;
				break;
			case 2:
				iter->Set_Channel(FLYSOUND_LOOP_2);
				++g_FlyCount;
				break;
			case 3:
				iter->Set_Channel(FLYSOUND_LOOP_3);
				++g_FlyCount;
				break;
			case 4:
				iter->Set_Channel(FLYSOUND_LOOP_4);
				++g_FlyCount;
				break;
			default:
				break;
			}
			break;
		case MONSTER_TYPE::MOS:
			switch (g_MosCount)
			{
			case 0:
				iter->Set_Channel(MOSSOUND_LOOP_0);
				++g_MosCount;
				break;
			case 1:
				iter->Set_Channel(MOSSOUND_LOOP_1);
				++g_MosCount;
				break;
			case 2:
				iter->Set_Channel(MOSSOUND_LOOP_2);
				++g_MosCount;
				break;
			case 3:
				iter->Set_Channel(MOSSOUND_LOOP_3);
				++g_MosCount;
				break;
			case 4:
				iter->Set_Channel(MOSSOUND_LOOP_4);
				++g_MosCount;
				break;
			default:
				break;
			}
			break;
			break;
		case MONSTER_TYPE::HUSK:
			break;
		case MONSTER_TYPE::KNIGHT:
			break;
		case MONSTER_TYPE::NOMOVE_FLY:
			switch (g_NoFlyCount)
			{
			case 0:
				iter->Set_Channel(NOFLYSOUND_LOOP_0);
				++g_NoFlyCount;
				break;
			case 1:
				iter->Set_Channel(NOFLYSOUND_LOOP_1);
				++g_NoFlyCount;
				break;
			case 2:
				iter->Set_Channel(NOFLYSOUND_LOOP_2);
				++g_NoFlyCount;
				break;
			case 3:
				iter->Set_Channel(NOFLYSOUND_LOOP_3);
				++g_NoFlyCount;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
		




	}





}

//void CObjMgr::Load_EditMonster()
//{
//	if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE ||
//		CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"Stage")
//	{
//		HANDLE	hFile = CreateFile(L"../Data/Stage_Monster.dat",
//			GENERIC_READ,
//			NULL,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL,
//			NULL);
//
//		if (INVALID_HANDLE_VALUE == hFile)
//		{
//			MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
//			return;
//		}
//
//		INFO	tInfo{};
//		int iMonType = 0;
//		DWORD	dwByte = 0;
//
//		while (true)
//		{
//			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
//			ReadFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);
//
//			if (0 == dwByte)
//				break;
//
//			if (iMonType == (int)MONSTER_TYPE::FLY)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Fly>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::MOS)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Mos>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::HUSK)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Husk>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::KNIGHT)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Knight>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::NOMOVE_FLY)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_NoFly>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//		}
//
//		CloseHandle(hFile);
//	}
//	else if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_BOSS ||
//		CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"BossStage")
//	{
//		HANDLE	hFile = CreateFile(L"../Data/BossStage_Monster.dat",
//			GENERIC_READ,
//			NULL,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL,
//			NULL);
//
//		if (INVALID_HANDLE_VALUE == hFile)
//		{
//			MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
//			return;
//		}
//
//		INFO	tInfo{};
//		int iMonType = 0;
//		DWORD	dwByte = 0;
//
//		while (true)
//		{
//			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
//			ReadFile(hFile, &iMonType, sizeof(int), &dwByte, nullptr);
//
//			if (0 == dwByte)
//				break;
//
//			if (iMonType == (int)MONSTER_TYPE::FLY)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Fly>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::MOS)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Mos>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::HUSK)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Husk>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::KNIGHT)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_Knight>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//			else if (iMonType == (int)MONSTER_TYPE::NOMOVE_FLY)
//			{
//				CObj* pMon = CAbstractFactory<CMonster_NoFly>::Create(tInfo.fX, tInfo.fY);
//				m_vecMon.push_back(pMon);
//			}
//		}
//
//		CloseHandle(hFile);
//	}
//}


