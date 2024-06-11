#include "stdafx.h"
#include "Tile.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"


CTile::CTile()
	: m_iDrawID(0)
{
}


CTile::~CTile()
{
	Release();
}

void CTile::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_TILE;

	m_eRender = RENDER_TYPE::RENDER_BACKGROUND;

	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

}

int CTile::Update(void)
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	/*if (m_iDrawID == 1)
	{
		Rectangle(hDC,
			(int)m_tRect.left + fScrollX,
			(int)m_tRect.top + fScrollY,
			(int)m_tRect.right + fScrollX,
			(int)m_tRect.bottom + fScrollY);
	}

	if (m_iDrawID == 2)
	{
		Ellipse(hDC,
			(int)m_tRect.left + fScrollX,
			(int)m_tRect.top + fScrollY,
			(int)m_tRect.right + fScrollX,
			(int)m_tRect.bottom + fScrollY);
	}*/
}

void CTile::Release(void)
{
}

void CTile::Tile_Collision()
{
}

void CTile::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	OBJECT_TYPE eType = _pObj->Get_ObjType();
	CObj* pColi = _pObj;


	switch (eType)
	{
	case OBJECT_TYPE::OBJ_PLAYER:
		switch (eDir)
		{
		case COL_DIR::LEFT:
			//Set_PosX(-fPush);
			break;
		case COL_DIR::TOP:
			//Set_PosY(fPush);
			break;
		case COL_DIR::RIGHT:
			//Set_PosX(fPush);
			break;
		case COL_DIR::BOTTOM:
			//Set_PosY(-fPush);
			break;
		}
		break;

	}
}

void CTile::Set_Motion()
{
}
