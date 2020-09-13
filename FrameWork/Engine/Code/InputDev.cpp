#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev()
	:m_pInputSDK(nullptr),
	m_pKeyBoard(nullptr),
	m_pMouse(nullptr)
{

}

Engine::CInputDev::~CInputDev()
{
	Free();
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DInput �İ�ü�� �����ϴ� �Լ�
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL)))
		return E_FAIL;

	// Ű���� ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;
	//FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);zz

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;
	//FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInputDev::Set_InputDev()
{
	//OldKey Backup
	memcpy_s(m_byOldKeyState, sizeof(_byte) * 256, m_byKeyState, sizeof(_byte) * 256);
	memcpy_s(&m_tOldMouseState, sizeof(DIMOUSESTATE), &m_tMouseState, sizeof(DIMOUSESTATE));
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

}

_int Engine::CInputDev::KeyPressing(_ubyte byKeyID)
{
	return m_byKeyState[byKeyID] & 0x80;
}

_int Engine::CInputDev::KeyDown(_ubyte byKeyID)
{
	if (!(m_byOldKeyState[byKeyID] & 0x80) && m_byKeyState[byKeyID])
		return 0x80;

	return 0x00;
}

_int Engine::CInputDev::KeyUp(_ubyte byKeyID)
{
	if (m_byOldKeyState[byKeyID] && !(m_byKeyState[byKeyID] & 0x80))
		return 0x80;

	return 0x00;
}

_byte Engine::CInputDev::MousePressing(MOUSEKEYSTATE eMouse)
{
	return m_tMouseState.rgbButtons[eMouse];
}

_long Engine::CInputDev::MouseMove(MOUSEMOVESTATE eMouseState)
{
	return *(((_long*)&m_tMouseState) + eMouseState);
}

_int Engine::CInputDev::MouseDown(MOUSEKEYSTATE eMouse)
{
	if (!(m_tOldMouseState.rgbButtons[eMouse] & 0x80) && m_tMouseState.rgbButtons[eMouse])
		return 0x80;

	return 0x00;
}

_int Engine::CInputDev::MouseUp(MOUSEKEYSTATE eMouse)
{
	if (m_tOldMouseState.rgbButtons[eMouse] && !(m_tMouseState.rgbButtons[eMouse] & 0x80))
		return 0x80;

	return 0x00;
}

void Engine::CInputDev::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

