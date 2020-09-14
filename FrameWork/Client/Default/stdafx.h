// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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

#include <process.h> // _beginthreadex 함수를 호출하기 위해선 반드시 존재해야 함

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