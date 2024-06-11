#pragma once

class CMainGame
{
public:
	void		Initialize(void);
	void		Update(void);
	void		Late_Update();
	void		Render();
	void		Release(void);

private:
	HDC			m_DC;
	DWORD		m_dwTime;

public:
	CMainGame();
	~CMainGame();
};

