#pragma once
#include "Scene.h"
class CScene_Logo :
	public CScene
{
public:
	CScene_Logo();
	virtual ~CScene_Logo();

	// Inherited via CScene
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};

