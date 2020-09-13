#include "Transform.h"


USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	m_vScale(1.f, 1.f, 1.f),
	m_vAngle(0.f, 0.f, 0.f),
	m_fSpeed(1.f),
	m_pParent(nullptr),
	m_vRevAngle(0.f, 0.f, 0.f),
	m_vSelfRotAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(_vec3) * INFO_END);
}

CTransform::CTransform(const CTransform & rhs)
	:CComponent(rhs),
	m_vScale(rhs.m_vScale),
	m_vAngle(rhs.m_vAngle),
	m_fSpeed(rhs.m_fSpeed),
	m_pParent(rhs.m_pParent),
	m_vRevAngle(rhs.m_vRevAngle),
	m_vSelfRotAngle(rhs.m_vSelfRotAngle)
{
	ZeroMemory(m_vInfo, sizeof(_vec3) * INFO_END);
}


CTransform::~CTransform()
{
}

_vec3 * CTransform::Get_Info(INFO eType)
{
	return &m_vInfo[eType];
}

void CTransform::Get_Info(INFO eType, _vec3 * pVector)
{
	memcpy(pVector, &m_vInfo[eType], sizeof(_vec3));
}

void CTransform::Get_Info_byWorld(INFO eType, _vec3 * pVector)
{
	memcpy(pVector, &m_matWorld.m[eType][0], sizeof(_vec3));
}

_vec3 CTransform::Get_Info_byWorld(INFO eType)
{
	_vec3 vPos;
	memcpy(&vPos, &m_matWorld.m[eType][0], sizeof(_vec3));
	return vPos;
}

_vec3 * CTransform::Get_Scale()
{
	return &m_vScale;
}

_vec3 * CTransform::Get_Angle()
{
	return &m_vAngle;
}

_vec3 * CTransform::Get_SelfRotAngle()
{
	return &m_vSelfRotAngle;
}

const _vec3 * CTransform::Get_Revolution()
{
	return &m_vRevAngle;
}

_float CTransform::Get_Speed()
{
	return m_fSpeed;
}

void CTransform::Get_WorldMatrix(_matrix * pWorld) const
{
	*pWorld = m_matWorld;
}

const _matrix * CTransform::Get_WorldMatrix() const
{
	return &m_matWorld;
}

const _matrix * CTransform::Get_NRotWorldMatrix(_matrix * pWorld) const
{
	if (nullptr != pWorld)
		*pWorld = m_matNRotWorld;

	return &m_matNRotWorld; return nullptr;
}

_matrix * CTransform::Get_WorldMatrix()
{
	return &m_matWorld;
}

void CTransform::Set_Info(_float fX, _float fY, _float fZ, INFO eType)
{
	m_vInfo[eType] = _vec3(fX, fY, fZ);
}

void CTransform::Set_Info(_vec3 * pInfo, INFO eType)
{
	m_vInfo[eType] = *pInfo;
}

void CTransform::Set_Scale(_float fX, _float fY, _float fZ)
{
	m_vScale = _vec3(fX, fY, fZ);
}

void CTransform::Set_Scale(_vec3 * pScale)
{
	m_vScale = *pScale;
}

void CTransform::Set_Angle(_float fX, _float fY, _float fZ)
{
	m_vAngle = _vec3(fX, fY, fZ);
}

void CTransform::Set_Angle(_vec3 * pAngle)
{
	m_vAngle = *pAngle;
}

void CTransform::Set_Revolution(_float fX, _float fY, _float fZ)
{
	m_vRevAngle = _vec3(fX, fY, fZ);
}

void CTransform::Set_Revolution(_vec3 * pRevolution)
{
	m_vRevAngle = *pRevolution;
}

void CTransform::Set_Speed(_float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CTransform::Set_Matrix(const _matrix * pMatrix)
{
	m_matWorld = *pMatrix;
}

void CTransform::Set_ParentMatrix(const _matrix * pParent)
{
	m_matWorld *= *pParent;
}

void CTransform::Set_Pos(_float fX, _float fY, _float fZ)
{
	m_vInfo[INFO_POS] = _vec3(fX, fY, fZ);
}

void CTransform::Set_Pos(_vec3 * pPos)
{
	m_vInfo[INFO_POS] = *pPos;
}

void CTransform::Set_RevParentMatrix(_matrix * pParent)
{
	m_pParent = pParent;
}

void CTransform::Set_ParentPos(_float fX, _float fY, _float fZ)
{
	m_pParent->_41 = fX;
	m_pParent->_42 = fY;
	m_pParent->_43 = fZ;
}

void CTransform::Set_ParentPos(_vec3 * pPos)
{
	m_pParent->_41 = pPos->x;
	m_pParent->_42 = pPos->y;
	m_pParent->_43 = pPos->z;
}

void CTransform::Set_SelfRotAngle(_vec3* vSelfRotAngle)
{
	m_vSelfRotAngle = *vSelfRotAngle;
}

float CTransform::Calculate_Distance(_vec3 vPos)
{
	return D3DXVec3Length(&(m_vInfo[INFO_POS] - vPos));
}

void CTransform::Move_Pos(const _vec3 * pDir, _double dSpeed, const _double & dTimeDelta)
{
	m_vInfo[INFO_POS] += *pDir * _float(dTimeDelta * dSpeed);
}

void CTransform::Move_Parent(const _vec3 * pDir, _double dSpeed, const _double & dTimeDelta)
{
	m_pParent->_41 += (pDir->x * _float(dTimeDelta * dSpeed));
	m_pParent->_42 += (pDir->y * _float(dTimeDelta * dSpeed));
	m_pParent->_43 += (pDir->z * _float(dTimeDelta * dSpeed));
}

void CTransform::Rotation(ROTATION eType, const _float & fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}

void CTransform::Revolution(ROTATION eType, const _float & fAngle)
{
	*(((_float*)&m_vRevAngle) + eType) += fAngle;
}

const _matrix * CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];
	_matrix	matRotAxis;
	_vec3	vAxis, vUp;

	return D3DXMatrixRotationAxis(&matRotAxis,
		D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], D3DXVec3Normalize(&vDir, &vDir)),
		acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
			D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void CTransform::Chase_Target(const _vec3 * pTargetPos, const _float & fSpeed)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed;

	_matrix		matRot = *Compute_LookAtTarget(pTargetPos);
	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

void CTransform::Look_Target(_vec3 pTargetPos)
{
	_vec3	vLook = m_vInfo[INFO_LOOK];								// 몬스터의 Look
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vMonsterToPlayer = pTargetPos - m_vInfo[INFO_POS];		// 몬스터에서 플레이어로 향하는 벡터
	D3DXVec3Normalize(&vMonsterToPlayer, &vMonsterToPlayer);

	float fAngle = Engine::GetAngleY(vLook, vMonsterToPlayer);

	if (!isnan(fAngle))
	{
		Rotation(Engine::ROT_Y, fAngle / 5);
	}

	//if (-3 < fAngle && fAngle < 3)
	//{

	//}
	//else
	//{
	//	// GetAngleY가 알아서 플러스 마이너스로 값을건네주니까 여기선 그냥 +만 해도 됌
	//	m_vAngle.y += (fAngle / 10);
	//}
}

_bool CTransform::Change_Scale(_vec3 * vScale)
{
	m_vScale += *vScale;
	if (m_vScale.x < 0.f)
		return false;
	else
		return true;
	return true;
}

void CTransform::RotationSelf(ROTATION eType, const _float & fAngle)
{
	_matrix matRotate;
	D3DXMatrixIdentity(&matRotate);
	if (eType == ROT_X)
	{
		D3DXMatrixRotationX(&matRotate, D3DXToRadian(fAngle));
	}
	else if (eType == ROT_Y)
	{
		D3DXMatrixRotationY(&matRotate, D3DXToRadian(fAngle));
	}
	else  if (eType == ROT_Z)
	{
		D3DXMatrixRotationZ(&matRotate, D3DXToRadian(fAngle));
	}
	matRotate *= m_matWorld;
	m_matWorld = matRotate;
}

void CTransform::Revolution_XZ(_double dAccTime, const _float & fAngle, _bool bRight)
{
	_vec3 vRight, vUp, vPos;
	memcpy(&vRight, &m_matWorld.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &m_matWorld.m[1][0], sizeof(_vec3));
	memcpy(&vPos, &m_pParent->m[3][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	if (bRight)
	{
		vPos += vRight * -cosf(dAccTime) * 5.f;
		vPos += vUp * sinf(dAccTime) * 5.f;
	}
	else
	{
		vPos += vRight * cosf(dAccTime) * 5.f;
		vPos += vUp * -sinf(dAccTime) * 5.f;
	}
	
	Set_ParentPos(&vPos);
}

void CTransform::RotationParent(ROTATION eType, const _float & fAngle)
{
	_matrix matRotate;
	D3DXMatrixIdentity(&matRotate);
	if (eType == ROT_X)
	{
		D3DXMatrixRotationX(&matRotate, D3DXToRadian(fAngle));
	}
	else if (eType == ROT_Y)
	{
		D3DXMatrixRotationY(&matRotate, D3DXToRadian(fAngle));
	}
	else if (eType == ROT_Z)
	{
		D3DXMatrixRotationZ(&matRotate, D3DXToRadian(fAngle));
	}
	matRotate *= *m_pParent;
	*m_pParent = matRotate;
}


HRESULT CTransform::Ready_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Ready_Transform()
{
	return S_OK;
}

_int CTransform::Update_Component(const _double & dTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// Scale
	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	// Rotation
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], D3DXToRadian(m_vAngle.x));
	D3DXMatrixRotationY(&matRot[ROT_Y], D3DXToRadian(m_vAngle.y));
	D3DXMatrixRotationZ(&matRot[ROT_Z], D3DXToRadian(m_vAngle.z));

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	//노디가 주석 달았음  데미지폰트에서 안써서 필요하면 주석푸삼
	if (m_pParent != nullptr)
	{
		_matrix			matRev[ROT_END];

		D3DXMatrixRotationX(&matRev[ROT_X], m_vRevAngle.x);
		D3DXMatrixRotationY(&matRev[ROT_Y], m_vRevAngle.y);
		D3DXMatrixRotationZ(&matRev[ROT_Z], m_vRevAngle.z);

		m_matWorld *= matRev[ROT_X] * matRev[ROT_Y] * matRev[ROT_Z] * (*m_pParent);
	}

	return NO_EVENT;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
