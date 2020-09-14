// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <math.h>
#include <ctime>
#include <time.h>

#include <process.h> // _beginthreadex �Լ��� ȣ���ϱ� ���ؼ� �ݵ�� �����ؾ� ��

//#include <io.h>
//#include "fmod.h"
//#pragma comment(lib, "fmodex_vc.lib")

#include "Base.h"
#include "Export_Funtion.h"

#include "Client_Defines.h"
//#include "SoundMgr.h"
#include "EffectMgr.h"
#include "UIMgr.h"
#include "AiMgr.h"
#include "CollisionMgr.h"
#include "TextureMgr.h"
#include "CameraMgr.h"
#include "MatchingMgr.h"
#include "PuzzleMgr.h"
#include "LoadingMgr.h"
#include "RunGameMgr.h"
#include "QuestMgr.h"
#include "SoundMgr.h"

#define	DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

using namespace std;
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

//#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")

//#define LOADINGTHREAD