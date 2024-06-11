#pragma once
#include "Obj.h"
class CEnergyPo_Coli :
	public CObj
{
public:
	CEnergyPo_Coli();
	virtual ~CEnergyPo_Coli();

	// Inherited via CObj
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

	virtual void    Tile_Collision() override;
	virtual void	Collider(CObj* _pObj, COL_DIR eDir, float fPush) override;
	virtual void	OnCollision(CObj* _pObj, RECT _rc) override;

private:
	void		Set_Motion();

private:
	float		m_fDistance;
};

