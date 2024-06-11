#pragma once
#include "Scene.h"
class CScene_Menu :
	public CScene
{
public:
	CScene_Menu();
	virtual ~CScene_Menu();

	// Inherited via CScene
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};

