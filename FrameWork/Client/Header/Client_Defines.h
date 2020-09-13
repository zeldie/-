#ifndef Defines_h__
#define Defines_h__

using namespace std;

#include "Client_Typedef.h"
#include "Client_Macro.h"
#include "Client_Extern.h"
#include "Client_Enum.h"
#include "Client_Struct.h"

extern	HWND		g_hWnd;
extern HINSTANCE	g_hInst;

class CMyStrCmp
{
public:
	CMyStrCmp() {}
	CMyStrCmp(const TCHAR* pString)
		:m_pString(pString)
	{}
	~CMyStrCmp() {}

	template<typename T>
	bool operator()(T& rPair)
	{
		return !lstrcmp(rPair.first, m_pString);
	}
private:
	const TCHAR* m_pString;
};
#endif // Defines_h__
