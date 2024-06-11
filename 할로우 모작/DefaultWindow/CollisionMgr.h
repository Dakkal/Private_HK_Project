#pragma once

#include "Obj.h"
#include "Tile.h"

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static	void Collision_Rect(list<CObj*> Dst, list<CObj*> Src);

	static	void Collision_Sphere(list<CObj*> Dst, list<CObj*> Src);
	static	bool Sphere_Check(CObj* pDst, CObj* pSrc);

	static  void Collision_RectEx(list<CObj*> Dst, list<CObj*> Src);
	static	bool RectEx_Check(CObj* pDst, CObj* pSrc, float* _X, float* _Y);

	static	void TestTile(CObj* pDst, CObj* pSrc);

};

