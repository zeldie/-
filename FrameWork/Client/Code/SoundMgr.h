#pragma once
#include "fmod.hpp"
#include "common.h"

class CSoundMgr
{
public:
	// ä�� ���� �������. 
	enum CHANNELID {
		BGM, BLADE1, AI,
		MAX_CHANNEL
	};
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
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
	void MyPlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void MyPlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void AllStop();
	void SetVolumeDown(_float fVolume);
	void SetAiVolumeDown(_float fVolume);
public:
	void LoadSoundFile();
	void Load_HoSoundFile();
	void Load_SiwonSoundFile();
	void Load_AiSoundFile();
	void Load_UISoundFile();
private:
	static CSoundMgr* m_pInstance;
	//FMOD_SOUND ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD::Sound*> m_mapSound;
	vector<FMOD::Sound*> m_vecSound;
	// FMOD_CHANNEL ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD::Channel* m_pChannelBGM[MAX_CHANNEL];
	FMOD::Channel* m_pChannelArr[1000];
	// ���� ä�� ��ü�� ��ġ ���� ��ü . 
	FMOD::System     *m_pSystem;

	FMOD_CHANNELGROUP* m_pChanelGroup;

	vector<FMOD::Sound*> m_vecHoSound;
	vector<FMOD::Sound*> m_vecSiwonSound;
	vector<FMOD::Sound*> m_vecAiSound; 
	vector<FMOD::Sound*> m_vecUISound;

public:
	void BGMSTART(_int iNum);
	int MaxChannel = 1000;

	void SoundOn(_int iNum, _float fVolume = 0.35f);
	void HoSoundOn(_int iNum, _float fVolume = 0.35f);
	void SiwonSoundOn(_int iNum, _float fVolume = 0.35f);
	void UISoundOn(_int iNum, _float fVolume = 0.35f);

	void Set_DefaultVolume(_float fVolume) { m_fDefalultVolume = fVolume; }
	void HoBGM(_int iNum);
	//void SoundOn(int iNum);
	//void HoSoundOn(_int iNum);
	void AiSoundOn(_int iNum, _float fVolume = 0.35f);
public:
	int i = 0;

	int bladec = 0;
	int bladehit = 0;
	int skc1 = 0;
	int skc2 = 0;
	int skc3 = 0;
	int fend = 0;

	_uint iChannel;
	_float m_fDefalultVolume;
};