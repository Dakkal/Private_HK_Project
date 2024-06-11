#pragma once
class CKeyMgr
{
public:
	CKeyMgr();
	~CKeyMgr();

public:
	bool	Key_Pressing(int _iKey);
	bool	Key_Down(int _iKey);
	bool	Key_Up(int _iKey);

public:
	static CKeyMgr*		Get_Inst()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyMgr;

		return m_pInstance;
	}
	static void		Destory_Inst()
	{
		if (m_pInstance)
			Safe_Delete<CKeyMgr*>(m_pInstance);
	}

private:
	static	CKeyMgr*	m_pInstance;
	bool				m_bKeyState[VK_MAX];
};

