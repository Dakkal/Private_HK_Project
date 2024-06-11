#pragma once
#include "Obj.h"
class CBoss :
	public CObj
{
public:
	CBoss();
	virtual ~CBoss();

	// Inherited via CObj
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
	virtual void Collider(CObj * _pObj, COL_DIR eDir, float fPush) override;
	virtual void Tile_Collision() override;
	virtual void Set_Motion() override;
	virtual void OnCollision(CObj* _pObj, RECT _rc) override;

private:
	void	Target_Player();
	void	Down();
	void	Set_PlayerDis();

	void	Action();
	void	Pattern();

	void	EvadeJump();
	void	CloseJump();
	void	EndJump();

	void	Pattern1();
	void	Pattern2();

	void	Grogy();
	void	Get_Damaged(int _iDamage);
	void    Dead();
	

private:
	int				m_iArmorHP;

	BOSS_STATE		m_eCurState;
	BOSS_STATE		m_ePreState;

	DIR_TYPE		m_ePreDir;
	DIR_TYPE		m_eCurDir;

	DWORD			m_dwIsActionTime;
	DWORD			m_dwIsPatternTime;

	DWORD			m_dwPattern2Time;
	DWORD			m_dwPattern2ColiTime;
	DWORD			m_dwGrogyTime;

	float			m_fPlayerDis;
	bool			m_bIsAction;
	bool			m_bIsPattern;
	bool			m_bIsGrogy;

	bool			m_bEvadeJump;
	bool			m_bCloseJump;

	bool			m_bPattern1;
	bool			m_bPattern2;

	bool			m_bDisEnd;

};

