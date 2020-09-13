#include "Mesh.h"

USING(Engine)

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CMesh::CMesh(const CMesh & rhs)
	:CComponent(rhs)
{
}

CMesh::~CMesh()
{
}

void CMesh::Free()
{
	CComponent::Free();
}
