#pragma once
#include "Scene.h"
class CScene_Edit :
	public CScene
{
public:
	CScene_Edit();
	virtual ~CScene_Edit();

	// Inherited via CScene
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	MONSTER_TYPE	m_eMonType;

};

