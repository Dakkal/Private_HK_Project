#include "stdafx.h"
#include "CollisionMgr.h"
#include "TileMgr.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*> Dst, list<CObj*> Src)
{
	RECT rc;
	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (IntersectRect(&rc, &Dest->Get_Rect(), &Sour->Get_Rect()))
			{
				Dest->OnCollision(Sour, rc);
				Sour->OnCollision(Dest, rc);
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*> Dst, list<CObj*> Src)
{
	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (Sphere_Check(Dest, Sour))
			{
				Dest->Set_Dead();
				Sour->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Sphere_Check(CObj * pDst, CObj * pSrc)
{
	float fWidth =	  fabs(pDst->Get_Pos().fX - pSrc->Get_Pos().fX);
	float fHeigth =	  fabs(pDst->Get_Pos().fY - pSrc->Get_Pos().fY);
	float fDiagnoal = sqrtf(pow(fWidth, 2) + pow(fHeigth, 2));
	
	float fRadious =  (pDst->Get_Pos().fX + pSrc->Get_Pos().fX) * 0.5f;

	return fDiagnoal <= fRadious;
}

void CCollisionMgr::Collision_RectEx(list<CObj*> Dst, list<CObj*> Src)
{
	float	fX = 0.f, fY = 0.f;

	for (auto& Dest : Dst)
	{
		for (auto& Sour : Src)
		{
			if (RectEx_Check(Dest, Sour, &fX, &fY))
			{
				// 상 하 충돌
				if (fX > fY)
				{
					if (Dest->Get_Pos().fY < Sour->Get_Pos().fY)
					{
						Dest->Set_PosY(-fY);
					}
					else
					{
						Dest->Set_PosY(fY);
					}
				}
				// 좌 우 충돌
				else
				{
					if (Dest->Get_Pos().fX < Sour->Get_Pos().fX)
					{
						Dest->Set_PosX(-fX);
					}
					else
					{
						Dest->Set_PosX(fX);
					}
				}
			}
		}
	}
}

bool CCollisionMgr::RectEx_Check(CObj * pDst, CObj * pSrc, float* _X, float* _Y)
{
	float	fWidth =  fabs(pDst->Get_Pos().fX - pSrc->Get_Pos().fX) + pDst->Get_Speed();
	float	fHeight = fabs(pDst->Get_Pos().fY - pSrc->Get_Pos().fY) - pDst->Get_Speed();

	float	fRadiusX = (pDst->Get_Pos().fCX + pSrc->Get_Pos().fCX) * 0.5f;
	float	fRadiusY = (pDst->Get_Pos().fCY + pSrc->Get_Pos().fCY) * 0.5f;

	if ((fRadiusX >= fWidth) && (fRadiusY >= fHeight))
	{
		*_X = fRadiusX - fWidth;
		*_Y = fRadiusY - fHeight;
		return true;
	}

	return false;
}

//void CCollisionMgr::Collision_RectTile(list<CObj*> Dst, list<CObj*> Src)
//{
//	float fX = 0.f, fY = 0.f;
//
//	for (auto& Dest : Dst)
//	{
//		for (auto& Sour : Src)
//		{
//			if (Dest == Sour && ((Dest->Get_ObjType() == OBJECT_TYPE::OBJ_TILE) == (Sour->Get_ObjType() == OBJECT_TYPE::OBJ_TILE)))
//				return;
//
//			if (Test(Dest, Sour, &fX, &fY))
//			{
//				if (fX > fY)
//				{
//					// 상하 충돌
//					if (Dest->Get_Pos().fY < Sour->Get_Pos().fY)
//					{
//						Dest->Collider(Sour, COL_DIR::BOTTOM, fY);
//						Sour->Collider(Dest, COL_DIR::TOP, fY);
//					}
//					else
//					{
//						Dest->Collider(Sour, COL_DIR::TOP, fY);
//						Sour->Collider(Dest, COL_DIR::BOTTOM, fY);
//					}
//				}
//				else
//				{
//					// 좌우 충돌
//					if (Dest->Get_Pos().fX < Sour->Get_Pos().fX)
//					{
//						Dest->Collider(Sour, COL_DIR::RIGHT, fX);
//						Sour->Collider(Dest, COL_DIR::LEFT, fX);
//					}
//					else if (Dest->Get_Pos().fX > Sour->Get_Pos().fX)
//					{
//						Dest->Collider(Sour, COL_DIR::LEFT, fX);
//						Sour->Collider(Dest, COL_DIR::RIGHT, fX);
//					}
//				}
//			}
//			else
//			{
//				Dest->Set_OffGround();
//				Sour->Set_OffGround();
//			}
//		}
//	}
//}


void CCollisionMgr::TestTile(CObj * pDst, CObj * pSrc)
{
	float fWidth = 0.f;
	float fHeight = 0.f;

	fWidth = fabs(pDst->Get_Pos().fX - pSrc->Get_Pos().fX);
	fHeight = fabs(pDst->Get_Pos().fY - pSrc->Get_Pos().fY);
	
	float fRadiusX = (pDst->Get_Pos().fCX + pSrc->Get_Pos().fCX) * 0.5f;
	float fRadiusY = (pDst->Get_Pos().fCY + pSrc->Get_Pos().fCY) * 0.5f;


	float fOverX = fRadiusX - fWidth;
	float fOverY = fRadiusY - fHeight;

	if ((fRadiusX >= fWidth) && (fRadiusY >= fHeight))
	{
			// 상하 충돌
			if (fOverX > fOverY)
			{
				if (pDst->Get_Pos().fY < pSrc->Get_Pos().fY)
				{
					pDst->Collider(pSrc, COL_DIR::TOP, fOverY);
				}
				else
				{
					pDst->Collider(pSrc, COL_DIR::BOTTOM, fOverY);
				}
			}
			// 좌우 충돌
			else
			{
				if (pDst->Get_Pos().fX < pSrc->Get_Pos().fX)
				{
					pDst->Collider(pSrc, COL_DIR::LEFT, fOverX);
				}
				else
				{
					pDst->Collider(pSrc, COL_DIR::RIGHT, fOverX);
				}
			}
	}
	else
		pDst->Set_OffGround();
		
}

	
