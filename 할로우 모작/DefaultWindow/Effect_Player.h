#pragma once
#include "Obj.h"
class CEffect_Player :
	public CObj
{
public:
	CEffect_Player();
	virtual ~CEffect_Player();

	// Inherited via CObj
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
	virtual void Collider(CObj * _pObj, COL_DIR eDir, float fPush) override {}
	virtual void Tile_Collision() override {}
	virtual void Set_Motion() override {}
	virtual void OnCollision(CObj * _pObj, RECT _rc) override {}

public:
	void	Set_Effect(EFFECT_PLAYER _eEffect) { m_ePlayerEffect = _eEffect; }


private:
	EFFECT_PLAYER		m_ePlayerEffect;
	bool				m_bIsEffect;

};

