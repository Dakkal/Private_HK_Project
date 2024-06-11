#pragma once
#include "Obj.h"
class CPlayer :	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();
public:

	virtual void	Initialize() override;
	virtual int		Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void	Release() override;

	virtual void    Tile_Collision() override;
	virtual void	Collider(CObj* _pObj, COL_DIR eDir, float fPush) override;
	virtual void	Set_Motion() override;
	virtual void	OnCollision(CObj* _pObj, RECT _rc) override;

public:
	BOOL			Get_Start()		{ return m_bSceneStart; }
	BOOL			Get_Heal()		{ return m_bHeal; }
	BOOL			Get_EnergyPo()  { return m_bEnergyPo; }
	BOOL			Get_StageClear() { return m_bStageClear; }
	vector<CObj*>&	Get_vecHp()		{ return m_vecHp; }

public:
	void			Hit_Count()		{ ++m_iHitCount; }
	void			Parring_Count() { m_iHitCount += 5; }
	void			Is_ParringJump() { m_bParringJump = true; m_fTime = 0.f;}

	
	void			Off_StageClear() { m_bStageClear = false; }

	BOOL			Get_Parring() { return m_bParring; }
	void			On_Parring() { m_bParring = true; }
	void			Off_Parring() { m_bParring = false; }

	//카메라 쉐이킹ㅇ
	void			On_CameraShaking() { m_bCameraShaking = true; m_dwCameraTime = GetTickCount(); }
	void			Off_CameraShaking() { m_bCameraShaking = false; m_dwCameraTime = GetTickCount(); }

private:
	void	Key_Input();
	void	Jump();
	void	DoubleJump();
	void	ParringJump();
	void	Dash();
	void	Attack();
	void	Heal();
	void	Energy_Po();
	void	Down();
	void	OffSet();
	void	IsHit();
	void	Climb();


private:
	void	Get_Damaged(int _iDamage);

	void	Use_Mana() { --m_tStat.iMana; m_iHitCount = 0; }
	void	Get_Mana();

	void	Revive();

private:
	CObj*		m_pColi;
	bool		m_bParringJump;

	bool		m_bStageClear;

	STATE		m_ePreState;
	STATE		m_eCurState;
	bool		m_bSceneStart;

	DWORD		m_dwHealTime;
	DWORD		m_dwHitTime;
	bool		m_bHitTime;
	DWORD		m_dwInvicTime;
	bool		m_bInvicible;
	bool		m_bDashInvic;

	bool		m_bJump;
	bool		m_bAttack;
	
	float		m_fDash;
	bool		m_bDash;
	bool		m_bHeal;
	bool		m_bEnergyPo;
	bool		m_bDoubleJump;

	vector<CObj*> m_vecHp;

	int			m_iHitCount;

	bool		m_bParring;
	
	bool		m_bCameraShaking;
	DWORD		m_dwCameraTime;

	bool		m_bClimb;
	bool		m_bLClimb;
	bool		m_bRClimb;

};

