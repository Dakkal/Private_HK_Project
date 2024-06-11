#pragma once
#include "Obj.h"
class CMonKnighht_Coli :
	public CObj
{
public:
	CMonKnighht_Coli();
	virtual ~CMonKnighht_Coli();

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
	void	Set_Owner(CObj* _pObj) { m_pOwner = _pObj; }

	BOOL	Get_IsParring() { return m_bIsParring; }

private:
	bool	m_bIsParring;
	CObj*	m_pOwner;
};

