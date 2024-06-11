#pragma once
#include "Scene.h"
class CScene_Stage2 :
	public CScene
{
public:
	CScene_Stage2();
	virtual ~CScene_Stage2();

	// Inherited via CScene
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};

