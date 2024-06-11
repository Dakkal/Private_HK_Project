#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void	Initialize(void)	PURE;
	virtual int		Update(void)		PURE;
	virtual void	Late_Update()		PURE;
	virtual void	Render(HDC hDC)		PURE;
	virtual void	Release(void)		PURE;

	virtual void	Collider(CObj* _pObj, COL_DIR eDir, float fPush)	PURE;
	virtual void	Tile_Collision()									PURE;
	virtual void	Set_Motion()										PURE;
	virtual void	OnCollision(CObj* _pObj, RECT _rc)					PURE;

public:
	void	Set_Pos(float _X, float _Y)	{ m_tInfo.fX = _X; m_tInfo.fY = _Y; }
	void	Set_PosX(float _X)			{ m_tInfo.fX += _X; }
	void	Set_PosY(float _Y)			{ m_tInfo.fY += _Y; }

	void    Set_Dead()					{ m_bDead = true; }

	void	Set_Speed(float _fSpeed)	{ m_fSpeed = _fSpeed; }
	void	Set_Angle(float _fAngle)	{ m_fAngle = _fAngle; }
	void	Set_Dis(float _fDis)		{ m_fSpeed = _fDis; }
	void	Set_Power(float _fPower)	{ m_fSpeed = _fPower; }

	void	Set_Target(CObj* _pTarget)	{ m_pTarget = _pTarget; }

	void	Set_FrameKey(TCHAR* pFrameKey)	{ m_pFrameKey = pFrameKey; }
	void	Set_LRDir(DIR_TYPE _eDir)		{ m_eLRDir = _eDir; }
	void	Set_UDDir(DIR_TYPE _eDir)		{ m_eUDDir = _eDir; }

	void	Set_OnCollision()		{ m_bCollision = true; }
	void	Set_OffCollision()		{ m_bCollision = false; }
	void	Set_OnGround()			{ m_bGround = true; }
	void	Set_OffGround()			{ m_bGround = false; }

	void    Set_Hit() { m_bHit = true; }

	void	Set_Channel(CHANNEL_TYPE _eChnnel) { m_eChannel = _eChnnel; }


	void		Update_Rect(void);


public:
	INFO		Get_Pos()			{ return m_tInfo; }
	RECT		Get_Rect()			{ return m_tRect; }

	STAT		Get_Stat()			{ return m_tStat; }

	BOOL		Get_Dead()			{ return m_bDead; }

	FLOAT		Get_Speed()			{ return m_fSpeed; }
	FLOAT		Get_Angle()			{ return m_fAngle; }
	FLOAT		Get_Dis()			{ return m_fDistance; }
	FLOAT		Get_Power()			{ return m_fPower; }

	CObj*		Get_Target()		{ return m_pTarget; }
	RENDER_TYPE	Get_RenderType()	{ return m_eRender; }
	DIR_TYPE	Get_LRDirType()		{ return m_eLRDir; }
	DIR_TYPE	Get_UDDirType()		{ return m_eUDDir; }
	FRAME		Get_Frame()			{ return m_tFrame; }

	OBJECT_TYPE Get_ObjType()		{ return m_eObjType; }

	BOOL		Get_Ground()		{ return m_bGround; }

	BOOL		Get_Down()			{ return m_bDown; }
	FLOAT		Get_DownSpeed()		{ return m_fDownSpeed; }
	MONSTER_TYPE Get_MonType()		{ return m_eMonType; }
protected:

protected:
	
	void		Update_BmpRect(void);
	void		Move_Frame(void);

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	RECT		m_tBmpRect;
	POINT		m_tBmp;

	DWORD		m_dwTime;
	bool		m_bDead;
	bool		m_bHit;

	float		m_fSpeed;
	float		m_fAngle;
	float		m_fDistance;
	float		m_fPower;
	float		m_fTime;
	
	OBJECT_TYPE m_eObjType;
	CObj*       m_pTarget;

	STAT		m_tStat;


	TCHAR*		m_pFrameKey;
	FRAME		m_tFrame;
	int			m_iCurFrame;
	RENDER_TYPE	m_eRender;
	DIR_TYPE	m_eLRDir;
	DIR_TYPE	m_eUDDir;
	
	DIR_TYPE	m_eColDir;
	bool		m_bGround;
	bool		m_bCollision;

	bool		m_bDown;
	float		m_fDownSpeed;

	MONSTER_TYPE m_eMonType;

	CHANNEL_TYPE m_eChannel;
	DWORD		 m_dwSoundTime;
};

