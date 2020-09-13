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
	
//	m_pEffect ���� �ְ�, m_pErrMsg ���� ���� ��� : ���������� ���̴� ������ �ҷ��� ���

//	m_pEffect ���� ����, m_pErrMsg ���� �ִ� ��� : �ε� ����
//	m_pEffect ���� �ְ�, m_pErrMsg ���� �ִ� ��� : �ε� ��� ����
	

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,
										wstrFilePath.c_str(),
										NULL,		// ���̴����� ����� ��ũ�� ����
										NULL,		// ���̴����� ����� ���� ����
										D3DXSHADER_DEBUG,	// ���̴� ����� �ɼ�(������ �ܺζ��̺귯���� ���ؼ��� ����װ� ����, Ȥ�ö� ����׸� �������� ������� ���� �ɼ� ��)
										NULL,		// ���� ���ҽ� ������ ��� ������ ���ΰ�(null�� �־��� �⺻������ �׷��� �޸𸮿� ������)
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

