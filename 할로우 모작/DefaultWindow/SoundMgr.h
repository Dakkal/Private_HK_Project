#pragma once

#include "Define.h"

class CSoundMgr
{
public:
	static CSoundMgr* Get_Inst()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Inst()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 
	void Release(); 

public:
	void PlaySound(TCHAR* pSoundKey, CHANNEL_TYPE eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void PlayLoop(TCHAR* pSoundKey, CHANNEL_TYPE eID, float fVolume);
	void StopSound(CHANNEL_TYPE eID);
	void StopAll();
	void SetChannelVolume(CHANNEL_TYPE eID, float fVolume);

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 

	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 
};

