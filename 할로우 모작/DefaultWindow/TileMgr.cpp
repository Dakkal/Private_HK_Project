#include "stdafx.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"

CTileMgr*		CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize(void)
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float	fX = float(TILECX * j) + (TILECX >> 1);
			float	fY = float(TILECY * i) + (TILECY >> 1);

			CObj*		pObj = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pObj);
		}
	}

}

void CTileMgr::Update(void)
{
	for (auto& iter : m_vecTile)
		iter->Update();
}

void CTileMgr::Late_Update(void)
{
	for (auto& iter : m_vecTile)
		iter->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	int	iCullX = abs((int)CScrollMgr::Get_Inst()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollMgr::Get_Inst()->Get_ScrollY() / TILECY);

	int	iMaxX = iCullX + WINCX / TILECX + 50;
	int	iMaxY = iCullY + WINCY / TILECY + 50;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int	iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}

}

void CTileMgr::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_DeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Picking_Tile(POINT _pt, int _iDrawID)
{
	int		j = _pt.x / TILECX;
	int		i = _pt.y / TILECY;

	int		iIndex = i * TILEX + j;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_Tile(_iDrawID);
}

void CTileMgr::Save_Tile(void)
{
	if (CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"Stage1")
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage1_Tile.dat",
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

		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		for (auto& iter : m_vecTile)
		{
			iDrawID = dynamic_cast<CTile*>(iter)->Get_DrawID();

			WriteFile(hFile, &(iter->Get_Pos()), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"Stage2")
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage2_Tile.dat",
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

		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		for (auto& iter : m_vecTile)
		{
			iDrawID = dynamic_cast<CTile*>(iter)->Get_DrawID();

			WriteFile(hFile, &(iter->Get_Pos()), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"BossStage")
	{
		HANDLE	hFile = CreateFile(L"../Data/BossStage_Tile.dat",
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

		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		for (auto& iter : m_vecTile)
		{
			iDrawID = dynamic_cast<CTile*>(iter)->Get_DrawID();

			WriteFile(hFile, &(iter->Get_Pos()), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	
}

void CTileMgr::Load_EditTile()
{
	if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE ||
		CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"Stage")
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage_Tile.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)	// 파일 개방에 실패했다면
		{
			MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
			return;
		}

		INFO	tInfo{};
		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj*	pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
			dynamic_cast<CTile*>(pObj)->Set_Tile(iDrawID);

			m_vecTile.push_back(pObj);
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_BOSS ||
		CSceneMgr::Get_Inst()->Get_Scene()->Get_StageKey() == L"BossStage")
	{
		HANDLE	hFile = CreateFile(L"../Data/BossStage_Tile.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)	// 파일 개방에 실패했다면
		{
			MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
			return;
		}

		INFO	tInfo{};
		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj*	pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
			dynamic_cast<CTile*>(pObj)->Set_Tile(iDrawID);

			m_vecTile.push_back(pObj);
		}

		CloseHandle(hFile);
	}

}

void CTileMgr::Load_Tile(void)
{
	if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE)
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage1_Tile.dat",
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
		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj*	pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
			dynamic_cast<CTile*>(pObj)->Set_Tile(iDrawID);

			if (static_cast<CTile*>(pObj)->Get_DrawID() == 0)
			{
				Safe_Delete(pObj);
			}
			else if (static_cast<CTile*>(pObj)->Get_DrawID() == 1)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_TILE, pObj);
			}
			else if (static_cast<CTile*>(pObj)->Get_DrawID() == 2)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_TILE, pObj);
			}
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_STAGE2)
	{
		HANDLE	hFile = CreateFile(L"../Data/Stage2_Tile.dat",
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
		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj*	pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
			dynamic_cast<CTile*>(pObj)->Set_Tile(iDrawID);

			if (static_cast<CTile*>(pObj)->Get_DrawID() == 0)
			{
				Safe_Delete(pObj);
			}
			else if (static_cast<CTile*>(pObj)->Get_DrawID() == 1)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_TILE, pObj);
			}
			else if (static_cast<CTile*>(pObj)->Get_DrawID() == 2)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_TILE, pObj);
			}
		}

		CloseHandle(hFile);
	}
	else if (CSceneMgr::Get_Inst()->Get_SceneType() == SCENE_TYPE::SC_BOSS)
	{
		HANDLE	hFile = CreateFile(L"../Data/BossStage_Tile.dat",
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
		int		iDrawID = 0, iOption = 0;
		DWORD	dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CObj*	pObj = CAbstractFactory<CTile>::Create(tInfo.fX, tInfo.fY);
			dynamic_cast<CTile*>(pObj)->Set_Tile(iDrawID);

			if (static_cast<CTile*>(pObj)->Get_DrawID() == 0)
			{
				Safe_Delete(pObj);
			}
			else if (static_cast<CTile*>(pObj)->Get_DrawID() == 1)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_TILE, pObj);
			}
			else if (static_cast<CTile*>(pObj)->Get_DrawID() == 2)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_TILE, pObj);
			}
		}

		CloseHandle(hFile);
	}

}