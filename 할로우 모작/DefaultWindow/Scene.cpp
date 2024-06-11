#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
	: m_bPause(false)
{
	m_dwTime = GetTickCount();
}


CScene::~CScene()
{
}
