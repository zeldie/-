#include "ScreenTex.h"

USING(Engine)

Engine::CScreenTex::CScreenTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CScreenTex::CScreenTex(const CScreenTex& rhs)
	: CVIBuffer(rhs)
{

}

CScreenTex::~CScreenTex()
{
}

HRESULT Engine::CScreenTex::Ready_Prototype(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY, _float fViewZ)
{
	m_dwVtxCnt = 4;
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXSCREEN);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	if (FAILED(CVIBuffer::Ready_Prototype()))
		return E_FAIL;

	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(fStartX, fStartY, fViewZ, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(fStartX + fSizeX, fStartY, fViewZ, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(fStartX + fSizeX, fStartY + fSizeY, fViewZ, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(fStartX, fStartY + fSizeY, fViewZ, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	m_vStartPos = _vec3(fStartX, fStartY, fViewZ);
	m_vSize = _vec2(fSizeX, fSizeY);

	return S_OK;
}

HRESULT CScreenTex::Ready_Buffer()
{
	return S_OK;
}

void CScreenTex::VertexXControl_LeftDir(_float fSizeX)
{
	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z, 1.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x * fSizeX, m_vStartPos.y, m_vStartPos.z, 1.f);//
	pVertex[1].vTexUV = _vec2(fSizeX, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x * fSizeX, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);//
	pVertex[2].vTexUV = _vec2(fSizeX, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

void CScreenTex::VertexXControl_RightDir(_float fSizeX)
{
	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x + m_vSize.x * fSizeX, m_vStartPos.y, m_vStartPos.z, 1.f);//
	pVertex[0].vTexUV = _vec2(fSizeX, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x + m_vSize.x * fSizeX, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);//
	pVertex[3].vTexUV = _vec2(fSizeX, 1.f);

	m_pVB->Unlock();

}

void CScreenTex::VertexYControl_UpDir(_float fSizeY)
{
	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y * fSizeY, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, fSizeY);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y * fSizeY, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, fSizeY);

	m_pVB->Unlock();
}

void CScreenTex::VertexYControl_DownDir(_float fSizeY)
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y * fSizeY, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, fSizeY);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y * fSizeY, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, fSizeY);

	m_pVB->Unlock();
}

void CScreenTex::VertexXYControl(_float fSizeX, _float fSizeY)
{
	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_vec2 vSize = _vec2(m_vSize.x* (fSizeX - 1.f)*0.5f, m_vSize.y* (fSizeY - 1.f)*0.5f);

	pVertex[0].vPos = _vec4(m_vStartPos.x - vSize.x, m_vStartPos.y - vSize.y, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x + vSize.x, m_vStartPos.y - vSize.y, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x + vSize.x, m_vStartPos.y + m_vSize.y + vSize.y, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x - vSize.x, m_vStartPos.y + m_vSize.y + vSize.y, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

}

void CScreenTex::VertexOriginControl()
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

void CScreenTex::VertexPositionXControl(_float fPos)
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x + fPos, m_vStartPos.y , m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x + fPos, m_vStartPos.y , m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x + fPos, m_vStartPos.y + m_vSize.y , m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x + fPos, m_vStartPos.y + m_vSize.y , m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

void CScreenTex::VertexPositionYControl(_float fPos)
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + fPos, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + fPos, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y + fPos, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y + fPos, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}


void CScreenTex::VertexPositionXControl2(_float fPos)
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	m_vStartPos.x += fPos;

	pVertex[0].vPos = _vec4(m_vStartPos.x , m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x , m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x , m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x , m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

void CScreenTex::VertexPositionYControl2(_float fPos)
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	m_vStartPos.y += fPos;

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y , m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y , m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y , m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y , m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

void CScreenTex::VertexPositionXYControl2(_vec2 vPos)
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	m_vStartPos.x = vPos.x;
	m_vStartPos.y = vPos.y;

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

}

void CScreenTex::VertexZControl()
{
	_float fView = 0.f;

	if (0.1f <= m_vStartPos.z)
	{
		fView = 0.f;
	}
	else if (0.f <= m_vStartPos.z)
	{
		fView = 0.1f;
	}

	m_vStartPos.z = fView;

	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

void CScreenTex::Resize_Buffer(_float fX, _float fY)
{
	VTXSCREEN*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	m_pVB->Unlock();
}

_bool CScreenTex::CheckAroundMouse(HWND hWnd, _vec3 * pMousePos /*= nullptr*/, _vec3 vSizeRatio/* = _vec3(0.f, 0.f, 0.f)*/)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	*pMousePos = _vec3((_float)ptMouse.x, (_float)ptMouse.y, 0.f);

	_vec3 vVertex[4];
	vVertex[0] = _vec3(m_vStartPos.x - vSizeRatio.x, m_vStartPos.y - vSizeRatio.y, 0.f);
	vVertex[1] = _vec3(m_vStartPos.x + m_vSize.x + +vSizeRatio.x, m_vStartPos.y - vSizeRatio.y, 0.f);
	vVertex[2] = _vec3(m_vStartPos.x + m_vSize.x + vSizeRatio.x, m_vStartPos.y + m_vSize.y + vSizeRatio.y, 0.f);
	vVertex[3] = _vec3(m_vStartPos.x - vSizeRatio.x, m_vStartPos.y + m_vSize.y + vSizeRatio.y, 0.f);

	_vec2 vMouseDir[4] = {
		*pMousePos - vVertex[0],
		*pMousePos - vVertex[1],
		*pMousePos - vVertex[2],
		*pMousePos - vVertex[3]
	};

	_vec2 vLine[4] = {
		vVertex[1] - vVertex[0],
		vVertex[2] - vVertex[1],
		vVertex[3] - vVertex[2],
		vVertex[0] - vVertex[3]
	};

	_vec2 vNormal[4] = {};
	for (_uint i = 0; i < 4; ++i)
	{
		vNormal[i] = _vec2(-vLine[i].y, vLine[i].x);

		D3DXVec2Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec2Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (_uint i = 0; i < 4; ++i)
	{
		if (0 > D3DXVec2Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;

}

void CScreenTex::VertexCustomizeTest(const _double& dTimeDelta, _float fSpeed, CUSTOMIZE CustomizeType)
{
	if (POSX == CustomizeType)
	{
		m_vStartPos.x += (_float)dTimeDelta *fSpeed;
	}
	else if (POSY == CustomizeType)
	{
		m_vStartPos.y += (_float)dTimeDelta *fSpeed;
	}
	else if (SIZEX == CustomizeType)
	{
		m_vSize.x += (_float)dTimeDelta *fSpeed;
	}
	else if (SIZEY == CustomizeType)
	{
		m_vSize.y += (_float)dTimeDelta *fSpeed;
	}
	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y, m_vStartPos.z, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(m_vStartPos.x + m_vSize.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(m_vStartPos.x, m_vStartPos.y + m_vSize.y, m_vStartPos.z, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

Engine::CScreenTex* Engine::CScreenTex::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY, _float fViewZ/*=0.f*/)
{
	CScreenTex*	pInstance = new CScreenTex(pGraphic_Device);

	if (FAILED(pInstance->Ready_Prototype(fStartX, fStartY, fSizeX, fSizeY, fViewZ)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_ViewPort");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CScreenTex::Clone()
{
	return new CScreenTex(*this);
}


void Engine::CScreenTex::Free()
{
	CVIBuffer::Free();
}

