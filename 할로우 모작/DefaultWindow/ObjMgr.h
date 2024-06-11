#pragma once

#include "Obj.h"

class CObjMgr
{
public:
	CObjMgr();
	~CObjMgr();

public:
	void		AddObj(OBJECT_TYPE _eType, CObj* _pObj);
	void		Update(void);
	void		Late_Update();
	void		Render(HDC hDc);
	void		Release(void);

public:
	list<CObj*>&	Get_Obj(OBJECT_TYPE _eType) { return m_ObjList[(int)_eType]; }
	vector<CObj*>&	Get_vecMon() { return m_vecMon; }
	CObj*			Get_Target(OBJECT_TYPE eType, CObj * pObj);

public:
	void		Delete_Objlist(OBJECT_TYPE _eObjType);
	void		AddMon(CObj* _pObj);
	void		Update_Mon();
	void		Late_Update_Mon();
	void		Render_Mon(HDC hDc);
	void		Delete_vecMon();
	void		Save_Monster(void);
	void		Load_Monster(void);
	void		Load_EditMonster();

	void		Set_Channel();

private:
	list<CObj*>		m_ObjList[(int)OBJECT_TYPE::END];
	vector<CObj*>	m_vecMon;

public:
	static CObjMgr*	Get_Inst()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

		return m_pInstance;
	}
	static void		Destroy_Inst()
	{
		if (m_pInstance)
		{
			Safe_Delete<CObjMgr*>(m_pInstance);
		}
	}

private:
	static		CObjMgr*	m_pInstance;
};

