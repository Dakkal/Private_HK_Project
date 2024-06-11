#pragma once
#include "Scene.h"
class CScene_Clear :
	public CScene
{
public:
	CScene_Clear();
	virtual ~CScene_Clear();

	// Inherited via CScene
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	HWND	m_hVideo;

};

