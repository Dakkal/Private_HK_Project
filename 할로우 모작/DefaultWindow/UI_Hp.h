#pragma once
#include "Obj.h"
class CUI_Hp :
	public CObj
{
public:
	CUI_Hp();
	virtual ~CUI_Hp();

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

public:
	void	Set_CurHpState(HP_STATE _eState) { m_eCurState = _eState; }

private:
	HP_STATE	m_ePreState;
	HP_STATE 	m_eCurState;

};

