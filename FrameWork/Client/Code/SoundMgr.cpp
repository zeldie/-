#include "stdafx.h"
#include "SoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;

CSoundMgr::CSoundMgr()
	:m_pSystem(nullptr),
	iChannel(0),
	m_fDefalultVolume(0.f)
{
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;

	Common_Init(&extradriverdata);

	/*
	Create a System object and initialize
	*/
	result = FMOD::System_Create(&m_pSystem);
	ERRCHECK(result);

	result = m_pSystem->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	result = m_pSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);

	LoadSoundFile();
	Load_HoSoundFile();
	Load_AiSoundFile();
	Load_SiwonSoundFile();
}

void CSoundMgr::Release()
{
	for (auto& MyPair : m_mapSound)
	{
		delete[] MyPair.first;
		MyPair.second->release();
	}
	m_mapSound.clear();

	for (auto s : m_vecSound)
	{
		s->release();
	}

	m_vecSound.clear();

	for (auto s : m_vecHoSound)
	{
		s->release();
	}

	m_vecHoSound.clear();

	for (auto s : m_vecAiSound)
	{
		s->release();
	}
	m_vecAiSound.clear();

	FMOD_RESULT       result;
	result = m_pSystem->close();
	ERRCHECK(result);
	result = m_pSystem->release();
	ERRCHECK(result);

	Common_Close();
}

void CSoundMgr::MyPlaySound(TCHAR * pSoundKey, CHANNELID eID)
{
	auto& iter = find_if(m_mapSound.begin(), m_mapSound.end(), CMyStrCmp(pSoundKey));
	if (iter == m_mapSound.end())
		return;
	bool IsPlay;
	if (m_pChannelArr[eID]->isPlaying(&IsPlay))
	{
		m_pSystem->playSound(iter->second, 0, false, &m_pChannelArr[eID]);
	}
	m_pSystem->update();

}

void CSoundMgr::MyPlayBGM(TCHAR * pSoundKey)
{
	//auto& iter = find_if(m_mapSound.begin(), m_mapSound.end(), CMyStrCmp(pSoundKey));
	//if (iter == m_mapSound.end())
	//	return;

	//m_pSystem->playSound(iter->second, 0, false, &m_pChannelArr[BGM]);
	//m_pChannelArr[BGM]->setMode(FMOD_LOOP_NORMAL);
	//m_pSystem->update();
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	m_pChannelArr[eID]->stop();
}

void CSoundMgr::AllStop()
{
	/*for (int ix = 0; ix < MAX_CHANNEL; ++ix)
		m_pChannelArr[ix]->stop();*/

	m_pChannelBGM[BGM]->stop();
}

void CSoundMgr::SetVolumeDown(_float fVolume)
{
	m_pChannelBGM[BGM]->setVolume(fVolume);
}

void CSoundMgr::SetAiVolumeDown(_float fVolume)
{
	m_pChannelBGM[AI]->setVolume(fVolume);
}

void CSoundMgr::LoadSoundFile()
{
	FMOD::Sound* pSound = nullptr;

	//////////
	// BGM //
	/////////

	// 0 / Loding Check / Test
	FMOD_RESULT eRes;
	eRes = m_pSystem->createSound("../../Sound/LoadingEnd.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);	// Err Check - 파일을 찾지 못할 경우 콘솔창에 에러 뜸
	// 맵으로 만들면 편하겠지만 귀찮아서 걍 씀
	// 사운드 추가 시 위와 같이 주석으로 번호/사용처/이름 달아줄 것
	// 벡터로 만들었기 때문에 위 내용이 반드시 지켜져야 함. 안되면 뭐...

	// 1 / Player / Jump-Landing
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Jump_Landing.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes); //Player_Run

	// 2 / Player / Player_Run
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Run.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes); //

	// 3 / Player / Player_RunEnd
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_RunEnd.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 4 / Player / Player_Jump
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Jump.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 5 / Player / Player_Atkvoice_Long
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_ATK_Charge.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 6 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying00.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 7 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying01.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 7 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying02.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 8 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying03.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 9 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying04.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 11 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying05.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 12 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying06.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 13 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying07.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 14 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying08.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 15 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying09.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 16 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying10.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 17 / Player / Player_Dying
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dying11.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 18 / Player / Player_Enter voice
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Enter0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 19 / Player / Player_Enter voice
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Enter1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 20 / Player / Player_Enter voice
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Enter2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 21 / Player / Player_Enter voice
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Enter3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 22 / Player / Player_Enter voice
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Enter4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 23 / Player / Player_Enter voice
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Enter5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 24 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt00.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 25 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt01.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 26 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt02.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 27 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt03.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 28 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt04.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 29 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt05.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 30 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt06.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 31 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt07.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 32 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt08.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 33 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt09.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 34 / Player / Player_Hurt
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt10.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 35 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 36 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 37 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 38 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 39 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 40 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 41 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri6.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 42 / Player / Player_Hurt_Cri
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Hurt_Cri7.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// DualSword
	// 43 / DualSword / DS_Slash
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Slash00.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 44 / DualSword / DS_Slash
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Slash01.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 45 / DualSword / DS_Slash
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Slash02.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 46 / DualSword / DS_Slash
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Slash03.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 47 / DualSword / DS_Slash
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Slash04.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 48 / DualSword / DS_Slash
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Slash05.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 49 / DualSword / DS_Slash
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Slash06.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// TwohandSword
	// 50 / TwohandSword / TS_Slash
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Slash0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 51 / TwohandSword / TS_Slash
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Slash1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 52 / TwohandSword / TS_Slash
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Slash2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 53 / TwohandSword / TS_Slash
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Slash3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 54 / TwohandSword / TS_Rock
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Rock.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 55 / TwohandSword / TS_Rock
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Rock2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 56 / TwohandSword / TS_Rock
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Rock3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// Longbow
	// 57 / LongBow / LB_Shot
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Shot0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 58 / LongBow / LB_Shot
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Shot1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 59 / LongBow / LB_Shot
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Shot2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 60 / LongBow / LB_Shot
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Shot3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 61 / LongBow / LB_Shot
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Shot4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 62 / LongBow / LB_Shot
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Shot5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 63 / LongBow / LB_Draw
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Draw.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 64 / LongBow / LB_Siege
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Siege.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	//Orb
	// 65 / Orb / Orb_Shot
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Shot_01_A.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 66 / Orb / Orb_Shot
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Shot_01_B.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 67 / Orb / Orb_Shot
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Shot_01_C.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 68 / Orb / Orb_Shot
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Shot_01_D.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 69 / Orb / Orb_Shot
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Shot_01_E.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 70 / Orb / Orb_Shot
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Shot_02_A.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 71 / Orb / Orb_Shot
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Shot_02_B.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 72 / Orb / Orb_AirDash
	eRes = m_pSystem->createSound("../../Sound/Orb/CHR_Orb_Dash_01_A.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 73 / Orb / Orb_EX1
	eRes = m_pSystem->createSound("../../Sound/Orb/OB_EX1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 74 / Orb / OB_RSkill
	eRes = m_pSystem->createSound("../../Sound/Orb/OB_RSkill.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 75 / Orb / OB_ESkill
	eRes = m_pSystem->createSound("../../Sound/Orb/OB_ESkill.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 76 / Player / Player_CaptureFlag
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_CaptureFlag.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 77 / Player / Player_GetFlag
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_GetFlag.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 78 / Player / Player_Knockdown
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Knockdown.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 79 / Player / Player_Respawn
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Respawn.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 80 / Player / Player_RespawnVoice0
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_RespawnVoice0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 81 / Player / Player_RespawnVoice0
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_RespawnVoice1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 82 / Player / Player_RespawnVoice0
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_RespawnVoice2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 83 / Player / Player_Dash
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Dash.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 84 / Player / Player_Tired
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Tired0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 85 / Player / Player_Tired
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Tired1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 86 / LongBow / LB_Rage
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Rage0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 87 / LongBow / LB_Rage
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Rage1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 88 / Orb / OB_Rage
	eRes = m_pSystem->createSound("../../Sound/Orb/OB_Rage0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 89 / Orb / OB_Rage
	eRes = m_pSystem->createSound("../../Sound/Orb/OB_Rage1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 90 / TwohandSword / TS_Rage
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Rage0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 91 / TwohandSword / TS_Rage
	eRes = m_pSystem->createSound("../../Sound/TwohandSword/TS_Rage1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 92 / DualSword / DS_Rage
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Rage0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
	// 93 / DualSword / DS_Rage
	eRes = m_pSystem->createSound("../../Sound/DualSword/DS_Rage1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 94 / Loading BGM / Test
	eRes = m_pSystem->createSound("../../Sound/BGM/LoadingBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 95 / LobyBGM / Test
	eRes = m_pSystem->createSound("../../Sound/BGM/LobyBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);


	// 96 / ApostleBGM / Test
	eRes = m_pSystem->createSound("../../Sound/BGM/ApostleBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);


	// 97 / BelatosBGM / Test
	eRes = m_pSystem->createSound("../../Sound/BGM/BelatosBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 98 / CartelBGM / Test
	eRes = m_pSystem->createSound("../../Sound/BGM/CartelBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 99 / RunGameBGM / Test
	eRes = m_pSystem->createSound("../../Sound/BGM/RunGameBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 100 / CustomizingBGM / Test
	eRes = m_pSystem->createSound("../../Sound/BGM/CustomizingBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Belatos %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// 101 / Belatos Long_Attack_1 / Long_Attack_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 102 / Belatos Long_Attack_2 / Long_Attack_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 103 / Belatos Long_Attack_2 / Long_Attack_3
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 104 / Belatos Long_Attack_2 / Long_Attack_4
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Long_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 105 / Belatos Middle_Attack_1 / Middle_Attack_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 106 / Belatos Middle_Attack_2 / Middle_Attack_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 107 / Belatos Middle_Attack_3 / Middle_Attack_3
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 108 / Belatos Middle_Attack_4 / Middle_Attack_4
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 109 / Belatos Middle_Attack_5 / Middle_Attack_5
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Middle_05_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 110 / Belatos Short_Attack_1 / Short_Attack_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 111 / Belatos Short_Attack_2 / Short_Attack_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 112 / Belatos Short_Attack_3 / Short_Attack_3
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 113 / Belatos Short_Attack_4 / Short_Attack_4
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 114 / Belatos Short_Attack_5 / Short_Attack_5
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_05_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 115 / Belatos Short_Attack_6 / Short_Attack_6
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Attack_Short_06_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 116 / Belatos Dying_Dialogue_1 / Dying_Dialogue_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Dialogue_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 117 / Belatos Dying_Dialogue_2 / Dying_Dialogue_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Dialogue_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 118 / Belatos Dying_Middle_1 / Dying_Middle_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 119 / Belatos Dying_Shrot_1 / Dying_Shrot_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Short_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 120 / Belatos Dying_Shrot_2 / Dying_Shrot_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Dying_Short_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 121 / Belatos Hurt_Long_1 / Hurt_Long_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Long_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 122 / Belatos Hurt_Long_2 / Hurt_Long_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Long_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 123 / Belatos Hurt_Middle_1 / Hurt_Middle_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 124 / Belatos Hurt_Middle_2 / Hurt_Middle_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Middle_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 125 / Belatos Hurt_Short_1 / Hurt_Short_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 126 / Belatos Hurt_Short_2 / Hurt_Short_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 127 / Belatos Hurt_Short_3 / Hurt_Short_3
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 128 / Belatos Hurt_Short_4 / Hurt_Short_4
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Hurt_Short_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 129 / Belatos Idle_1 / Idle_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Idle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 130 / Belatos Idle_2 / Idle_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Idle_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 131 / Belatos Idle_3 / Idle_3
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Idle_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 132 / Belatos Player_Dead_1 / Player_Dead_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_PlayerDead_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 133 / Belatos Start_1 / Start_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_Start_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 134 / Belatos UltimateSkill_1 / UltimateSkill_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Voice_Belatos_SpearGuardian_UltimateSkill_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 135 / Belatos Start_EffectSound / Start_EffectSound
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Start_EffectSound.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 136 / Belatos Start_2 / Start_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_StartVoice_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 137 / Belatos Arrow / Arrow
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Arrow.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 138 / Belatos JumpAttack_Sword / JumpAttack_Sword
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_JumpAttack_SwordSound.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 139 / Belatos JumpAttack_ShockSound / JumpAttack_ShockSound
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_JumpAttack_ShockSound.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 140 / Belatos Shield_Up_1 / Shield_Up_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 141 / Belatos Shield_Up_2 / Shield_Up_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 142 / Belatos Shield_Up_3 / Shield_Up_3
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 143 / Belatos Shield_Up_4 / Shield_Up_4
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 144 / Belatos Shield_Up_5 / Shield_Up_5
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 145 / Belatos Shield_Up_6 / Shield_Up_6
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldUp_6.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 146 / Belatos ShieldRush_1 / ShieldRush_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldRush_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 147 / Belatos ShieldRush_2 / ShieldRush_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_ShieldRush_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound); 
	ERRCHECK(eRes);

	// 148 / Belatos Sweep / Sweep
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Sweep.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 149 / Belatos Thrust / Thrust
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Thrust.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 150 / Belatos Attack / Attack
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Attack.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 151 / Belatos Run_1 / Run_1
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Run_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 152 / Belatos Run_2 / Run_2
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Run_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 153 / Belatos Run_3 / Run_3
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Run_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 154 / Belatos Run_4 / Run_4
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Run_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);

	// 155 / Belatos Down / Down
	eRes = m_pSystem->createSound("../../Sound/Belatos/Belatos_Down.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSound.push_back(pSound);
	ERRCHECK(eRes);
}

void CSoundMgr::Load_HoSoundFile()
{
	FMOD::Sound* pSound = nullptr;

	FMOD_RESULT eRes;
	//eRes = m_pSystem->createSound("../../Sound/LoadingEnd.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);
	//ERRCHECK(eRes);	// Err Check - 파일을 찾지 못할 경우 콘솔창에 에러 뜸
	//				// 맵으로 만들면 편하겠지만 귀찮아서 걍 씀
	//				// 사운드 추가 시 위와 같이 주석으로 번호/사용처/이름 달아줄 것
	//				// 벡터로 만들었기 때문에 위 내용이 반드시 지켜져야 함. 안되면 뭐...

	// 0 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Suc0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 1 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Suc1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 2 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Suc2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 3 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Suc3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 4 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Suc4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 5 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Suc5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	// 6 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Suc6.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 7 / MiniGame / MiniGame_Fail
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Fail0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 8 / MiniGame / MiniGame_Fail
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_Fail1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 9 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_AISuc0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 10 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_AISuc1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 11 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_AISuc2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 12 / MiniGame / MiniGame_Suc
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_AISuc3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);


	// 13 / MiniGame / MiniGame_Fail
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_AIFail0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 14 / MiniGame / MiniGame_Fail
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_AIFail1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 15 / MiniGame / MiniGame_Fail
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MiniGame_AIFail2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);


	// 16 / Total / Win
	eRes = m_pSystem->createSound("../../Sound/Win.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 17 / PlayerSound / Player_Heartbeat
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Heartbeat0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 18 / PlayerSound / Player_Heartbeat
	eRes = m_pSystem->createSound("../../Sound/PlayerSound/Player_Heartbeat0.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 19 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer00.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 20 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer01.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 21 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer02.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 22 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer03.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 23 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer04.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 24 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer05.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 25 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer06.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 26 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer07.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 27 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer08.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 28 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer09.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 29 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer10.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 30 / Hit / HitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/HitPlayer11.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 31 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer00.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 32 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer01.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 33 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer02.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 34 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer03.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 35 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer04.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 36 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer05.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 37 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer06.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 38 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer07.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 39 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer08.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 40 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer09.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 41 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer10.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 42 / Hit / CriticalHitPlayer
	eRes = m_pSystem->createSound("../../Sound/Hit/CriticalHitPlayer11.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 43 / BGM / MatcingBGM
	eRes = m_pSystem->createSound("../../Sound/BGM/MatcingBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 44 / MiniGame / PlayStart
	eRes = m_pSystem->createSound("../../Sound/MiniGame/PlayStart.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 45 / MiniGame / MyTurn
	eRes = m_pSystem->createSound("../../Sound/MiniGame/MyTurn.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 46 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Killed0.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 47 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Killed1.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 48 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Kill0.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 49 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Kill1.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 50 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Kill2.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 51 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Kill3.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 52 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Kill4.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 53 / Cartel / Killed
	eRes = m_pSystem->createSound("../../Sound/Cartel/Kill5.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 54 / Cartel / Aliance_Killed0
	eRes = m_pSystem->createSound("../../Sound/Cartel/Aliance_Killed0.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
	// 55 / Cartel / Aliance_Killed0
	eRes = m_pSystem->createSound("../../Sound/Cartel/Aliance_Killed0.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 56 / Cartel / Aliance_Killed0
	eRes = m_pSystem->createSound("../../Sound/Cartel/Voice_Win.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);

	// 57 /  BGM / EndingBGM
	eRes = m_pSystem->createSound("../../Sound/BGM/EndingBGM.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecHoSound.push_back(pSound);
	ERRCHECK(eRes);
}

void CSoundMgr::Load_SiwonSoundFile()
{
	FMOD::Sound* pSound = nullptr;

	FMOD_RESULT eRes;
	//eRes = m_pSystem->createSound("../../Sound/LoadingEnd.ogg", FMOD_DEFAULT, 0, &pSound);
	//m_vecSound.push_back(pSound);
	//ERRCHECK(eRes);	// Err Check - 파일을 찾지 못할 경우 콘솔창에 에러 뜸
	//				// 맵으로 만들면 편하겠지만 귀찮아서 걍 씀
	//				// 사운드 추가 시 위와 같이 주석으로 번호/사용처/이름 달아줄 것
	//				// 벡터로 만들었기 때문에 위 내용이 반드시 지켜져야 함. 안되면 뭐...

	// 0 / 탐욕의사도 / 스킬1
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 1 / 탐욕의사도 / 스킬2
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Middle_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 2 / 탐욕의사도 / 스킬3
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Middle_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 3 / 탐욕의사도 / 스킬4
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Short_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 4 / 탐욕의사도 / 스킬5
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Short_02_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 5 / 탐욕의사도 / 스킬6
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Short_03_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 6 / 탐욕의사도 / 스킬7
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Short_04_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 7 / 탐욕의사도 / 스킬8
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Short_05_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 8 / 탐욕의사도 / 스킬9
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Attack_Short_06_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 9 / 탐욕의사도 / 죽는소리
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Dying_Long_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 10 / 탐욕의사도 / 다운
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Hurt_Long_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 11 / 탐욕의사도 / 그로기
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Hurt_Middle_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 12 / 탐욕의사도 / 컷신 웃는소리
	eRes = m_pSystem->createSound("../../Sound/Apostle/Voice_ApostleOfGreed_Laugh_Long_01_KR.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 13 / 런게임 / 아이템줍기
	eRes = m_pSystem->createSound("../../Sound/RunGame/PickUpItem.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);

	// 14 / 런게임 / 텔레포트
	eRes = m_pSystem->createSound("../../Sound/RunGame/Teleport.mp3", FMOD_DEFAULT, 0, &pSound);
	m_vecSiwonSound.push_back(pSound);
	ERRCHECK(eRes);



}

void CSoundMgr::Load_AiSoundFile()
{
	FMOD::Sound* pSound = nullptr;
	FMOD_RESULT eRes;

	// 0 / Alliance Att1 / Att1
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Attack_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 1 / Alliance Att2 / Att2
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Attack_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 2 / Alliance Att3 / Att3
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Attack_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 3 / Alliance Att4 / Att4
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Attack_Short_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 4 / Alliance Att5 / Att5
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Attack_Short_5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 5 / Alliance Dying1 / Dying1
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Dying_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 6 / Alliance Dying2 / Dying2
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Dying_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 7 / Alliance Dying3 / Dying3
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Dying_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 8 / Alliance Dying4 / Dying4
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Dying_Short_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 9 / Alliance Flag1 / Flag1
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Flag_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 10 / Alliance Flag2 / Flag2
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Flag_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 11 / Alliance Flag3 / Flag3
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Flag_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 12 / Alliance Flag4 / Flag4
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Flag_Short_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 13 / Alliance Flag5 / Flag5
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Flag_Short_5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 14 / Alliance Flag6 / Flag6
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Flag_Short_6.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 15 / Alliance Hurt1 / Hurt1
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Hurt_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 16 / Alliance Hurt2 / Hurt2
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Hurt_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 17 / Alliance Hurt3 / Hurt3
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Hurt_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 18 / Alliance Respawn1 / Respawn1
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Respawn_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 19 / Alliance Respawn2 / Respawn2
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Respawn_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 20 / Alliance Respawn3/ Respawn3
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Respawn_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 21 / Enemy1 Att1 / Att1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Attack_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 22 / Enemy1 Att2 / Att2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Attack_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 23 / Enemy1 Att3 / Att3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Attack_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 24 / Enemy1 Dying1 / Dying1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Dying_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 25 / Enemy1 Dying2 / Dying2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Dying_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 26 / Enemy1 Flag1 / Flag1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Flag_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 27 / Enemy1 Flag2 / Flag2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Flag_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 28 / Enemy1 Flag3 / Flag3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Flag_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 29 / Enemy1 Flag4 / Flag4
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Flag_Short_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 30 / Enemy1 Flag5 / Flag5
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Flag_Short_5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 31 / Enemy1 Flag6 / Flag6
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Flag_Short_6.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 32 / Enemy1 Hurt1 / Hurt1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Hurt_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 33 / Enemy1 Hurt2 / Hurt2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Hurt_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 34 / Enemy1 Hurt3 / Hurt3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Hurt_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 35 / Enemy1 Respawn1 / Respawn1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Respawn_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 36 / Enemy1 Respawn2 / Respawn2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Respawn_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 37 / Enemy1 Respawn3 / Respawn3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Respawn_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 38 / Enemy2 Att1 / Att1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Attack_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 39 / Enemy2 Att2 / Att2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Attack_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 40 / Enemy2 Att3 / Att3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Attack_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 41 / Enemy2 Dying1 / Dying1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Dying_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 42 / Enemy2 Dying2 / Dying2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Dying_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 43 / Enemy2 Flag1 / Flag1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 44 / Enemy2 Flag2 / Flag2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 45 / Enemy2 Flag3 / Flag3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 46 / Enemy2 Flag4 / Flag4
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 47 / Enemy2 Flag5 / Flag5
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_5.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 48 / Enemy2 Flag6 / Flag6
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_6.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 49 / Enemy2 Flag7 / Flag1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_7.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 50 / Enemy2 Flag8 / Flag8
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_8.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 51 / Enemy2 Flag9 / Flag9
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_9.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 52 / Enemy2 Flag10 / Flag10
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Flag_Short_10.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 53 / Enemy2 Hurt1 / Hurt1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Hurt_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 54 / Enemy2 Hurt2 / Hurt2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Hurt_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 55 / Enemy2 Hurt3 / Hurt3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Hurt_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 56 / Enemy2 Respawn1 / Respawn1
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Respawn_Short_1.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 57 / Enemy2 Respawn2 / Respawn2
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Respawn_Short_2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 58 / Enemy2 Respawn3 / Respawn3
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Respawn_Short_3.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 59 / Alliance Att6 / Att6
	eRes = m_pSystem->createSound("../../Sound/AI_Alliance_Sound/Ai_Man1_Attack_Short_6.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 60 / Enemy1 Att4 / Att4
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy1_Sound/Ai_Woman_Attack_Short_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 61 / Enemy2 Att4 / Att4
	eRes = m_pSystem->createSound("../../Sound/AI_Enemy2_Sound/Ai_Man2_Attack_Short_4.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 62 / LongBow Ultimate / Ultimate
	eRes = m_pSystem->createSound("../../Sound/LongBow/LB_Ultimate.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);

	// 63 / Ending_BGM
	eRes = m_pSystem->createSound("../../Sound/BGM/CartelBGM2.ogg", FMOD_DEFAULT, 0, &pSound);
	m_vecAiSound.push_back(pSound);
	ERRCHECK(eRes);
}


void CSoundMgr::BGMSTART(int iNum)
{
	m_pSystem->playSound(m_vecSound[iNum], 0, false, &m_pChannelBGM[BGM]);
	m_pChannelBGM[BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->update();
	m_pChannelBGM[BGM]->setVolume(0.5f);
	//FMOD_Channel_SetVolume(m_pChannelBGM[BGM],)
}

void CSoundMgr::SoundOn(_int iNum, _float fVolume)
{
	if (i > 990)
		i = 0;

	m_pSystem->playSound(m_vecSound[iNum], 0, false, &m_pChannelArr[i]);
	m_pSystem->update();
	m_pChannelArr[i]->setVolume(fVolume - m_fDefalultVolume);
	++i;
}

void CSoundMgr::HoSoundOn(_int iNum, _float fVolume)
{
	if (i > 990)
		i = 0;

	m_pSystem->playSound(m_vecHoSound[iNum], 0, false, &m_pChannelArr[i]);
	m_pSystem->update();
	m_pChannelArr[i]->setVolume(fVolume - m_fDefalultVolume);
	++i;
}

void CSoundMgr::SiwonSoundOn(_int iNum, _float fVolume)
{
	if (i > 990)
		i = 0;

	m_pSystem->playSound(m_vecSiwonSound[iNum], 0, false, &m_pChannelArr[i]);
	m_pSystem->update();
	m_pChannelArr[i]->setVolume(fVolume - m_fDefalultVolume);
	++i;
}

void CSoundMgr::HoBGM(_int iNum)
{
	m_pSystem->playSound(m_vecHoSound[iNum], 0, false, &m_pChannelBGM[BGM]);
	m_pChannelBGM[BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->update();
	m_pChannelBGM[BGM]->setVolume(0.5f);
}

void CSoundMgr::AiSoundOn(_int iNum, _float fVolume)
{
	if (i > 990)
		i = 0;

	m_pSystem->playSound(m_vecAiSound[iNum], 0, false, &m_pChannelBGM[AI]);
	m_pSystem->update();
	//cout << "volume setting : " << fVolume - m_fDefalultVolume << endl;
	_float Volume = fVolume - m_fDefalultVolume;
	if (0.f > Volume)
		Volume = 0.f;
	m_pChannelBGM[AI]->setVolume(Volume);
	++i;
}
