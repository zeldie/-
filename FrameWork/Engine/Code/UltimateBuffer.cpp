#include "UltimateBuffer.h"

USING(Engine)

Engine::CUltimateBuffer::CUltimateBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwTriCnt(0)
	, m_dwVtxFVF(0)
{

}

Engine::CUltimateBuffer::CUltimateBuffer(const CUltimateBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
{
	Safe_AddRef(m_pVB);
}

CUltimateBuffer::~CUltimateBuffer()
{
}

HRESULT Engine::CUltimateBuffer::Ready_Prototype()
{

	m_dwVtxCnt = 34;
	m_dwVtxFVF = FVF_ULTIMATE;
	m_dwTriCnt = 32;
	m_dwVtxSize = sizeof(ULTIMATETEX);

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,	// 그리고자 하는 버텍스의 크기
		0,						// 버퍼 사용 방식(0인 정적 버퍼)
		m_dwVtxFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;


	ULTIMATETEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0] = { 0.5f,	-0.5f,	0.f,		0.5f,	0.5f,		0.5f,	 0.5f };
	pVertex[1] = { 0.5f,     0.f,	0.f,		0.5f,	0.f,		0.5f,    0.f };
	pVertex[2] = { 0.375f,   0.f,	0.f,		0.375f,	0.f,		0.375f,  0.f };
	pVertex[3] = { 0.25f,    0.f,	0.f,		0.25f,	0.f,		0.25f,   0.f };
	pVertex[4] = { 0.125f,   0.f,	0.f,		0.125f,	0.f,		0.125f,  0.f };

	pVertex[5] = { 0.f,		 0.f,	0.f,		0.f,	0.f,		0.f,	0.f };

	pVertex[6] = { 0.f,		-0.125f, 0.f,		0.f,	0.125f,		0.f,	0.125f };
	pVertex[7] = { 0.f,		-0.25f,	 0.f,		0.f,	0.25f,		0.f,	0.25f };
	pVertex[8] = { 0.f,		-0.375f, 0.f,		0.f,	0.375f,		0.f,	0.375f };
	pVertex[9] = { 0.f,		-0.5f,	 0.f,		0.f,	0.5f,		0.f,	0.5f };
	pVertex[10] = { 0.f,	-0.625f, 0.f,		0.f,	0.625f,		0.f,	0.625f };
	pVertex[11] = { 0.f,	-0.75f,  0.f,		0.f,	0.75f,		0.f,	0.75f };
	pVertex[12] = { 0.f,	-0.875f, 0.f,		0.f,	0.875f,		0.f,	0.875f };

	pVertex[13] = { 0.f,    -1.f,	 0.f,		0.f,    1.f,		0.f,    1.f };

	pVertex[14] = { 0.125f,  -1.f,	 0.f,		0.125f,  1.f,		0.125f,  1.f };
	pVertex[15] = { 0.25f,   -1.f, 	 0.f,		0.25f,   1.f,		0.25f,   1.f };
	pVertex[16] = { 0.375f,  -1.f,	 0.f,		0.375f,  1.f,		0.375f,  1.f };
	pVertex[17] = { 0.5f,    -1.f,	 0.f,		0.5f,    1.f,		0.5f,    1.f };
	pVertex[18] = { 0.625f,  -1.f,	 0.f,		0.625f,  1.f,		0.625f,  1.f };
	pVertex[19] = { 0.75f,   -1.f,	 0.f,		0.75f,   1.f,		0.75f,   1.f };
	pVertex[20] = { 0.875f,  -1.f,	 0.f,		0.875f,  1.f,		0.875f,  1.f };

	pVertex[21] = { 1.f,     -1.f,	 0.f,		1.f,     1.f,		1.f,     1.f };

	pVertex[22] = { 1.f,	 -0.875f, 0.f,		1.f,	 0.875f,    1.f,	 0.875f };
	pVertex[23] = { 1.f,	 -0.75f,  0.f,		1.f,	 0.75f,     1.f,	 0.75f };
	pVertex[24] = { 1.f,	 -0.625f, 0.f,		1.f,	 0.625f,    1.f,	 0.625f };
	pVertex[25] = { 1.f,	 -0.5f,   0.f,		1.f,	 0.5f,      1.f,	 0.5f };
	pVertex[26] = { 1.f,	 -0.375f, 0.f,		1.f,	 0.375f,    1.f,	 0.375f };
	pVertex[27] = { 1.f,	 -0.25f,  0.f,		1.f,	 0.25f,     1.f,	 0.25f };
	pVertex[28] = { 1.f,	 -0.125f, 0.f,		1.f,	 0.125f,    1.f,	 0.125f };

	pVertex[29] = { 1.f,	  0.f,    0.f,     1.f,		 0.f,		1.f,     0.f };
	pVertex[30] = { 0.875f,   0.f,	  0.f,	   0.875f,   0.f,		0.875f,  0.f };
	pVertex[31] = { 0.75f,    0.f,    0.f,	   0.75f,    0.f,		0.75f,   0.f };
	pVertex[32] = { 0.625f,   0.f,	  0.f,	   0.625f,   0.f,		0.625f,  0.f };
	pVertex[33] = { 0.5f,     0.f,	  0.f,	   0.5f,     0.f,		0.5f,    0.f };

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CUltimateBuffer::Ready_Buffer()
{
	return S_OK;
}

HRESULT Engine::CUltimateBuffer::Render_Buffer(_uint iCount)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);	// 버텍스 버퍼를 디바이스 장치와 링크하는 함수
	m_pGraphicDev->SetFVF(m_dwVtxFVF);	// 장치와 연결한 버텍스의 속성

	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, iCount);
	//m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, iCount, m_pVB, sizeof(ULTIMATETEX));
	return S_OK;
}

Engine::CUltimateBuffer* Engine::CUltimateBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUltimateBuffer*	pInstance = new CUltimateBuffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);


	return pInstance;
}

CComponent * CUltimateBuffer::Clone(void)
{
	return nullptr;
	//return new CUltimateBuffer(*this);
}

void Engine::CUltimateBuffer::Free()
{
	Safe_Release(m_pVB);

	CComponent::Free();
}

