#pragma once
#include "Scene.h"
#include "Tile.h"

class CTile;

class CScene_Stage1 :
	public CScene
{
public:
	CScene_Stage1();
	virtual ~CScene_Stage1();

	// Inherited via CScene
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;



};

