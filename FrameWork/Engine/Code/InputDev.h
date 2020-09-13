#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev final : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev();

public:
	void	Set_InputDev();

	_int	KeyPressing(_ubyte byKeyID);
	_int	KeyDown(_ubyte byKeyID);
	_int	KeyUp(_ubyte byKeyID);
	_byte	MousePressing(MOUSEKEYSTATE eMouse);
	_long	MouseMove(MOUSEMOVESTATE eMouseState);
	_int	MouseDown(MOUSEKEYSTATE eMouse);
	_int	MouseUp(MOUSEKEYSTATE eMouse);

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	
private:
	LPDIRECTINPUT8			m_pInputSDK;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	LPDIRECTINPUTDEVICE8	m_pMouse;
	_byte					m_byKeyState[256];
	_byte					m_byOldKeyState[256];
	DIMOUSESTATE			m_tMouseState;
	DIMOUSESTATE			m_tOldMouseState;
public:
	virtual void	Free();

};
END
#endif // InputDev_h__
