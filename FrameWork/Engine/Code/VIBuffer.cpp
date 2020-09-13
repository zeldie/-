#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwTriCnt(0)
	, m_dwVtxFVF(0)
	, m_dwIdxSize(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT Engine::CVIBuffer::Ready_Prototype()
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,	// 그리고자 하는 버텍스의 크기
		0,						// 버퍼 사용 방식(0인 정적 버퍼)
		m_dwVtxFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Ready_Buffer()
{
	return S_OK;
}

HRESULT Engine::CVIBuffer::Render_Buffer()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);	// 버텍스 버퍼를 디바이스 장치와 링크하는 함수
	m_pGraphicDev->SetFVF(m_dwVtxFVF);	// 장치와 연결한 버텍스의 속성
	m_pGraphicDev->SetIndices(m_pIB);	// 장치와 인덱스 버퍼를 링크시키는 함수

	// 인덱스를 기준으로 폴리곤을 그리는 함수
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	return S_OK;
}

void Engine::CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();
}

