#pragma once
#include "Obj.h"
class CMonster_Knight :
	public CObj
{
public:
	CMonster_Knight();
	virtual ~CMonster_Knight();

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
	void    IsParring() { m_bIsParring = true; }

private:
	void	Detect_Player();
	void	Get_Damaged(int _iDamage) { m_tStat.iHp -= _iDamage; }
	void	Down();
	void	Guard();
	void	Attack();
	void	Target_Player();
	void	Set_PlayerDis();
	void	Grogy();
	void	Detect_Sound();

private:
	bool			m_bIsParring;

	DWORD			m_dwAttackTime;

	DIR_TYPE		m_ePreDir;
	DIR_TYPE		m_eCurDir;

	MONSTER_STATE	m_eCurMonState;
	MONSTER_STATE	m_ePreMonState;

	POINT	m_tDetect;
	RECT	m_tDetectRect;

	float	m_fPlayerDis;

	bool	m_bDetect;
	bool	m_bAttack;
	bool	m_bGuard;
	bool    m_bSwitchPos;

	CObj*   m_pColi;
	bool	m_bGrogy;

	bool	m_bIsSound;

};

