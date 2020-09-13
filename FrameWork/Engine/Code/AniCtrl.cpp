#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_dAccTime(0)
	, m_iOldAniIdx(999)
	, m_dbPeriod(0.0)
	, m_bStop(false)
{
	Safe_AddRef(m_pAniCtrl);
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_dAccTime(rhs.m_dAccTime)
	, m_iOldAniIdx(rhs.m_iOldAniIdx)
	, m_dbPeriod(0.0)
{
	rhs.m_pAniCtrl->CloneAnimationController(
		rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),
		rhs.m_pAniCtrl->GetMaxNumTracks(),
		rhs.m_pAniCtrl->GetMaxNumEvents(),
		&m_pAniCtrl);
	Safe_AddRef(m_pAniCtrl);
}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AniCtrl(void)
{
	return S_OK;
}

void Engine::CAniCtrl::Set_AnimationSet(const _uint& iIndex)
{
	if (m_iOldAniIdx == iIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = NULL;

	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName();

	m_dbPeriod = pAS->GetPeriod();	// 현재 애니메이션 셋의 전체 재생 시간

									// 트랙 위에 애니메이션 셋을 설정(설치)하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 사용하지 않은 이벤트 정보들을 없는 것으로 처리하는 함수(이벤트들 때문에 보간이 안되는 경우가 간혹 발생)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 애니메이션이 재생되는 트랙을 설정하는 함수(2인자 : on/off, 3인자 : 어느 시점)
	// 애니메이션 재생 시 사용되는 시간 값은 모두 double형
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + 0.25);

	// 현재 트랙의 재생 속도를 제어하는 함수
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 현재 트랙의 재생 가중치를 설정하는 함수
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);


	// 트랙 활성화 여부를 묻는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // advancedTime 함수 호출 시 누적되던 시간 값을 초기화 하는 함수
	m_dAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;

	m_iCurrentTrack = m_iNewTrack;
}

void CAniCtrl::Set_AnimationSet_Change(const _uint& iIndex, _double dTrackTime, _double dAccTime)
{
	m_dAccTime = dAccTime;
	_uint iTempIndex = iIndex + 1;
	if (m_iOldAniIdx == iTempIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = NULL;

	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName();

	m_dbPeriod = pAS->GetPeriod();	// 현재 애니메이션 셋의 전체 재생 시간

									// 트랙 위에 애니메이션 셋을 설정(설치)하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 사용하지 않은 이벤트 정보들을 없는 것으로 처리하는 함수(이벤트들 때문에 보간이 안되는 경우가 간혹 발생)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 애니메이션이 재생되는 트랙을 설정하는 함수(2인자 : on/off, 3인자 : 어느 시점)
	// 애니메이션 재생 시 사용되는 시간 값은 모두 double형
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + 0.25);

	// 현재 트랙의 재생 속도를 제어하는 함수
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 현재 트랙의 재생 가중치를 설정하는 함수
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);


	// 트랙 활성화 여부를 묻는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	//m_pAniCtrl->ResetTime(); // advancedTime 함수 호출 시 누적되던 시간 값을 초기화 하는 함수
	//m_dAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, dAccTime);

	m_iOldAniIdx = iIndex;

	m_iCurrentTrack = m_iNewTrack;
}

void CAniCtrl::Set_TrackTime(_uint iCurrentTrack, _double dTrackTime)
{
	m_pAniCtrl->SetTrackPosition(iCurrentTrack, dTrackTime);
}

void Engine::CAniCtrl::Play_AnimationSet(const _double& dTimeDelta)
{
	if (!m_bStop)
	{
		m_pAniCtrl->AdvanceTime(dTimeDelta, NULL);	// 2인자 : 애니메이션 동작에 따른 이펙트나 사운드에 대한 담당 객체 주소, 하지만 우리는 직접 만들어서 사용한다.

		m_dAccTime += dTimeDelta;
	}
	else
	{
		m_pAniCtrl->AdvanceTime(0, NULL);
	}
}

void CAniCtrl::Reset_Animation()
{
	m_pAniCtrl->ResetTime(); // advancedTime 함수 호출 시 누적되던 시간 값을 초기화 하는 함수
	//m_dAccTime = 0.;
	m_dbPeriod = 0.;
}

CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*	pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);
	
	return pInstance;
}

CAniCtrl* Engine::CAniCtrl::Clone(const CAniCtrl& rhs)
{
	CAniCtrl*	pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

_double & CAniCtrl::Get_TrackTime()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	D3DXTRACK_DESC			tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);
	return tTrackInfo.Position;
}

_bool Engine::CAniCtrl::Is_AnimationSetFinish(const _double dbRevisionTime)
{
	D3DXTRACK_DESC			tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	if (tTrackInfo.Position >= m_dbPeriod - 0.1 - dbRevisionTime)
		return true;

	return false;
}

