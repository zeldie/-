#include "stdafx.h"
#include "Water.h"


CWater::CWater(LPDIRECT3DDEVICE9 pGraphicDev)
	:CStaticMeshObject(pGraphicDev)
{
}


CWater::~CWater()
{
}

HRESULT CWater::Ready_GameObject(_vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	if (FAILED(Clone_Component()))
		return E_FAIL;
	return S_OK;
}

_int CWater::Update_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::Update_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

_int CWater::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CStaticMeshObject::LateUpdate_GameObject(dTimeDelta);
	return Engine::NO_EVENT;
}

void CWater::Render_Geometry(const _double & dTimeDelta)
{
	//SHADER
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	Engine::Safe_AddRef(pEffect);

	if (FAILED(Setup_ShaderProps(pEffect)))
		return;
	_ulong dwSubset = m_pStaticMeshCom->Get_SubsetNum();
	_uint iPassMax = 0;

	for (_ulong i = 0; i < dwSubset; ++i)
	{
		if (FAILED(pEffect->SetTexture("g_DiffuseTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_DIFFUSE, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_NormalTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_NORMAL, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_SpecularTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_SPECULAR, i))))
			return;
		if (FAILED(pEffect->SetTexture("g_EmmisiveTexture", m_pStaticMeshCom->Get_Texture(Engine::MESHTEXTURE_EMMISIVE, i))))
			return;
		pEffect->CommitChanges();
		pEffect->Begin(&iPassMax, 0);
		pEffect->BeginPass(0);
		m_pStaticMeshCom->Render_Meshes(i);
		pEffect->EndPass();
		pEffect->End();
	}
	Engine::Safe_Release(pEffect);
}

HRESULT CWater::Clone_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone(Engine::RESOURCE_STATIC, L"TransformCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::TRANSFORM, pComponent);

	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone(Engine::RESOURCE_STATIC, L"RendererCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::RENDERER, pComponent);

	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Mesh_Water"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::MESH, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Water"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CWater::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);
	_matrix matInvWorld;
	m_pTransformCom->Get_WorldMatrix(&matInvWorld);
	D3DXMatrixInverse(&matInvWorld, NULL, &matInvWorld);
	pEffect->SetMatrix("g_WorldInv", &matInvWorld);

	Mtrl WaterMtrl;
	WaterMtrl.vambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	WaterMtrl.vdiffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	WaterMtrl.vspec = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);
	WaterMtrl.fspecPower = 200.f;

	DirLight tDirLight;
	tDirLight.vdiffuse = Engine::Get_Light(0)->Diffuse;
	tDirLight.vambient = Engine::Get_Light(0)->Ambient;
	tDirLight.vspec = Engine::Get_Light(0)->Specular;
	tDirLight.vdirW = Engine::Get_Light(0)->Direction;

	pEffect->SetValue("g_Mtrl", &WaterMtrl, sizeof(Mtrl));
	pEffect->SetValue("g_Light", &tDirLight, sizeof(DirLight));
	pEffect->SetFloat("fRefractBias", m_fRefractBias);
	pEffect->SetFloat("fRefractPower", m_fRefractPower);
	pEffect->SetValue("vRippleScale", &m_vRippleScale, sizeof(_vec2));

	pEffect->SetValue("vCameraPos", &vCamPos, sizeof(_vec3));
	pEffect->SetValue("vWaveMapOffset0", &m_vWaveOffset0, sizeof(_vec2));
	pEffect->SetValue("vWaveMapOffset1", &m_vWaveOffset1, sizeof(_vec2));


	return S_OK;
}

CWater * CWater::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos, _vec3 * pAngle, _vec3 * pScale)
{
	CWater* pInstance = new CWater(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(pPos, pAngle, pScale)))
		Engine::Safe_Release(pInstance);
	return pInstance;
}

void CWater::Free()
{
	CStaticMeshObject::Free();
}
