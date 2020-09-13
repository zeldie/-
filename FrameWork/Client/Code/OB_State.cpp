#include "stdafx.h"
#include "OB_State.h"

COB_State::COB_State()
{

}

COB_State::~COB_State()
{

}

void COB_State::Enter(CPlayer * pPlayer)
{
	m_pMeshCom = pPlayer->Get_PlayerMesh();
	m_pMeshCom = pPlayer->Get_PlayerMesh();
	m_pTransform = pPlayer->Get_TransformCom();
	m_fSpeed = pPlayer->Get_Speed();
	pPlayer->Set_BattleType(CPlayer::TYPE_BATTLE);
}

void COB_State::Update(CPlayer * pPlayer, const _double dTimeDelta)
{

}

void COB_State::ChangeState(CPlayerState * pState)
{
}