#pragma once
#include "Obj.h"
class CMenuUI :
	public CObj
{
public:
	CMenuUI();
	virtual ~CMenuUI();

	// Inherited via CObj
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
	virtual void Collider(CObj * _pObj, COL_DIR eDir, float fPush) override;
	virtual void Tile_Collision() override;
	virtual void Set_Motion() override;
	virtual void OnCollision(CObj * _pObj, RECT _rc) override;

public:
	void		Set_MenuType(MENU_TYPE _eType) { m_eMenu = _eType; }
	MENU_TYPE	Get_MenuType() { return m_eMenu; }

private:
	MENU_TYPE	m_eMenu;


};

