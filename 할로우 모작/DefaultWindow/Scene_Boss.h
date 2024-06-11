#pragma once
#include "Scene.h"
class CScene_Boss :
	public CScene
{
public:
	CScene_Boss();
	virtual ~CScene_Boss();

	// Inherited via CScene
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};

