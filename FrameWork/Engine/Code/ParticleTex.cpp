#include "ParticleTex.h"

USING(Engine)

Engine::CParticleTex::CParticleTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CParticleTex::CParticleTex(const CParticleTex& rhs)
	:CVIBuffer(rhs)
{

}

CParticleTex::~CParticleTex()
{
}

HRESULT CParticleTex::Ready_Buffer()
{
	m_dwVtxCnt = 100;
	m_dwVtxFVF = FVF_PARTICLE;
	m_dwVtxSize = sizeof(PARTICLETEX);

	FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		m_dwVtxFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		NULL));

	D3DVERTEXELEMENT9	Element[MAX_FVF_DECL_SIZE] = {
		// 0�� ��Ʈ���� ����ϴ� ���ؽ��� ���� ���
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,	0 },
		{ 0, 16, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE,		0 },

		//// 1�� ��Ʈ���� ����ϴ� ���ؽ��� ���� ���
		//{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1 },
		//{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },
		//{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3 },
		//{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4 },

		D3DDECL_END()
	};

	FAILED(m_pGraphicDev->CreateVertexDeclaration(Element, &m_pDeclaration));

	//PARTICLETEX*		pVertex = nullptr;

	//m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	//pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	//pVertex[0].vTexUV = _vec2(0.f, 0.f);

	//pVertex[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	//pVertex[1].vTexUV = _vec2(1.f, 0.f);

	//pVertex[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	//pVertex[2].vTexUV = _vec2(1.f, 1.f);

	//pVertex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	//pVertex[3].vTexUV = _vec2(0.f, 1.f);

	//m_pVB->Unlock();

	return S_OK;
}

void CParticleTex::Update_Buffer(list<PARTICLE_INFO> listParticleInfo)
{
	PARTICLETEX*	pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	list<PARTICLE_INFO>::iterator iter;
	for (iter = listParticleInfo.begin(); iter != listParticleInfo.end(); ++iter)
	{
		pVertex->vPos = iter->vPos;
		pVertex->Color = iter->Color;
		++pVertex;
	}

	m_pVB->Unlock();
}

void CParticleTex::Render_InstanceBuffer(list<PARTICLE_INFO> listParticleInfo, _float fSize)
{
	float	fa = 0.f;
	float	fb = 1.f;
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fSize));	//��ƼŬ Ŭ������ ������ �Է¹޾Ƽ� �װ� ������ ��������.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&fa));

	// �Ÿ��� ���� ��ƼŬ ũ��
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*)&fa));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&fa));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&fb));

	// �ؽ�ó�� ����
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//=======================================================================================================================================

	Update_Buffer(listParticleInfo);


	//m_pGraphicDev->SetVertexDeclaration(m_pDeclaration);	// SetFVF�� ���� ������ ������ �������ִ� �Լ�
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	//m_pGraphicDev->SetIndices(m_pIB);

	// ��ġ�� ��Ʈ�� ���۸� ���ε� ��Ű�� �Լ�
	m_pGraphicDev->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_dwVtxCnt);	// 1���� : ��Ʈ�� ��ȣ, 2���� : �ν��Ͻ� ������ŭ D3DSTREAMSOURCE_INDEXEDDATA�� OR ������ ���� ������ ������ ������ ��, ��ƼŬ �ÿ��� 1�� ���
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	//m_pGraphicDev->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);		// 1���� : ��Ʈ�� ��ȣ, 2���� : �ν��Ͻ� ������ŭ D3DSTREAMSOURCE_INSTANCEDATA�� OR ������ ���� ������ ������ ������ ��, ��ƼŬ �ÿ��� 1�� ���
	//m_pGraphicDev->SetStreamSource(1, m_pVBTransform, 0, sizeof(VTXMATRIX));

	//m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, 0, m_dwVtxCnt);

	m_pGraphicDev->SetStreamSourceFreq(0, 1);				// �ν��Ͻ� ������ �������� ��ġ�� ���� ��Ʈ���� ���ļ��� �⺻ ���·� �������� ����
	//m_pGraphicDev->SetStreamSourceFreq(1, 1);

	//=======================================================================================================================================
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

CParticleTex * CParticleTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticleTex*	pInstance = new CParticleTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CParticleTex::Clone()
{
	//��������
	CParticleTex*	pInstance = new CParticleTex(m_pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
	//return new CParticleTex(*this);
}

void CParticleTex::Free()
{
	Safe_Release(m_pDeclaration);
	Safe_Release(m_pVBTransform);

	CVIBuffer::Free();
}
