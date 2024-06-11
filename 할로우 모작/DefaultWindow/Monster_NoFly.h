#pragma once
#include "Obj.h"
class CMonster_NoFly :
	public CObj
{
public:
	CMonster_NoFly();
	virtual ~CMonster_NoFly();

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

private:
	void	Detect_Player();
	void	Get_Damaged(int _iDamage) { m_tStat.iHp -= _iDamage; }

	void	Detect_Sound();

private:
	MONSTER_STATE	m_eCurMonState;
	MONSTER_STATE	m_ePreMonState;

	POINT	m_tDetect;
	RECT	m_tDetectRect;

	bool	m_bDetect;

	bool	m_bIsSound;
	bool	m_bSoundPlay;
};

