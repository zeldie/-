#include "stdafx.h"
#include "PuzzleMgr.h"

IMPLEMENT_SINGLETON(CPuzzleMgr)

CPuzzleMgr::CPuzzleMgr()
	: m_pGraphicDev(nullptr)
	, m_iNull_Index(24)
	, m_bInit(false)
	, m_bStart(false)
	, m_pPreview(nullptr)
	, m_pSuccessView(nullptr)
	, m_bSuccess(false)
	, m_dSuccessDelay(0.)
	, m_ePuzzleType(PUZZLE_END)
	, m_bChangeScene(false)
{
}

CPuzzleMgr::~CPuzzleMgr()
{
	CUIMgr::GetInstance()->EraseMouse();
	Engine::Safe_Release(m_pGraphicDev);
}

HRESULT CPuzzleMgr::Set_Device(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (pGraphicDev == nullptr)
		return E_FAIL;

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	return S_OK;
}

void CPuzzleMgr::Update_PuzzleMgr(const _double & dTimeDelta)
{
	LateInit();
	if (m_bSuccess)
	{
		if (m_ePuzzleType == PUZZLE_APOSTLE)
			Success_Update_Apostle(dTimeDelta);
		if (m_ePuzzleType == PUZZLE_BELATOS)
			Success_Update_Belatos(dTimeDelta);
	}

	if (m_bStart)
	{
		if (Success_Check())
		{
			// 클리어
			if (m_ePuzzleType == PUZZLE_APOSTLE)
				m_vecPuzzleObj[m_iNull_Index]->Set_TextureNum(25);  // 디졸브 넣어서 천천히 보이게
			if (m_ePuzzleType == PUZZLE_BELATOS)
				m_vecPuzzleObj[m_iNull_Index]->Set_TextureNum(16);  // 디졸브 넣어서 천천히 보이게
			
			// 클리어 함수 호출해서, 퍼즐들 움직인다->
			// 성공했다는 메세지 나오고 잠시 후 스테이지로 진입

			m_bSuccess = true;
			m_dSuccessDelay = g_dAccumulatedTime + 1.f;
			////cout << "성공" << endl;
		}
		else
		{
			////cout << "미완성" << endl;
		}
	}

	//if (Engine::KeyDown(DIK_6))
	//{
	//	for (auto& iter : m_vecPuzzleObj)
	//	{
	//		iter->Get_ScreenTex()->VertexPositionXControl2(1.f);
	//	}
	//}
	//if (Engine::KeyDown(DIK_7))
	//{
	//	for (auto& iter : m_vecPuzzleObj)
	//	{
	//		iter->Get_ScreenTex()->VertexPositionXControl2(-1.f);
	//	}
	//}
	//if (Engine::KeyDown(DIK_8))
	//{
	//	for (auto& iter : m_vecPuzzleObj)
	//	{
	//		iter->Get_ScreenTex()->VertexPositionYControl2(1.f);
	//	}
	//}
	//if (Engine::KeyDown(DIK_9))
	//{
	//	for (auto& iter : m_vecPuzzleObj)
	//	{
	//		iter->Get_ScreenTex()->VertexPositionYControl2(-1.f);
	//	}
	//}

	//if (Engine::KeyDown(DIK_5))
	//{
	//	//cout << "========================================================" << endl;
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			//cout << (i * 4) + j << "번 : " << m_vecPuzzleObj[(i * 4) + j]->Get_TextureNum() << "   ";
	//		}
	//		//cout << endl;
	//	}
	//	//cout << "========================================================" << endl;
	//}
}

void CPuzzleMgr::Create_Puzzle_Apostle()
{
	Delete_Puzzle();
	m_vecPuzzleObj.shrink_to_fit();
	m_vecPuzzleObj.reserve(25);

	CPuzzleObj* pPuzzleObj = nullptr;

	// 퍼즐
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			pPuzzleObj = CPuzzleObj::Create(m_pGraphicDev, 675.f + 105.f * j, 97.5f + 105.f * i, 100.f, 100.f, 0.1f, L"Texture_Puzzle_Apostle");
			Engine::Add_GameObject(Engine::UI, L"PuzzleObj_Apostle", pPuzzleObj);
			pPuzzleObj->Set_Index((5 * i) + j);
			m_vecPuzzleObj.push_back(pPuzzleObj);
		}
	}
	int i = 0;
	for (auto& iter : m_vecPuzzleObj)
	{
		iter->Set_TextureNum(i++);
	}
	m_iNull_Index = 24;

	Reset_Puzzle_Apostle();

	// 미리보기
	m_pPreview = CPuzzlePreview::Create(m_pGraphicDev, 80.f, 110.f, 500.f, 500.f, 0.1f, L"Texture_PuzzlePreview_Apostle");
	Engine::Add_GameObject(Engine::UI, L"PuzzlePreview_Apostle", m_pPreview);
	m_bStart = true;
	m_ePuzzleType = PUZZLE_APOSTLE;
}

void CPuzzleMgr::Create_Puzzle_Belatos()
{
	Delete_Puzzle();
	m_vecPuzzleObj.shrink_to_fit();
	m_vecPuzzleObj.reserve(16);

	CPuzzleObj* pPuzzleObj = nullptr;

	// 퍼즐
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pPuzzleObj = CPuzzleObj::Create(m_pGraphicDev, 675.f + 130.f * j, 102.5f + 130.f * i, 125.f, 125.f, 0.1f, L"Texture_Puzzle_Belatos");
			Engine::Add_GameObject(Engine::UI, L"PuzzleObj_Belatos", pPuzzleObj);
			pPuzzleObj->Set_Index((4 * i) + j);
			m_vecPuzzleObj.push_back(pPuzzleObj);
		}
	}
	int i = 0;
	for (auto& iter : m_vecPuzzleObj)
	{
		iter->Set_TextureNum(i++);
	}
	m_iNull_Index = 15;

	Reset_Puzzle_Belatos();

	// 미리보기
	m_pPreview = CPuzzlePreview::Create(m_pGraphicDev, 80.f, 110.f, 500.f, 500.f, 0.1f, L"Texture_PuzzlePreview_Belatos");
	Engine::Add_GameObject(Engine::UI, L"PuzzlePreview_Belatos", m_pPreview);
	m_bStart = true;
	m_ePuzzleType = PUZZLE_BELATOS;
}

void CPuzzleMgr::Reset_Puzzle_Apostle()
{
	int i = 0;
	m_iNull_Index = 1;
	m_vecPuzzleObj[i++]->Set_TextureNum(2);
	m_vecPuzzleObj[i++]->Set_TextureNum(24);
	m_vecPuzzleObj[i++]->Set_TextureNum(3);
	m_vecPuzzleObj[i++]->Set_TextureNum(4);
	m_vecPuzzleObj[i++]->Set_TextureNum(9);

	m_vecPuzzleObj[i++]->Set_TextureNum(0);
	m_vecPuzzleObj[i++]->Set_TextureNum(1);
	m_vecPuzzleObj[i++]->Set_TextureNum(7);
	m_vecPuzzleObj[i++]->Set_TextureNum(8);
	m_vecPuzzleObj[i++]->Set_TextureNum(14);

	m_vecPuzzleObj[i++]->Set_TextureNum(5);
	m_vecPuzzleObj[i++]->Set_TextureNum(10);
	m_vecPuzzleObj[i++]->Set_TextureNum(6);
	m_vecPuzzleObj[i++]->Set_TextureNum(11);
	m_vecPuzzleObj[i++]->Set_TextureNum(12);

	m_vecPuzzleObj[i++]->Set_TextureNum(15);
	m_vecPuzzleObj[i++]->Set_TextureNum(16);
	m_vecPuzzleObj[i++]->Set_TextureNum(17);
	m_vecPuzzleObj[i++]->Set_TextureNum(13);
	m_vecPuzzleObj[i++]->Set_TextureNum(19);

	m_vecPuzzleObj[i++]->Set_TextureNum(20);
	m_vecPuzzleObj[i++]->Set_TextureNum(21);
	m_vecPuzzleObj[i++]->Set_TextureNum(22);
	m_vecPuzzleObj[i++]->Set_TextureNum(18);
	m_vecPuzzleObj[i++]->Set_TextureNum(23);
	// 0,5,10,11,12,13,14,9,4,3,2,1,6,11,12,13,18,23,24
}

void CPuzzleMgr::Reset_Puzzle_Belatos()
{
	int i = 0;
	m_iNull_Index = 3;
	m_vecPuzzleObj[i++]->Set_TextureNum(5);
	m_vecPuzzleObj[i++]->Set_TextureNum(0);
	m_vecPuzzleObj[i++]->Set_TextureNum(3);
	m_vecPuzzleObj[i++]->Set_TextureNum(15);

	m_vecPuzzleObj[i++]->Set_TextureNum(4);
	m_vecPuzzleObj[i++]->Set_TextureNum(1);
	m_vecPuzzleObj[i++]->Set_TextureNum(6);
	m_vecPuzzleObj[i++]->Set_TextureNum(7);

	m_vecPuzzleObj[i++]->Set_TextureNum(12);
	m_vecPuzzleObj[i++]->Set_TextureNum(8);
	m_vecPuzzleObj[i++]->Set_TextureNum(10);
	m_vecPuzzleObj[i++]->Set_TextureNum(2);

	m_vecPuzzleObj[i++]->Set_TextureNum(9);
	m_vecPuzzleObj[i++]->Set_TextureNum(13);
	m_vecPuzzleObj[i++]->Set_TextureNum(14);
	m_vecPuzzleObj[i++]->Set_TextureNum(11);
	// 7,11,15,14,13,12,8,4,0,1,5,6,7,3,2,6,5,4,8,9,13,14  
}

void CPuzzleMgr::Delete_Puzzle()
{
	//for (auto& iter : m_vecPuzzleObj)
	//{
	//	iter->Set_Dead();
	//}
	//m_vecPuzzleObj.clear();

	//if (m_pPreview != nullptr)
	//{
	//	m_pPreview->Set_Dead();
	//	m_pPreview = nullptr;
	//}
	//if (m_pSuccessView != nullptr)
	//{
	//	m_pSuccessView->Set_Dead();
	//	m_pSuccessView = nullptr;
	//}
	m_vecPuzzleObj.clear();
	m_pPreview = nullptr;
	m_pSuccessView = nullptr;
	m_bStart = false;
}

void CPuzzleMgr::Clicked_Puzzle(CPuzzleObj * pPuzzleObj)
{
	_uint iIndex = pPuzzleObj->Get_Index();	// 매니저의 벡터 인덱스

	if (m_ePuzzleType == PUZZLE_APOSTLE)
	{
		switch (iIndex)
		{
		case 0:
		{
			if ((m_iNull_Index == 1) || (m_iNull_Index == 5))
				Change_Puzzle(iIndex);
			break;
		}
		case 1:
		{
			if ((m_iNull_Index == 0) || (m_iNull_Index == 2) || (m_iNull_Index == 6))
				Change_Puzzle(iIndex);
			break;
		}
		case 2:
		{
			if ((m_iNull_Index == 1) || (m_iNull_Index == 7) || (m_iNull_Index == 3))
				Change_Puzzle(iIndex);
			break;
		}
		case 3:
		{
			if ((m_iNull_Index == 2) || (m_iNull_Index == 4) || (m_iNull_Index == 8))
				Change_Puzzle(iIndex);
			break;
		}
		case 4:
		{
			if ((m_iNull_Index == 3) || (m_iNull_Index == 9))
				Change_Puzzle(iIndex);
			break;
		}
		case 5:
		{
			if ((m_iNull_Index == 0) || (m_iNull_Index == 6) || (m_iNull_Index == 10))
				Change_Puzzle(iIndex);
			break;
		}
		case 6:
		{
			if ((m_iNull_Index == 1) || (m_iNull_Index == 5) || (m_iNull_Index == 7) || (m_iNull_Index == 11))
				Change_Puzzle(iIndex);
			break;
		}
		case 7:
		{
			if ((m_iNull_Index == 2) || (m_iNull_Index == 6) || (m_iNull_Index == 8) || (m_iNull_Index == 12))
				Change_Puzzle(iIndex);
			break;
		}
		case 8:
		{
			if ((m_iNull_Index == 3) || (m_iNull_Index == 7) || (m_iNull_Index == 9) || (m_iNull_Index == 13))
				Change_Puzzle(iIndex);
			break;
		}
		case 9:
		{
			if ((m_iNull_Index == 4) || (m_iNull_Index == 8) || (m_iNull_Index == 14))
				Change_Puzzle(iIndex);
			break;
		}
		case 10:
		{
			if ((m_iNull_Index == 5) || (m_iNull_Index == 11) || (m_iNull_Index == 15))
				Change_Puzzle(iIndex);
			break;
		}
		case 11:
		{
			if ((m_iNull_Index == 6) || (m_iNull_Index == 10) || (m_iNull_Index == 12) || (m_iNull_Index == 16))
				Change_Puzzle(iIndex);
			break;
		}
		case 12:
		{
			if ((m_iNull_Index == 7) || (m_iNull_Index == 11) || (m_iNull_Index == 13) || (m_iNull_Index == 17))
				Change_Puzzle(iIndex);
			break;
		}
		case 13:
		{
			if ((m_iNull_Index == 8) || (m_iNull_Index == 12) || (m_iNull_Index == 14) || (m_iNull_Index == 18))
				Change_Puzzle(iIndex);
			break;
		}
		case 14:
		{
			if ((m_iNull_Index == 9) || (m_iNull_Index == 13) || (m_iNull_Index == 19))
				Change_Puzzle(iIndex);
			break;
		}
		case 15:
		{
			if ((m_iNull_Index == 10) || (m_iNull_Index == 16) || (m_iNull_Index == 20))
				Change_Puzzle(iIndex);
			break;
		}
		case 16:
		{
			if ((m_iNull_Index == 11) || (m_iNull_Index == 15) || (m_iNull_Index == 17) || (m_iNull_Index == 21))
				Change_Puzzle(iIndex);
			break;
		}
		case 17:
		{
			if ((m_iNull_Index == 12) || (m_iNull_Index == 16) || (m_iNull_Index == 18) || (m_iNull_Index == 22))
				Change_Puzzle(iIndex);
			break;
		}
		case 18:
		{
			if ((m_iNull_Index == 13) || (m_iNull_Index == 17) || (m_iNull_Index == 19) || (m_iNull_Index == 23))
				Change_Puzzle(iIndex);
			break;
		}
		case 19:
		{
			if ((m_iNull_Index == 14) || (m_iNull_Index == 18) || (m_iNull_Index == 24))
				Change_Puzzle(iIndex);
			break;
		}
		case 20:
		{
			if ((m_iNull_Index == 15) || (m_iNull_Index == 21))
				Change_Puzzle(iIndex);
			break;
		}
		case 21:
		{
			if ((m_iNull_Index == 16) || (m_iNull_Index == 20) || (m_iNull_Index == 22))
				Change_Puzzle(iIndex);
			break;
		}
		case 22:
		{
			if ((m_iNull_Index == 17) || (m_iNull_Index == 21) || (m_iNull_Index == 23))
				Change_Puzzle(iIndex);
			break;
		}
		case 23:
		{
			if ((m_iNull_Index == 18) || (m_iNull_Index == 22) || (m_iNull_Index == 24))
				Change_Puzzle(iIndex);
			break;
		}
		case 24:
		{
			if ((m_iNull_Index == 19) || (m_iNull_Index == 23))
				Change_Puzzle(iIndex);
			break;
		}
		default:
			break;
		}
	}
	else if (m_ePuzzleType == PUZZLE_BELATOS)
	{
		switch (iIndex)
		{
		case 0:
		{
			if ((m_iNull_Index == 1) || (m_iNull_Index == 4))
				Change_Puzzle(iIndex);
			break;
		}
		case 1:
		{
			if ((m_iNull_Index == 0) || (m_iNull_Index == 2) || (m_iNull_Index == 5))
				Change_Puzzle(iIndex);
			break;
		}
		case 2:
		{
			if ((m_iNull_Index == 1) || (m_iNull_Index == 6) || (m_iNull_Index == 3))
				Change_Puzzle(iIndex);
			break;
		}
		case 3:
		{
			if ((m_iNull_Index == 2) || (m_iNull_Index == 7))
				Change_Puzzle(iIndex);
			break;
		}
		case 4:
		{
			if ((m_iNull_Index == 0) || (m_iNull_Index == 5) || (m_iNull_Index == 8))
				Change_Puzzle(iIndex);
			break;
		}
		case 5:
		{
			if ((m_iNull_Index == 1) || (m_iNull_Index == 4) || (m_iNull_Index == 6) || (m_iNull_Index == 9))
				Change_Puzzle(iIndex);
			break;
		}
		case 6:
		{
			if ((m_iNull_Index == 2) || (m_iNull_Index == 5) || (m_iNull_Index == 7) || (m_iNull_Index == 10))
				Change_Puzzle(iIndex);
			break;
		}
		case 7:
		{
			if ((m_iNull_Index == 3) || (m_iNull_Index == 6) || (m_iNull_Index == 11))
				Change_Puzzle(iIndex);
			break;
		}
		case 8:
		{
			if ((m_iNull_Index == 4) || (m_iNull_Index == 9) || (m_iNull_Index == 12))
				Change_Puzzle(iIndex);
			break;
		}
		case 9:
		{
			if ((m_iNull_Index == 5) || (m_iNull_Index == 8) || (m_iNull_Index == 10) || (m_iNull_Index == 13))
				Change_Puzzle(iIndex);
			break;
		}
		case 10:
		{
			if ((m_iNull_Index == 6) || (m_iNull_Index == 9) || (m_iNull_Index == 11) || (m_iNull_Index == 14))
				Change_Puzzle(iIndex);
			break;
		}
		case 11:
		{
			if ((m_iNull_Index == 7) || (m_iNull_Index == 10) || (m_iNull_Index == 15))
				Change_Puzzle(iIndex);
			break;
		}
		case 12:
		{
			if ((m_iNull_Index == 8) || (m_iNull_Index == 13))
				Change_Puzzle(iIndex);
			break;
		}
		case 13:
		{
			if ((m_iNull_Index == 9) || (m_iNull_Index == 12) || (m_iNull_Index == 14))
				Change_Puzzle(iIndex);
			break;
		}
		case 14:
		{
			if ((m_iNull_Index == 10) || (m_iNull_Index == 13) || (m_iNull_Index == 15))
				Change_Puzzle(iIndex);
			break;
		}
		case 15:
		{
			if ((m_iNull_Index == 11) || (m_iNull_Index == 14))
				Change_Puzzle(iIndex);
			break;
		}
		default:
			break;
		}
	}

}

void CPuzzleMgr::LateInit()
{
	if (m_bInit)
		return;
	m_bInit = true;

	//Engine::AddSubject(Engine::MATCHING_GMAE);
	CUIMgr::GetInstance()->CreateMouse(m_pGraphicDev, CMouse::MOUSE_PICKING);
}

void CPuzzleMgr::Change_Puzzle(_uint iIndex)
{
	_uint	iTexnum;
	iTexnum = m_vecPuzzleObj[iIndex]->Get_TextureNum();
	m_vecPuzzleObj[iIndex]->Set_TextureNum(m_vecPuzzleObj[m_iNull_Index]->Get_TextureNum());
	m_vecPuzzleObj[m_iNull_Index]->Set_TextureNum(iTexnum);
	m_iNull_Index = iIndex;
}

_bool CPuzzleMgr::Success_Check()
{
	for (auto& iter : m_vecPuzzleObj)
	{
		if (iter->Get_TextureNum() != iter->Get_Index())
			return false;
	}
	return true;
}

void CPuzzleMgr::Success_Update_Apostle(const _double& dTimeDelta)
{
	if (m_dSuccessDelay <= g_dAccumulatedTime)
	{
		m_pSuccessView = CPuzzlePreview::Create(m_pGraphicDev, 685.f, 107.5f, 500.f, 500.f, 0.05f, L"Texture_PuzzlePreview_Apostle");
		Engine::Add_GameObject(Engine::UI, L"PuzzlePreview_Apostle", m_pSuccessView);
		m_bChangeScene = true;
		m_bSuccess = false;
		return;
	}

	m_vecPuzzleObj[0]->Get_ScreenTex()->VertexPositionXControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[0]->Get_ScreenTex()->VertexPositionYControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[1]->Get_ScreenTex()->VertexPositionXControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[1]->Get_ScreenTex()->VertexPositionYControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[2]->Get_ScreenTex()->VertexPositionYControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[3]->Get_ScreenTex()->VertexPositionXControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[3]->Get_ScreenTex()->VertexPositionYControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[4]->Get_ScreenTex()->VertexPositionXControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[4]->Get_ScreenTex()->VertexPositionYControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[5]->Get_ScreenTex()->VertexPositionXControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[5]->Get_ScreenTex()->VertexPositionYControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[6]->Get_ScreenTex()->VertexPositionXControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[6]->Get_ScreenTex()->VertexPositionYControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[7]->Get_ScreenTex()->VertexPositionYControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[8]->Get_ScreenTex()->VertexPositionXControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[8]->Get_ScreenTex()->VertexPositionYControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[9]->Get_ScreenTex()->VertexPositionXControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[9]->Get_ScreenTex()->VertexPositionYControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[10]->Get_ScreenTex()->VertexPositionXControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[11]->Get_ScreenTex()->VertexPositionXControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[13]->Get_ScreenTex()->VertexPositionXControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[14]->Get_ScreenTex()->VertexPositionXControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[15]->Get_ScreenTex()->VertexPositionXControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[15]->Get_ScreenTex()->VertexPositionYControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[16]->Get_ScreenTex()->VertexPositionXControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[16]->Get_ScreenTex()->VertexPositionYControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[17]->Get_ScreenTex()->VertexPositionYControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[18]->Get_ScreenTex()->VertexPositionXControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[18]->Get_ScreenTex()->VertexPositionYControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[19]->Get_ScreenTex()->VertexPositionXControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[19]->Get_ScreenTex()->VertexPositionYControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[20]->Get_ScreenTex()->VertexPositionXControl2(10.f * dTimeDelta);
	m_vecPuzzleObj[20]->Get_ScreenTex()->VertexPositionYControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[21]->Get_ScreenTex()->VertexPositionXControl2(5.f * dTimeDelta);
	m_vecPuzzleObj[21]->Get_ScreenTex()->VertexPositionYControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[22]->Get_ScreenTex()->VertexPositionYControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[23]->Get_ScreenTex()->VertexPositionXControl2(-5.f * dTimeDelta);
	m_vecPuzzleObj[23]->Get_ScreenTex()->VertexPositionYControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[24]->Get_ScreenTex()->VertexPositionXControl2(-10.f * dTimeDelta);
	m_vecPuzzleObj[24]->Get_ScreenTex()->VertexPositionYControl2(-10.f * dTimeDelta);
}

void CPuzzleMgr::Success_Update_Belatos(const _double & dTimeDelta)
{
	if (m_dSuccessDelay <= g_dAccumulatedTime)
	{
		m_pSuccessView = CPuzzlePreview::Create(m_pGraphicDev, 682.5f, 110.f, 500.f, 500.f, 0.05f, L"Texture_PuzzlePreview_Belatos");
		Engine::Add_GameObject(Engine::UI, L"PuzzlePreview_Belatos", m_pSuccessView);
		m_bChangeScene = true;
		m_bSuccess = false;
		return;
	}

	m_vecPuzzleObj[0]->Get_ScreenTex()->VertexPositionXControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[0]->Get_ScreenTex()->VertexPositionYControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[1]->Get_ScreenTex()->VertexPositionXControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[1]->Get_ScreenTex()->VertexPositionYControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[2]->Get_ScreenTex()->VertexPositionXControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[2]->Get_ScreenTex()->VertexPositionYControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[3]->Get_ScreenTex()->VertexPositionXControl2(-7.5f * dTimeDelta);
	m_vecPuzzleObj[3]->Get_ScreenTex()->VertexPositionYControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[4]->Get_ScreenTex()->VertexPositionXControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[4]->Get_ScreenTex()->VertexPositionYControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[5]->Get_ScreenTex()->VertexPositionXControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[5]->Get_ScreenTex()->VertexPositionYControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[6]->Get_ScreenTex()->VertexPositionXControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[6]->Get_ScreenTex()->VertexPositionYControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[7]->Get_ScreenTex()->VertexPositionXControl2(-7.5f * dTimeDelta);
	m_vecPuzzleObj[7]->Get_ScreenTex()->VertexPositionYControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[8]->Get_ScreenTex()->VertexPositionXControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[8]->Get_ScreenTex()->VertexPositionYControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[9]->Get_ScreenTex()->VertexPositionXControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[9]->Get_ScreenTex()->VertexPositionYControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[10]->Get_ScreenTex()->VertexPositionXControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[10]->Get_ScreenTex()->VertexPositionYControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[11]->Get_ScreenTex()->VertexPositionXControl2(-7.5f * dTimeDelta);
	m_vecPuzzleObj[11]->Get_ScreenTex()->VertexPositionYControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[12]->Get_ScreenTex()->VertexPositionXControl2(7.5f * dTimeDelta);
	m_vecPuzzleObj[12]->Get_ScreenTex()->VertexPositionYControl2(-7.5f * dTimeDelta);
	m_vecPuzzleObj[13]->Get_ScreenTex()->VertexPositionXControl2(2.5f * dTimeDelta);
	m_vecPuzzleObj[13]->Get_ScreenTex()->VertexPositionYControl2(-7.5f * dTimeDelta);
	m_vecPuzzleObj[14]->Get_ScreenTex()->VertexPositionXControl2(-2.5f * dTimeDelta);
	m_vecPuzzleObj[14]->Get_ScreenTex()->VertexPositionYControl2(-7.5f * dTimeDelta);
	m_vecPuzzleObj[15]->Get_ScreenTex()->VertexPositionXControl2(-7.5f * dTimeDelta);
	m_vecPuzzleObj[15]->Get_ScreenTex()->VertexPositionYControl2(-7.5f * dTimeDelta);
}
