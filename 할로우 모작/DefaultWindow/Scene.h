#pragma once
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void	Initialize(void)	PURE;
	virtual int		Update(void)		PURE;
	virtual void	Late_Update(void)	PURE;
	virtual void	Render(HDC hDC)		PURE;
	virtual void	Release(void)		PURE;
	
public:
	TCHAR*		Get_StageKey() { return m_pStageKey; }

protected:
	TCHAR*		m_pStageKey;

	DWORD		m_dwTime;
	bool		m_bPause;
};

