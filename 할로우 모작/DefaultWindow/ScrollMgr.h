#pragma once
class CScrollMgr
{
public:
	CScrollMgr();
	~CScrollMgr();

public:
	FLOAT	Get_ScrollX() { return m_fScrollX; }
	FLOAT	Get_ScrollY() { return m_fScrollY; }

	void	Set_ScrollX(float _X) { m_fScrollX += _X; }
	void	Set_ScrollY(float _Y) { m_fScrollY += _Y; }

	void	Set_PoinScroll(float _X, float _Y);

	void    Scroll_Lock();

public:
	static	CScrollMgr*	Get_Inst()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;

		return m_pInstance;
	}
	static	void Destory_Inst()
	{
		if (m_pInstance)
			Safe_Delete<CScrollMgr*>(m_pInstance);
	}

private:
	static	CScrollMgr*		m_pInstance;
	float	m_fScrollX;
	float   m_fScrollY;
};

