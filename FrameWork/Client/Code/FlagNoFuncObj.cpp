#include "stdafx.h"
#include "FlagNoFuncObj.h"
#include "ScreenTex.h"

CFlagNoFuncObj::CFlagNoFuncObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUIObject(pGraphicDev),
	m_eFlagNoFuncObjType(FLAGNOFUNCOBJ_END),
	m_fPosY(0.f),
	m_dGetFlagTxtTime(0.f)
{
}


CFlagNoFuncObj::~CFlagNoFuncObj()
{
}

HRESULT CFlagNoFuncObj::Ready_GameObject(FLAGNOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	if (FAILED(Clone_Component(fX, fY, fSizeX, fSizeY, fViewZ)))
		return E_FAIL;

	m_eFlagNoFuncObjType = eType;
	m_bRenderUI = bRender;

	return S_OK;
}

_int CFlagNoFuncObj::Update_GameObject(const _double & dTimeDelta)
{
	if (m_bIsDead)
	{
		if (FLAGNOFUNCOBJ_READYFLAGTXT == m_eFlagNoFuncObjType)
		{
			//깃발 재생성 시킨당.
			CUIMgr::GetInstance()->Set_ReCreateFlag(true);
		}

		return Engine::OBJ_DEAD;
	}

	CUIObject::Update_GameObject(dTimeDelta);

	if(FLAGNOFUNCOBJ_GETFLAGTXT == m_eFlagNoFuncObjType
		|| FLAGNOFUNCOBJ_FLAG == m_eFlagNoFuncObjType
		|| FLAGNOFUNCOBJ_READYFLAGTXT == m_eFlagNoFuncObjType
		|| FLAGNOFUNCOBJ_RECREATEFLAGTXT == m_eFlagNoFuncObjType)
		m_dGetFlagTxtTime += dTimeDelta;

	return Engine::NO_EVENT;
}

_int CFlagNoFuncObj::LateUpdate_GameObject(const _double & dTimeDelta)
{
	CUIObject::LateUpdate_GameObject(dTimeDelta);

	if (m_bRenderUI)
	{
		if (FLAGNOFUNCOBJ_ENDTXT == m_eFlagNoFuncObjType)
		{
			m_fPosY += _float(dTimeDelta)*20.f;

			if (20.f <= m_fPosY)
				m_fPosY = 20.f;

			//아래에서 위로
			m_pScreenTexBufferCom->VertexPositionYControl(-m_fPosY);
		}
		else if (FLAGNOFUNCOBJ_GETFLAGTXT == m_eFlagNoFuncObjType 
			|| FLAGNOFUNCOBJ_FLAG == m_eFlagNoFuncObjType
			|| FLAGNOFUNCOBJ_READYFLAGTXT == m_eFlagNoFuncObjType
			|| FLAGNOFUNCOBJ_RECREATEFLAGTXT == m_eFlagNoFuncObjType)
		{
			if (2.f <= m_dGetFlagTxtTime)
				m_bIsDead = true; 
		}
	}
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
	return Engine::NO_EVENT;
}

void CFlagNoFuncObj::Render_Geometry(const _double & dTimeDelta)
{
	if (m_bRenderUI)
	{
		//SHADER
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		if (pEffect == nullptr)
			return;
		Engine::Safe_AddRef(pEffect);

		if (FAILED(Setup_ShaderProps(pEffect)))
			return;

		_uint iPassMax = 0;

		pEffect->Begin(&iPassMax, 0);

		if(FLAGNOFUNCOBJ_FLAG==m_eFlagNoFuncObjType)
			pEffect->BeginPass(5);
		else
			pEffect->BeginPass(1);

		m_pScreenTexBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);

	}
}

HRESULT CFlagNoFuncObj::Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ)
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

	//Buffer
	pComponent = m_pScreenTexBufferCom = Engine::CScreenTex::Create(m_pGraphicDev, fX, fY, fSizeX, fSizeY, fViewZ);
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Texture_FlagNoFuncObj"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::TEXTURE, pComponent);

	//Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Shader_Normal"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_STATIC].emplace(Engine::SHADER, pComponent);

	return S_OK;
}

HRESULT CFlagNoFuncObj::Setup_ShaderProps(LPD3DXEFFECT & pEffect)
{
	CBaseObject::Set_ShaderMatrix(pEffect);

	if (FLAGNOFUNCOBJ_FLAG == m_eFlagNoFuncObjType)
		pEffect->SetVector("g_vRGBA", &_vec4(1.f,0.7f,0.f,1.f ));

	m_pTextureCom->SetTexture(pEffect, "g_DiffuseTexture", m_eFlagNoFuncObjType);
	return S_OK;
}

CFlagNoFuncObj * CFlagNoFuncObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, FLAGNOFUNCOBJ eType, _bool bRender, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CFlagNoFuncObj* pInstance = new CFlagNoFuncObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType, bRender, fX, fY, fSizeX, fSizeY, fViewZ)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlagNoFuncObj::Free()
{
	CUIObject::Free();
}
