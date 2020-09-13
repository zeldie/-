#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	 m_pEffect(nullptr),
	 m_pErrMsg(nullptr)
{

}

Engine::CShader::CShader(const CShader& rhs)
	:CComponent(rhs),
	m_pEffect(rhs.m_pEffect),
	m_pErrMsg(rhs.m_pErrMsg)
{
	Safe_AddRef(m_pEffect);
}

Engine::CShader::~CShader()
{

}

HRESULT Engine::CShader::Ready_Prototype(wstring wstrFilePath)
{
	
//	m_pEffect 값이 있고, m_pErrMsg 값이 없는 경우 : 정상적으로 쉐이더 파일이 불러진 경우

//	m_pEffect 값이 없고, m_pErrMsg 값이 있는 경우 : 로드 에러
//	m_pEffect 값이 있고, m_pErrMsg 값도 있는 경우 : 로드 경고 상태
	

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,
										wstrFilePath.c_str(),
										NULL,		// 쉐이더에서 사용할 매크로 파일
										NULL,		// 쉐이더에서 사용할 포함 파일
										D3DXSHADER_DEBUG,	// 쉐이더 디버그 옵션(원래는 외부라이브러리를 통해서만 디버그가 가능, 혹시라도 디버그를 지원하지 않을까봐 넣은 옵션 값)
										NULL,		// 각종 리소스 정보를 어디에 저장할 것인가(null을 넣어줄 기본값으로 그래픽 메모리에 저장함)
										&m_pEffect,
										&m_pErrMsg)))
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_warning", MB_OK);
	}

	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Prototype(wstrFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CShader::Clone()
{
	return new CShader(*this);
}

void Engine::CShader::Free()
{
	Safe_Release(m_pEffect);
	CComponent::Free();
}

