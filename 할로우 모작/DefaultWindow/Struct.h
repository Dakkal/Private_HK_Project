#pragma once

typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;

}INFO;

typedef struct tagStat
{
	int		iMaxHp;
	int		iMaxMana;
	
	int		iHp;
	int		iMana;
	int		iDamage;

}STAT;

typedef struct tagFrame
{
	int			iFrameStart;
	int			iFrameEnd;
	int			iMotion;
	DWORD		dwSpeed;
	DWORD		dwTime;
	bool		bStopMotion;

}FRAME;

