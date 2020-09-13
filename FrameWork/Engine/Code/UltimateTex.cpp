#include "UltimateTex.h"

USING(Engine)

Engine::CUltimateTex::CUltimateTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUltimateBuffer(pGraphicDev)
{

}

Engine::CUltimateTex::CUltimateTex(const CUltimateTex& rhs)
	: CUltimateBuffer(rhs)
{

}

CUltimateTex::~CUltimateTex()
{
}

HRESULT Engine::CUltimateTex::Ready_Prototype()
{
	//m_dwVtxCnt = 34;
	//m_dwVtxFVF = FVF_ULTIMATE;
	//m_dwTriCnt = 32;
	//m_dwVtxSize = sizeof(ULTIMATETEX);

	//if (FAILED(CUltimateBuffer::Ready_Prototype()))
	//	return E_FAIL;

	//ULTIMATETEX* pVertex = nullptr;
	//m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	//pVertex[0] = { 0.5f,	-0.5f,	0.f,		0.5f,	0.5f,		0.5f,	 0.5f };
	//pVertex[1] = { 0.5f,     0.f,	0.f,		0.5f,	0.f,		0.5f,    0.f };
	//pVertex[2] = { 0.375f,   0.f,	0.f,		0.375f,	0.f,		0.375f,  0.f };
	//pVertex[3] = { 0.25f,    0.f,	0.f,		0.25f,	0.f,		0.25f,   0.f };
	//pVertex[4] = { 0.125f,   0.f,	0.f,		0.125f,	0.f,		0.125f,  0.f };

	//pVertex[5] = { 0.f,		 0.f,	0.f,		0.f,	0.f,		0.f,	0.f };

	//pVertex[6] = { 0.f,		-0.125f, 0.f,		0.f,	0.125f,		0.f,	0.125f };
	//pVertex[7] = { 0.f,		-0.25f,	 0.f,		0.f,	0.25f,		0.f,	0.25f };
	//pVertex[8] = { 0.f,		-0.375f, 0.f,		0.f,	0.375f,		0.f,	0.375f };
	//pVertex[9] = { 0.f,		-0.5f,	 0.f,		0.f,	0.5f,		0.f,	0.5f };
	//pVertex[10] = { 0.f,	-0.625f, 0.f,		0.f,	0.625f,		0.f,	0.625f };
	//pVertex[11] = { 0.f,	-0.75f,  0.f,		0.f,	0.75f,		0.f,	0.75f };
	//pVertex[12] = { 0.f,	-0.875f, 0.f,		0.f,	0.875f,		0.f,	0.875f };

	//pVertex[13] = { 0.f,    -1.f,	 0.f,		0.f,    1.f,		0.f,    1.f };

	//pVertex[14] = { 0.125f,  -1.f,	 0.f,		0.125f,  1.f,		0.125f,  1.f };
	//pVertex[15] = { 0.25f,   -1.f, 	 0.f,		0.25f,   1.f,		0.25f,   1.f };
	//pVertex[16] = { 0.375f,  -1.f,	 0.f,		0.375f,  1.f,		0.375f,  1.f };
	//pVertex[17] = { 0.5f,    -1.f,	 0.f,		0.5f,    1.f,		0.5f,    1.f };
	//pVertex[18] = { 0.625f,  -1.f,	 0.f,		0.625f,  1.f,		0.625f,  1.f };
	//pVertex[19] = { 0.75f,   -1.f,	 0.f,		0.75f,   1.f,		0.75f,   1.f };
	//pVertex[20] = { 0.875f,  -1.f,	 0.f,		0.875f,  1.f,		0.875f,  1.f };

	//pVertex[21] = { 1.f,     -1.f,	 0.f,		1.f,     1.f,		1.f,     1.f };

	//pVertex[22] = { 1.f,	 -0.875f, 0.f,		1.f,	 0.875f,    1.f,	 0.875f };
	//pVertex[23] = { 1.f,	 -0.75f,  0.f,		1.f,	 0.75f,     1.f,	 0.75f };
	//pVertex[24] = { 1.f,	 -0.625f, 0.f,		1.f,	 0.625f,    1.f,	 0.625f };
	//pVertex[25] = { 1.f,	 -0.5f,   0.f,		1.f,	 0.5f,      1.f,	 0.5f };
	//pVertex[26] = { 1.f,	 -0.375f, 0.f,		1.f,	 0.375f,    1.f,	 0.375f };
	//pVertex[27] = { 1.f,	 -0.25f,  0.f,		1.f,	 0.25f,     1.f,	 0.25f };
	//pVertex[28] = { 1.f,	 -0.125f, 0.f,		1.f,	 0.125f,    1.f,	 0.125f };

	//pVertex[29] = { 1.f,	  0.f,    0.f,     1.f,		 0.f,		1.f,     0.f };
	//pVertex[30] = { 0.875f,   0.f,	  0.f,	   0.875f,   0.f,		0.875f,  0.f };
	//pVertex[31] = { 0.75f,    0.f,    0.f,	   0.75f,    0.f,		0.75f,   0.f };
	//pVertex[32] = { 0.625f,   0.f,	  0.f,	   0.625f,   0.f,		0.625f,  0.f };
	//pVertex[33] = { 0.5f,     0.f,	  0.f,	   0.5f,     0.f,		0.5f,    0.f };

	//m_pVB->Unlock();

	return S_OK;
}

HRESULT CUltimateTex::Ready_Buffer()
{
	return S_OK;
}

HRESULT CUltimateTex::Render_Buffer()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	//m_pGraphicDev->SetTexture(0, pTexIcon);
	//m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(FVF_ULTIMATE));	// 버텍스 버퍼를 디바이스 장치와 링크하는 함수
	//m_pGraphicDev->SetFVF(FVF_TEX);
	//m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(VTXTEX));



	//m_pGraphicDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphicDev->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	//m_pGraphicDev->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//m_pGraphicDev->SetTexture(1, pTexCooldown);
	//m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 32, m_vCooldown, sizeof(ULTIMATETEX));


	return S_OK;
}

Engine::CUltimateTex* Engine::CUltimateTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUltimateTex*	pInstance = new CUltimateTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype()))
		Safe_Release(pInstance);


	return pInstance;
}

CComponent * CUltimateTex::Clone()
{
	return new CUltimateTex(*this);
}


void Engine::CUltimateTex::Free()
{
	CUltimateBuffer::Free();
}

