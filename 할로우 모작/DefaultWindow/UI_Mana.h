#pragma once
#include "Obj.h"
class CUI_Mana :
	public CObj
{
public:
	CUI_Mana();
	virtual ~CUI_Mana();

	// Inherited via CObj
	virtual void	Initialize(void) override;
	virtual int		Update(void) override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void	Release(void) override;

	virtual void	Set_Motion() override;
	virtual void	Collider(CObj * _pObj, COL_DIR eDir, float fPush) override {}
	virtual void	Tile_Collision() override {}
	virtual void	OnCollision(CObj* _pObj, RECT _rc) override {}



private:
	MANA_STATE	m_ePreState;
	MANA_STATE 	m_eCurState;
};

