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

	m_dbPeriod = pAS->GetPeriod();	// ���� �ִϸ��̼� ���� ��ü ��� �ð�

									// Ʈ�� ���� �ִϸ��̼� ���� ����(��ġ)�ϴ� �Լ�
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// ������� ���� �̺�Ʈ �������� ���� ������ ó���ϴ� �Լ�(�̺�Ʈ�� ������ ������ �ȵǴ� ��찡 ��Ȥ �߻�)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// �ִϸ��̼��� ����Ǵ� Ʈ���� �����ϴ� �Լ�(2���� : on/off, 3���� : ��� ����)
	// �ִϸ��̼� ��� �� ���Ǵ� �ð� ���� ��� double��
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + 0.25);

	// ���� Ʈ���� ��� �ӵ��� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// ���� Ʈ���� ��� ����ġ�� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);


	// Ʈ�� Ȱ��ȭ ���θ� ���� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // advancedTime �Լ� ȣ�� �� �����Ǵ� �ð� ���� �ʱ�ȭ �ϴ� �Լ�
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

	m_dbPeriod = pAS->GetPeriod();	// ���� �ִϸ��̼� ���� ��ü ��� �ð�

									// Ʈ�� ���� �ִϸ��̼� ���� ����(��ġ)�ϴ� �Լ�
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// ������� ���� �̺�Ʈ �������� ���� ������ ó���ϴ� �Լ�(�̺�Ʈ�� ������ ������ �ȵǴ� ��찡 ��Ȥ �߻�)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// �ִϸ��̼��� ����Ǵ� Ʈ���� �����ϴ� �Լ�(2���� : on/off, 3���� : ��� ����)
	// �ִϸ��̼� ��� �� ���Ǵ� �ð� ���� ��� double��
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + 0.25);

	// ���� Ʈ���� ��� �ӵ��� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// ���� Ʈ���� ��� ����ġ�� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);


	// Ʈ�� Ȱ��ȭ ���θ� ���� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	//m_pAniCtrl->ResetTime(); // advancedTime �Լ� ȣ�� �� �����Ǵ� �ð� ���� �ʱ�ȭ �ϴ� �Լ�
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
		m_pAniCtrl->AdvanceTime(dTimeDelta, NULL);	// 2���� : �ִϸ��̼� ���ۿ� ���� ����Ʈ�� ���忡 ���� ��� ��ü �ּ�, ������ �츮�� ���� ���� ����Ѵ�.

		m_dAccTime += dTimeDelta;
	}
	else
	{
		m_pAniCtrl->AdvanceTime(0, NULL);
	}
}

void CAniCtrl::Reset_Animation()
{
	m_pAniCtrl->ResetTime(); // advancedTime �Լ� ȣ�� �� �����Ǵ� �ð� ���� �ʱ�ȭ �ϴ� �Լ�
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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
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

