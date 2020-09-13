#include "DynamicBuffer.h"

USING(Engine)

Engine::CDynamicBuffer::CDynamicBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev),
	m_dwCnt(0)
{

}

Engine::CDynamicBuffer::CDynamicBuffer(const CDynamicBuffer& rhs)
	: CVIBuffer(rhs),
	m_dwCnt(rhs.m_dwCnt)
{
}

Engine::CDynamicBuffer::~CDynamicBuffer(void)
{

}

HRESULT CDynamicBuffer::Ready_ProtoType(const _ulong & dwCnt)
{
	m_dwTriCnt = (dwCnt - 1) * 2;
	m_dwVtxCnt = dwCnt * 2;

	m_dwVtxFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_dwCnt = dwCnt;

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_dwVtxFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_DEFAULT,
		&m_pIB,
		NULL)))
		return E_FAIL;

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong		dwIndex = 0;

	for (_uint i = 0; i < dwCnt; ++i)
	{
		pVtxTex[i].vPos = _vec3(i * 3.f, 0, 0.f);
		pVtxTex[i].vTexUV = _vec2(i / (dwCnt - 1.f), 0.f);

		pVtxTex[i + dwCnt].vPos = _vec3(i * 3.f, 1.f, 0.f);
		pVtxTex[i + dwCnt].vTexUV = _vec2(i / (dwCnt - 1.f), 1.f);
	}

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	dwIndex = 0;

	for (_uint i = 0; i < dwCnt; ++i)
	{
		// ¿À¸¥ÂÊ »ï°¢Çü
		pIndex[dwIndex]._0 = dwCnt + i;
		pIndex[dwIndex]._1 = dwCnt + i + 1;
		pIndex[dwIndex]._2 = i + 1;
		++dwIndex;

		// ¿ÞÂÊ »ï°¢Çü
		pIndex[dwIndex]._0 = dwCnt + i;
		pIndex[dwIndex]._1 = i + 1;
		pIndex[dwIndex]._2 = i;
		++dwIndex;
	}

	m_pIB->Unlock();
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CDynamicBuffer::Ready_Buffer()
{
	return E_NOTIMPL;
}

_int CDynamicBuffer::Update_Buffer(list<_vec3> ListPos[])
{
	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	auto iterUp = ListPos[0].begin();
	auto iterDown = ListPos[1].begin();

	_ulong dwIndex = 0;

	for (_uint i = 0; i < m_dwCnt; ++i)
	{
		pVtxTex[i].vPos = *iterDown;
		pVtxTex[i].vTexUV = _vec2((float)(i / (m_dwCnt - 1.f)), 0.f);

		pVtxTex[i + m_dwCnt].vPos = *iterUp;
		pVtxTex[i + m_dwCnt].vTexUV = _vec2((float)(i / (m_dwCnt - 1.f)), 1.f);

		++iterDown;
		++iterUp;
	}

	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	dwIndex = 0;

	for (_uint i = 0; i < m_dwCnt; ++i)
	{
		// ¿À¸¥ÂÊ »ï°¢Çü
		pIndex[dwIndex]._0 = m_dwCnt + i;
		pIndex[dwIndex]._1 = m_dwCnt + i + 1;
		pIndex[dwIndex]._2 = i + 1;
		++dwIndex;

		// ¿ÞÂÊ »ï°¢Çü
		pIndex[dwIndex]._0 = m_dwCnt + i;
		pIndex[dwIndex]._1 = i + 1;
		pIndex[dwIndex]._2 = i;
		++dwIndex;
	}

	m_pIB->Unlock();
	m_pVB->Unlock();

	return 0;
}

CDynamicBuffer * CDynamicBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong & dwCnt)
{
	CDynamicBuffer*		pInstance = new CDynamicBuffer(pGraphicDev);

	if (FAILED(pInstance->Ready_ProtoType(dwCnt)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CDynamicBuffer::Clone()
{
	return new CDynamicBuffer(*this);
}

void CDynamicBuffer::Free(void)
{
	CVIBuffer::Free();
}


