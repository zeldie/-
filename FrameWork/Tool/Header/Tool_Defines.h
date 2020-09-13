#ifndef Defines_h__
#define Defines_h__

using namespace std;

#include "Tool_Typedef.h"
#include "Tool_Macro.h"
#include "Tool_Enum.h"

class CToolView;
extern CToolView*		g_pToolView;
	
class CMyForm;
extern CMyForm*			g_pMyForm;

class CMapTool;
extern CMapTool*		g_pMapTool;

class CMeshTool;
extern CMeshTool*		g_pMeshTool;

class CNavigationTool;
extern CNavigationTool*	g_pNavigationTool;

class CPathFindTool;
extern CPathFindTool*	g_pPathFindTool;

class CEffectTool;
extern CEffectTool*		g_pEffectTool;

class CMapScene;
extern CMapScene*		g_pMapScene;

class CCameraTool;
extern CCameraTool*		g_pCamTool;

class CRunTool;
extern CRunTool*		g_pRunTool;

extern HWND				g_hWnd;
extern HINSTANCE		g_hInst;

//¿Ã∆Â∆Æ ≈¯
extern double			g_dTimeBegin;
extern double			g_dTimeEnd;
extern bool				g_bIsPlay;

#endif // Defines_h__
