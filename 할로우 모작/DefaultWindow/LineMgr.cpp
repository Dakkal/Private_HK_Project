#include "stdafx.h"
#include "LineMgr.h"
#include "KeyMgr.h"

CLineMgr*		CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	LINEPOINT	tLine[4]{

		{ 100.f, 450.f },
		{ 300.f, 450.f },
		{ 500.f, 550.f },
		{ 2000.f, 550.f }
	};

	LINEPOINT	taLine[4]{

		{ 100.f, 350.f },
		{ 300.f, 350.f },
		{ 500.f, 250.f },
		{ 700.f, 250.f }
	};

	LINEPOINT	tbLine[2]{

		{ 100.f, 100.f },
		{ 400.f, 100.f }
	};

	LINEPOINT	tcLine[2]{

		{ 700.f, 100.f },
		{ 700.f, 0.f }
	};

	m_LineList.push_back(new CLine(tLine[0], tLine[1], LINE_TYPE::LINE_BOTTOM));
	m_LineList.push_back(new CLine(tLine[1], tLine[2], LINE_TYPE::LINE_BOTTOM));
	m_LineList.push_back(new CLine(tLine[2], tLine[3], LINE_TYPE::LINE_BOTTOM));
	m_LineList.push_back(new CLine(taLine[0], taLine[1], LINE_TYPE::LINE_REGULAR));
	m_LineList.push_back(new CLine(taLine[1], taLine[2], LINE_TYPE::LINE_REGULAR));
	m_LineList.push_back(new CLine(taLine[2], taLine[3], LINE_TYPE::LINE_REGULAR));
	m_LineList.push_back(new CLine(tbLine[0], tbLine[1], LINE_TYPE::LINE_GRAB));
	m_LineList.push_back(new CLine(tcLine[0], tcLine[1], LINE_TYPE::LINE_GRAB));

	//Load_LineData();		
}

void CLineMgr::Update()
{
}

void CLineMgr::Late_Update()
{
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& iter : m_LineList)
		iter->Render(hDC);
}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), Safe_DeleteObj());
	m_LineList.clear();
}

void CLineMgr::Load_LineData()
{
	//HANDLE	hFile = CreateFile(L"../Data/Line.dat",		// ���� ��ο� ���� �̸��� ���
	//	GENERIC_READ,			// ���� ���� ���(GENERIC_WRITE : ���� ��� GENERIC_READ : �б� ���)
	//	NULL,					// ���� ��� ����(NULL�� �����ϸ� �������� ����)
	//	NULL,					// ���� �Ӽ� ����(NULL�� �⺻ ���� �Ӽ� ����)
	//	OPEN_EXISTING,			// ���� �ɼ�( CREATE_ALWAYS(���� ����) : ������ ���ٸ� ����, �ִٸ� ����� �ɼ�, OPEN_EXISTING(�б� ����) : ������ ���� ��쿡�� �ҷ����� ����)
	//	FILE_ATTRIBUTE_NORMAL,	// ���� �Ӽ�(�б� ����, ���� ��) : �ƹ��� �Ӽ��� ���� �Ϲ� ���� ����
	//	NULL);					// ������ ������ �Ӽ��� ������ ���ø�, �츮�� �Ⱦ��ϱ� NULL

	//if (INVALID_HANDLE_VALUE == hFile)	// ���� ���濡 �����ߴٸ�
	//{
	//	MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
	//	return;
	//}

	//DWORD	dwByte = 0;
	//LINE	tInfo{};

	//while (true)
	//{
	//	ReadFile(hFile, &tInfo, sizeof(LINE), &dwByte, nullptr);

	//	if (0 == dwByte)
	//		break;

	//	m_LineList.push_back(new CLine(tInfo));
	//}

	//CloseHandle(hFile);

	//MessageBox(g_hWnd, _T("Load success"), L"Success", MB_OK);
}

bool CLineMgr::Collision_Line(float & fX, float & fY)
{
	if (m_LineList.empty())
		return false;

	CLine*		pTarget = nullptr;

	for (auto& iter : m_LineList)
	{
		if (fX >= iter->Get_LINE().tLPoint.fX &&
			fX <= iter->Get_LINE().tRPoint.fX &&
			fY >= iter->Get_LINE().tLPoint.fY &&
			fY <= iter->Get_LINE().tRPoint.fY)
		{
			pTarget = iter;
		}
		else if (fX >= iter->Get_LINE().tLPoint.fX &&
			fX <= iter->Get_LINE().tRPoint.fX &&
			fY <= iter->Get_LINE().tLPoint.fY &&
			fY >= iter->Get_LINE().tRPoint.fY)
		{
			pTarget = iter;
		}
	}


	if (!pTarget)
		return false;

	float	x1 = pTarget->Get_LINE().tLPoint.fX;
	float	x2 = pTarget->Get_LINE().tRPoint.fX;
	float	y1 = pTarget->Get_LINE().tLPoint.fY;
	float	y2 = pTarget->Get_LINE().tRPoint.fY;

	fY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

bool CLineMgr::Collision_LineJump(float & fX, float & fY, float & fCmpY)
{
	if (m_LineList.empty())
		return false;

	CLine*		pTarget = nullptr;

	for (auto& iter : m_LineList)
	{
		if (fX >= iter->Get_LINE().tLPoint.fX &&
			fX <= iter->Get_LINE().tRPoint.fX &&
			fY >= iter->Get_LINE().tLPoint.fY &&
			fY >= iter->Get_LINE().tRPoint.fY &&
			fCmpY <= iter->Get_LINE().tLPoint.fY &&
			fCmpY <= iter->Get_LINE().tRPoint.fY )
		{
			pTarget = iter;
		}
	}


	if (!pTarget)
		return false;

	float	x1 = pTarget->Get_LINE().tLPoint.fX;
	float	x2 = pTarget->Get_LINE().tRPoint.fX;
	float	y1 = pTarget->Get_LINE().tLPoint.fY;
	float	y2 = pTarget->Get_LINE().tRPoint.fY;

	fY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

bool CLineMgr::Collision_LineDown(float & fX, float & fY, float & fCmpY)
{
	if (m_LineList.empty())
		return false;

	CLine*		pTarget = nullptr;

	for (auto& iter : m_LineList)
	{
		if (fX >= iter->Get_LINE().tLPoint.fX &&
			fX <= iter->Get_LINE().tRPoint.fX &&
			fY <= iter->Get_LINE().tLPoint.fY &&
			fY <= iter->Get_LINE().tRPoint.fY &&
			fCmpY >= iter->Get_LINE().tLPoint.fY &&
			fCmpY >= iter->Get_LINE().tRPoint.fY)
		{
			pTarget = iter;
		}
	}


	if (!pTarget)
		return false;

	float	x1 = pTarget->Get_LINE().tLPoint.fX;
	float	x2 = pTarget->Get_LINE().tRPoint.fX;
	float	y1 = pTarget->Get_LINE().tLPoint.fY;
	float	y2 = pTarget->Get_LINE().tRPoint.fY;

	fY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

bool CLineMgr::Collision_LineDownJump(float & fX, float & fY, float & fCmpY)
{
	if (m_LineList.empty())
		return false;

	CLine*		pTarget = nullptr;

	for (auto& iter : m_LineList)
	{
		if (fX >= iter->Get_LINE().tLPoint.fX &&
			fX <= iter->Get_LINE().tRPoint.fX &&
			fY <= iter->Get_LINE().tLPoint.fY &&
			fY <= iter->Get_LINE().tRPoint.fY &&
			fCmpY <= iter->Get_LINE().tLPoint.fY &&
			fCmpY <= iter->Get_LINE().tRPoint.fY)
		{
			pTarget = iter;
		}
	}


	if (!pTarget)
		return false;

	float	x1 = pTarget->Get_LINE().tLPoint.fX;
	float	x2 = pTarget->Get_LINE().tRPoint.fX;
	float	y1 = pTarget->Get_LINE().tLPoint.fY;
	float	y2 = pTarget->Get_LINE().tRPoint.fY;

	fY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}
