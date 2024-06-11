#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse()
{
}


CMouse::~CMouse()
{
}

void CMouse::Initialize(void)
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
}

int CMouse::Update(void)
{
	POINT	pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;

	ShowCursor(false);

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC hDC)
{
	Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMouse::Release(void)
{
}
