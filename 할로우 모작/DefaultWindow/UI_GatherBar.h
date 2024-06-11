#pragma once
#include "Obj.h"
class CUI_GatherBar :
	public CObj
{
public:
	CUI_GatherBar();
	virtual ~CUI_GatherBar();

	// Inherited via CObj
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

	virtual void	Set_Motion() override;
	virtual void	Collider(CObj * _pObj, COL_DIR eDir, float fPush) override {}
	virtual void	Tile_Collision() override {}
	virtual void	OnCollision(CObj* _pObj, RECT _rc) override {}

public:
	BAR_STATE		Get_BarState() { return m_eCurState; }

private:
	BAR_STATE		m_ePreState;
	BAR_STATE		m_eCurState;
};

