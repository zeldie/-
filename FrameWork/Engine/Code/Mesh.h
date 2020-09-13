#pragma once
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CMesh : public CComponent
{
public:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();


public:
	virtual void Free();
};

END

