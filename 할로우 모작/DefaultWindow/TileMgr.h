#pragma once

#include "Tile.h"

class CTileMgr
{
public:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize(void);
	void Update(void);
	void Late_Update(void);
	void Render(HDC hDC);
	void Release(void);

public:
	void	Picking_Tile(POINT _pt, int _iDrawID);
	void	Save_Tile(void);
	void	Load_EditTile();
	void	Load_Tile(void);

	vector<CObj*>&  Get_vecTile() { return m_vecTile; }

public:
	static CTileMgr*		Get_Inst(void)
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;

		return m_pInstance;
	}
	static void		Destroy_Inst(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}

	}
private:
	static CTileMgr*		m_pInstance;
	vector<CObj*>			m_vecTile;

};

