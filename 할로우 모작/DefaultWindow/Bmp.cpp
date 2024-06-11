#include "stdafx.h"
#include "Bmp.h"


CBmp::CBmp()
{
}


CBmp::~CBmp()
{
	Release();
}

void CBmp::Load_Bmp(const TCHAR * pFilePath)
{
	HDC hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL, pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hOldmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
