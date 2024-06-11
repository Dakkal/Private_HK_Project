#pragma once
#include "Obj.h"
class CMouse :
	public CObj
{
public:
	CMouse();
	virtual ~CMouse();

	// Inherited via CObj
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};

