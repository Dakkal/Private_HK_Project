#include "stdafx.h"
#include "Boss.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "Pattern1_Coli.h"
#include "Pattern2_Coli.h"
#include "CollisionMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "Effect_Boss.h"
#include "SoundMgr.h"

CBoss::CBoss()
	: m_bEvadeJump(false), m_bCloseJump(false), m_bIsPattern(false), m_bIsAction(false), m_bIsGrogy(false), m_bDisEnd(false)
	, m_bPattern1(false), m_bPattern2(false)
{
	m_dwIsActionTime = GetTickCount();
	m_dwIsPatternTime = GetTickCount();
	m_dwPattern2Time = GetTickCount();
	m_dwPattern2ColiTime = GetTickCount();
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize(void)
{
	m_eObjType = OBJECT_TYPE::OBJ_BOSS;

	m_tStat = { 50, 0, 50, 0, 2 };
	m_iArmorHP = 30;

	m_tInfo = { 950.f, 2700.f, 150.f , 310.f };

	m_tBmp.x = 650;
	m_tBmp.y = 390;

	m_fSpeed = 5.f;
	m_fPower = 18.f;
	
	Set_Target(CObjMgr::Get_Inst()->Get_Obj(OBJECT_TYPE::OBJ_PLAYER).front());
	m_fPlayerDis = fabs(Get_Target()->Get_Pos().fX - m_tInfo.fX);

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Image/Boss/FalseKnight.bmp", L"FalseKnight");
	m_pFrameKey = L"FalseKnight";

	m_eCurState = BOSS_STATE::IDLE;
	m_eLRDir = DIR_TYPE::DIR_LEFT;
	m_eCurDir = m_eLRDir;
	m_tFrame.iFrameStart = 0;
	m_iCurFrame = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 1;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.bStopMotion = false;
}

int CBoss::Update(void)
{
	Tile_Collision();

	
	
	if(!m_bIsAction && !m_bIsPattern)
		Dead();

	if (!m_bDead)
	{
		Target_Player();

		Down();
		EvadeJump();
		CloseJump();
		//EndJump();

		Pattern1();
		Pattern2();

		if (!m_bIsAction && !m_bIsPattern)
			Grogy();

		m_bHit = false;
	}
	

	__super::Update_Rect();

	return 0;
}

void CBoss::Late_Update()
{
	Set_PlayerDis();


	if (!m_bIsGrogy && !m_bDead)
	{
		if (!m_bIsPattern && m_dwIsActionTime + 2500 < GetTickCount())
		{
			Action();
			m_dwIsActionTime = GetTickCount();
		}

		if (!m_bIsAction && m_dwIsPatternTime + 3200 < GetTickCount())
		{
			Pattern();
			m_dwIsPatternTime = GetTickCount();
		}
	}



	__super::Move_Frame();
	Set_Motion();

	Tile_Collision();
}

void CBoss::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Inst()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Inst()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Inst()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		int(m_tBmpRect.left + fScrollX),
		int(m_tBmpRect.top + fScrollY),
		(int)m_tBmp.x,
		(int)m_tBmp.y,
		hMemDC,
		m_tFrame.iFrameStart * (int)m_tBmp.x,
		m_tFrame.iMotion * (int)m_tBmp.y,
		(int)m_tBmp.x,
		(int)m_tBmp.y,
		RGB(100, 100, 100));

}

void CBoss::Release(void)
{
}

void CBoss::Collider(CObj * _pObj, COL_DIR eDir, float fPush)
{
	OBJECT_TYPE eType = _pObj->Get_ObjType();
	CObj* pColi = _pObj;

	switch (eDir)
	{
	case COL_DIR::LEFT:
		Set_PosX(-fPush);
		break;
	case COL_DIR::TOP:
		Set_PosY(-fPush);
		m_bGround = true;
		break;
	case COL_DIR::RIGHT:
		Set_PosX(fPush);
		break;
	case COL_DIR::BOTTOM:
		Set_PosY(fPush);
		break;
	}
}

void CBoss::Tile_Collision()
{
	CObj* pTile = CObjMgr::Get_Inst()->Get_Target(OBJECT_TYPE::OBJ_TILE, this);

	CCollisionMgr::TestTile(this, pTile);
}

void CBoss::Set_Motion()
{
	if (m_ePreDir != m_eCurDir || m_ePreState != m_eCurState)
	{
		if (m_eCurDir == DIR_TYPE::DIR_RIGHT)
		{
			switch (m_eCurState)
			{
			case BOSS_STATE::IDLE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				break;

			case BOSS_STATE::JUMP:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 2;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_JUMP);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Jump.wav", BOSSSOUND_JUMP, 1.0f);
				break;

			case BOSS_STATE::DOWN:
				m_tFrame.iFrameStart = 1;
				m_iCurFrame = 1;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 2;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::PATTERN1:
			{
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 4;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;

			}
				break;

			case BOSS_STATE::PATTERN2:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 10;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Rage.wav", BOSSSOUND_EFFECT, 0.9f);
				break;

			case BOSS_STATE::GROGY:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 7;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 7;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::HIT:
				m_tFrame.iFrameStart = 7;
				m_iCurFrame = 7;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 7;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::GROGY_RECOVER:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 9;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 7;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 7;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;
			}

			m_ePreState = m_eCurState;
			m_ePreDir = m_eCurDir;
		}
	
		else if (m_eCurDir == DIR_TYPE::DIR_LEFT)
		{
			switch (m_eCurState)
			{
			case BOSS_STATE::IDLE:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 1;
				m_tFrame.dwSpeed = 150;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				break;

			case BOSS_STATE::JUMP:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 3;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_JUMP);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Jump.wav", BOSSSOUND_JUMP, 1.0f);
				break;

			case BOSS_STATE::DOWN:
				m_tFrame.iFrameStart = 1;
				m_iCurFrame = 1;
				m_tFrame.iFrameEnd = 1;
				m_tFrame.iMotion = 3;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::PATTERN1:
			{
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 5;
				m_tFrame.iMotion = 5;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;

				
					
			}
				break;

			case BOSS_STATE::PATTERN2:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 10;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = false;
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Rage.wav", BOSSSOUND_EFFECT,0.9f);
				break;

			case BOSS_STATE::GROGY:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 7;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 6;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::HIT:
				m_tFrame.iFrameStart = 7;
				m_iCurFrame = 7;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 6;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::GROGY_RECOVER:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 0;
				m_tFrame.iFrameEnd = 3;
				m_tFrame.iMotion = 8;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;

			case BOSS_STATE::DEAD:
				m_tFrame.iFrameStart = 0;
				m_iCurFrame = 7;
				m_tFrame.iFrameEnd = 9;
				m_tFrame.iMotion = 6;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.bStopMotion = true;
				break;
			}

			m_ePreState = m_eCurState;
			m_ePreDir = m_eCurDir;
		}
	}
}

void CBoss::OnCollision(CObj * _pObj, RECT _rc)
{
	switch (_pObj->Get_ObjType())
	{
	case OBJECT_TYPE::OBJ_COLLI:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		break;
	case OBJECT_TYPE::OBJ_PROJECTILE:
		Get_Damaged(_pObj->Get_Stat().iDamage);
		break;
	case OBJECT_TYPE::OBJ_OBS:
		m_bDisEnd = true;
		break;
	default:
		break;
	}
}

void CBoss::Target_Player()
{
	if (!m_bIsAction && !m_bIsPattern && !m_bIsGrogy)
	{
		if (Get_Target()->Get_Pos().fX < m_tInfo.fX)
		{
			m_eLRDir = DIR_TYPE::DIR_LEFT;
			m_eCurDir = m_eLRDir;
		}

		else if (Get_Target()->Get_Pos().fX > m_tInfo.fX)
		{
			m_eLRDir = DIR_TYPE::DIR_RIGHT;
			m_eCurDir = m_eLRDir;
		}
	}
}

void CBoss::Down()
{
	// ³«ÇÏ
	if (!m_bGround && !m_bEvadeJump && !m_bCloseJump)
	{
		m_fDownSpeed = (-9.8f * m_fTime * m_fTime * 0.5f);
		m_tInfo.fY -= m_fDownSpeed;
		m_fTime += 0.1f;
		m_bDown = true;
		m_eCurState = BOSS_STATE::DOWN;
	}
	// ³«ÇÏÁ¾·á
	if (m_bGround && !m_bEvadeJump && !m_bCloseJump && !m_bIsGrogy)
	{
		m_fTime = 0.f;
		m_bDown = false;
		m_eCurState = BOSS_STATE::IDLE;
	}
}

void CBoss::Set_PlayerDis()
{
	m_fPlayerDis = fabs(Get_Target()->Get_Pos().fX - m_tInfo.fX);
}

void CBoss::Action()
{
	int iRandom = rand() % 3;

	if (!m_bIsPattern)
	{
		if (m_bDisEnd)
		{
			m_bDisEnd = false;
			m_bCloseJump = true;
			m_bIsAction = true;
		}
		else if (m_fPlayerDis <= 200.f)
		{
			if(iRandom == 0)
				m_bCloseJump = true;
			else
				m_bEvadeJump = true;
			m_bIsAction = true;
		}

		else if (m_fPlayerDis >= 500.f)
		{
			m_bCloseJump = true;
			m_bIsAction = true;
		}
	}
}

void CBoss::Pattern()
{
	if (!m_bIsAction && !m_bIsPattern)
	{
		int iRandom = rand() % 4;

		if (iRandom == 3)
		{
			m_bPattern2 = true;
			m_bIsPattern = true;
			m_dwPattern2Time = GetTickCount();
			m_tInfo.fCX = 300.f;
		}
		else
		{
			m_bPattern1 = true;
			m_bIsPattern = true;
			int iRandom = rand() % 2;
			if (iRandom == 0)
			{
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Attack_01.wav", BOSSSOUND_EFFECT, 1.0f);
			}

			else if (iRandom == 1)
			{
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_EFFECT);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Attack_02.wav", BOSSSOUND_EFFECT, 1.0f);
			}
		}
	
	}
}

void CBoss::Pattern1()
{
	if (m_bPattern1)
	{
		m_eCurState = BOSS_STATE::PATTERN1;

		if (m_dwTime + 300 <= GetTickCount())
		{
			if (m_eLRDir == DIR_TYPE::DIR_RIGHT && m_tFrame.iFrameStart == 4)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYCOLLI, CAbstractFactory<CPattern1_Coli>::Create(m_tInfo.fX + 200.f, m_tInfo.fY + 90.f));
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_ATTACK);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Strike.wav", BOSSSOUND_ATTACK, 1.0f);
			}
				
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT && m_tFrame.iFrameStart == 4)
			{
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYCOLLI, CAbstractFactory<CPattern1_Coli>::Create(m_tInfo.fX - 200.f, m_tInfo.fY + 90.f));
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_ATTACK);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Strike.wav", BOSSSOUND_ATTACK, 1.0f);
			}
				

			m_dwTime = GetTickCount();
		}

		if (m_eCurState == BOSS_STATE::PATTERN1 && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bPattern1 = false;
			m_bIsPattern = false;
			m_eCurState = BOSS_STATE::IDLE;
			
		}
	}
}

void CBoss::Pattern2()
{
	if (m_bPattern2)
	{

		float fRandom1 = ((rand() % 5) + 1) * 100;
		float fRandom2 = ((rand() % 5) + 1) * 100;
		float fRandom3 = ((rand() % 9) + 1) * 50;
		float fRandom4 = ((rand() % 9) + 1) * 50;
		float fRandom5 = ((rand() % 3) + 1) * 150;
		float fRandom6 = ((rand() % 3) + 1) * 150;
		m_eCurState = BOSS_STATE::PATTERN2;

		if (m_dwPattern2ColiTime + 1800 <= GetTickCount())
		{
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYPROJECTILE, CAbstractFactory<CPattern2_Coli>::Create(m_tInfo.fX - fRandom1, m_tInfo.fY - 600.f));
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYPROJECTILE, CAbstractFactory<CPattern2_Coli>::Create(m_tInfo.fX + fRandom2, m_tInfo.fY - 900.f));
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYPROJECTILE, CAbstractFactory<CPattern2_Coli>::Create(m_tInfo.fX - fRandom3, m_tInfo.fY - 700.f));
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYPROJECTILE, CAbstractFactory<CPattern2_Coli>::Create(m_tInfo.fX + fRandom4, m_tInfo.fY - 800.f));
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYPROJECTILE, CAbstractFactory<CPattern2_Coli>::Create(m_tInfo.fX - fRandom5, m_tInfo.fY - 750.f));
			CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_ENEMYPROJECTILE, CAbstractFactory<CPattern2_Coli>::Create(m_tInfo.fX + fRandom6, m_tInfo.fY - 650.f));
			m_dwPattern2ColiTime = GetTickCount();
		}
		

		if (m_dwSoundTime + 560 <= GetTickCount())
		{
			CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_ATTACK);
			CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Strike.wav", BOSSSOUND_ATTACK, 1.0f);
			m_dwSoundTime = GetTickCount();
		}

		if (m_dwPattern2Time + 6000 <= GetTickCount() && m_tFrame.iFrameStart == 4 || 
			m_dwPattern2Time + 6000 <= GetTickCount() && m_tFrame.iFrameStart == 8)
		{
			m_bPattern2 = false;
			m_bIsPattern = false;
			m_tInfo.fCX = 150.f;
			m_eCurState = BOSS_STATE::IDLE;
			m_dwPattern2Time = GetTickCount();
		}
	}
}

void CBoss::Grogy()
{
	if (m_bDead)
		return;

	if (m_eCurState == BOSS_STATE::GROGY_RECOVER && m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
	{
		m_bIsGrogy = false;
		m_eCurState = BOSS_STATE::IDLE;
	}

	if (!m_bIsAction && !m_bIsPattern && !m_bIsGrogy && m_iArmorHP <= 0 && m_bHit)
	{
		m_eCurState = BOSS_STATE::GROGY;
		m_bIsGrogy = true;
		m_bHit = false;
		m_dwGrogyTime = GetTickCount();
		m_tStat.iDamage = 0;
		CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_GROGY);
		CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Grogy.wav", BOSSSOUND_GROGY, 1.0f);
	}

	if (m_bIsGrogy)
	{
		if (m_eCurState == BOSS_STATE::GROGY && m_tFrame.iFrameStart < 7)
		{
			if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
				m_tInfo.fX -= 3.f;
			else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
				m_tInfo.fX += 3.f;
		}

		if (m_dwGrogyTime + 5000 < GetTickCount())
		{
			m_eCurState = BOSS_STATE::GROGY_RECOVER;
			m_dwGrogyTime = GetTickCount();
			m_dwIsActionTime = GetTickCount();
			m_dwIsPatternTime = GetTickCount();
			m_iArmorHP = 30;
			m_tStat.iDamage = 2;
		}
	}
	
}

void CBoss::Get_Damaged(int _iDamage)
{
	if (m_bIsGrogy && m_eCurState == BOSS_STATE::GROGY && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_tStat.iHp -= _iDamage;
		if (m_eCurState != BOSS_STATE::GROGY_RECOVER)
		{
			m_eCurState = BOSS_STATE::HIT;
			m_tFrame.iFrameStart = 7;
			CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Health_Damage.wav", BOSSSOUND_EFFECT, 1.0f);
		}
	}

	else if (m_bIsGrogy &&  m_eCurState == BOSS_STATE::HIT)
	{
		m_tStat.iHp -= _iDamage;
		m_tFrame.iFrameStart = 7;
		CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Health_Damage.wav", BOSSSOUND_EFFECT, 1.0f);
	}

	else
	{
		m_iArmorHP -= _iDamage;
		CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_EFFECT);
		CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Damaged.wav", BOSSSOUND_EFFECT, 1.0f);
	}

	m_bHit = true;
}

void CBoss::Dead()
{
	if (!m_bIsAction && !m_bIsPattern && !m_bIsGrogy && m_tStat.iHp <= 0 && m_bHit)
	{
		m_eCurState = BOSS_STATE::GROGY;
		m_bDead = true;
		m_bHit = false;
		m_tStat.iDamage = 0;
		CSoundMgr::Get_Inst()->StopAll();
		CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Death.wav", BOSSSOUND_GROGY, 1.0f);
	}

	if (m_bDead && m_tFrame.iFrameStart < 7)
	{
		if (m_eLRDir == DIR_TYPE::DIR_RIGHT)
			m_tInfo.fX -= 3.f;
		else if (m_eLRDir == DIR_TYPE::DIR_LEFT)
			m_tInfo.fX += 3.f;
	}
}

void CBoss::EvadeJump()
{
	if (m_bEvadeJump)
	{
		m_eCurState = BOSS_STATE::JUMP;

		if (m_eCurDir == DIR_TYPE::DIR_LEFT)
		{
			float JumpHeight = m_fPower * m_fTime;
			float Gravity = 9.8f * m_fTime * m_fTime * 0.5f;

			if (Gravity < -25.f)
				Gravity = -25.f;


			m_tInfo.fX += 8.f;
			m_tInfo.fY -= JumpHeight - Gravity;
			m_fTime += 0.15f;

			if (JumpHeight < Gravity)
			{
				m_fTime -= 0.12f;
			}

			if (JumpHeight < Gravity && m_bGround)
			{
				m_fTime = 0.f;
				m_bEvadeJump = false;
				m_bIsAction = false;
				m_eCurState = BOSS_STATE::IDLE;
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Boss>::Create(m_tInfo.fX, m_tInfo.fY + 100.f));
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_JUMP);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Land.wav", BOSSSOUND_JUMP, 1.0f);
			}
		}

		else if (m_eCurDir == DIR_TYPE::DIR_RIGHT)
		{
			float JumpHeight = m_fPower * m_fTime;
			float Gravity = 9.8f * m_fTime * m_fTime * 0.5f;

			if (Gravity < -25.f)
				Gravity = -25.f;

			m_tInfo.fX -= 8.f;
			m_tInfo.fY -= JumpHeight - Gravity;
			m_fTime += 0.15f;

			if (JumpHeight < Gravity)
			{
				m_fTime -= 0.12f;
			}

			if (JumpHeight < Gravity && m_bGround)
			{
				m_fTime = 0.f;
				m_bEvadeJump = false;
				m_bIsAction = false;
				m_eCurState = BOSS_STATE::IDLE;
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Boss>::Create(m_tInfo.fX, m_tInfo.fY + 100.f));
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_JUMP);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Land.wav", BOSSSOUND_JUMP, 1.0f);
			}
		}
	}
	
}

void CBoss::CloseJump()
{
	if (m_bCloseJump)
	{
		m_eCurState = BOSS_STATE::JUMP;

		if (m_eCurDir == DIR_TYPE::DIR_LEFT)
		{
			float JumpHeight = m_fPower * m_fTime;
			float Gravity = 9.8f * m_fTime * m_fTime * 0.5f;

			if (Gravity < -25.f)
				Gravity = -25.f;

			m_tInfo.fX -= 8.f;
			m_tInfo.fY -= JumpHeight - Gravity;
			m_fTime += 0.15f;

			if (JumpHeight < Gravity)
			{
				m_fTime -= 0.12f;
			}

			if (JumpHeight < Gravity && m_bGround)
			{
				m_fTime = 0.f;
				m_bCloseJump = false;
				m_bIsAction = false;
				m_eCurState = BOSS_STATE::IDLE;
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Boss>::Create(m_tInfo.fX, m_tInfo.fY + 100.f));
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_JUMP);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Land.wav", BOSSSOUND_JUMP, 1.0f);
			}
		}

		else if (m_eCurDir == DIR_TYPE::DIR_RIGHT)
		{
			float JumpHeight = m_fPower * m_fTime;
			float Gravity = 9.8f * m_fTime * m_fTime * 0.5f;

			if (Gravity < -25.f)
				Gravity = -25.f;

			m_tInfo.fX += 8.f;
			m_tInfo.fY -= JumpHeight - Gravity;
			m_fTime += 0.15f;

			if (JumpHeight < Gravity)
			{
				m_fTime -= 0.12f;
			}

			if (JumpHeight < Gravity && m_bGround)
			{
				m_fTime = 0.f;
				m_bCloseJump = false;
				m_bIsAction = false;
				m_eCurState = BOSS_STATE::IDLE;
				CObjMgr::Get_Inst()->AddObj(OBJECT_TYPE::OBJ_EFFECT, CAbstractFactory<CEffect_Boss>::Create(m_tInfo.fX, m_tInfo.fY + 100.f));
				CSoundMgr::Get_Inst()->StopSound(BOSSSOUND_JUMP);
				CSoundMgr::Get_Inst()->PlaySound(L"FKnight_Land.wav", BOSSSOUND_JUMP, 1.0f);
			}
		}
	}
}
