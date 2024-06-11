#pragma once

#include "Obj.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj*	Create()
	{
		T*	pType = new T;
		pType->Initialize();

		return pType;
	}

	static CObj*	Create(float _X, float _Y)
	{
		T*	pType = new T;
		pType->Initialize();
		pType->Set_Pos(_X, _Y);
		

		return pType;
	}


	static CObj*	Create(float _X, float _Y, float _Speed)
	{
		T*	pType = new T;
		pType->Initialize();
		pType->Set_Pos(_X, _Y);
		pType->Set_Speed(_Speed);
		
		return pType;
	}

	static CObj*	Create(float _X, float _Y, DIR_TYPE _eDir)
	{
		T*	pType = new T;
		pType->Set_LRDir(_eDir);
		pType->Initialize();
		pType->Set_Pos(_X, _Y);
		

		return pType;
	}

	static CObj*	Create(float _X, float _Y, EFFECT_PLAYER _Effect)
	{
		T*	pType = new T;
		pType->Set_Effect(_Effect);
		pType->Initialize();
		pType->Set_Pos(_X, _Y);
		

		return pType;
	}
};

