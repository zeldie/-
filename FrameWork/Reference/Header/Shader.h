#ifndef Shader_h__
#define Shader_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();


public:
	LPD3DXEFFECT		Get_EffectHandle() { return m_pEffect; }

public:
	HRESULT				Ready_Prototype(wstring wstrFilePath);

private:
	LPD3DXEFFECT				m_pEffect;
	LPD3DXBUFFER				m_pErrMsg;

public:
	static	CShader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFilePath);
	virtual CComponent*		Clone();
	virtual void			Free();
};

END
#endif // Shader_h__
