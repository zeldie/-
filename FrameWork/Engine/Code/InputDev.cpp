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
	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL)))
		return E_FAIL;

	// 키보드 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;
	//FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);zz

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;
	//FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
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

