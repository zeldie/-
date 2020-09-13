#include "stdafx.h"
#include "RunGameMgr.h"

#include "RunPlayer.h"
#include "RunObj_Obstacle.h"
#include "RunObj_Item.h"

IMPLEMENT_SINGLETON(CRunGameMgr)

CRunGameMgr::CRunGameMgr()
{
}


CRunGameMgr::~CRunGameMgr()
{
	Engine::Safe_Release(m_pGraphicDev);
}

HRESULT CRunGameMgr::Set_Device(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (pGraphicDev == nullptr)
		return E_FAIL;

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	return S_OK;
}

HRESULT CRunGameMgr::Load_Line(wstring wstrPath, LINETYPE eType, RUN_LINE_SECT eSect)
{
	wifstream fin;
	fin.open(wstrPath);

	if (fin.fail())
		return E_FAIL;

	LINE_INFO	tLineInfo;
	_float		fLineLengthSum = 0.f;

	_uint		iTempLineOption = 0;
	while (true)
	{
		fin >> tLineInfo.vStartPos.x;
		fin >> tLineInfo.vStartPos.y;
		fin >> tLineInfo.vStartPos.z;
		fin >> tLineInfo.vEndPos.x;
		fin >> tLineInfo.vEndPos.y;
		fin >> tLineInfo.vEndPos.z;
		fin >> iTempLineOption;
		//// �׽�Ʈ��
		//if (eType == LINE_BOTTOM_RIGHT || eType == LINE_WALL_RIGHT_MID)
		//	tLineInfo.eLineOption = LINE_FALL;
		//else
		//	tLineInfo.eLineOption = (LINEOPTION)iTempLineOption;
		tLineInfo.eLineOption = (LINEOPTION)iTempLineOption;

		if (fin.eof())
			break;

		tLineInfo.vDir = tLineInfo.vEndPos - tLineInfo.vStartPos;
		tLineInfo.fLength = D3DXVec3Length(&tLineInfo.vDir);
		fLineLengthSum += D3DXVec3Length(&tLineInfo.vDir);
		D3DXVec3Normalize(&tLineInfo.vDir, &tLineInfo.vDir);
		tLineInfo.eSect = eSect;
		m_vecLine[eType].push_back(tLineInfo);
	}
	fin.close();

	//m_fLineLenth[eType] = fLineLengthSum;	// �ش� ������ ���� ������ ��
	m_fLineLenth[eSect][eType] = fLineLengthSum;
	_float fRatioSum = 0.f;

	//�� �κ� �����ؾ���
	for (auto& iter : m_vecLine[eType])
	{
		if (iter.eSect != eSect)
			continue;
		_float fSectLength = D3DXVec3Length(&(iter.vEndPos - iter.vStartPos));	// ������ ���ͱ���
		//_float fRatio = fSectLength / fLineLengthSum;	//  �������ͱ��� / ��ü���ͱ���,  == �ش� ������ ���̰� ��ü�������� ��%���� 
		_float fRatio = fSectLength / m_fLineLenth[iter.eSect][eType];	//  �������ͱ��� / ��ü���ͱ���,  == �ش� ������ ���̰� ��ü�������� ��%���� 
		iter.dProgressStart = fRatioSum + (_double)eSect;		// ������ ���൵ ����
		iter.dProgressEnd = (fRatioSum += fRatio) + (_double)eSect;	// ������ ���൵ ��
	}
	return S_OK;
}

HRESULT CRunGameMgr::Create_Obstacle(_uint iSect)
{
	Engine::CLayer* pLayer = Engine::CManagement::GetInstance()->Get_Scene()->Get_Layer(Engine::GAMEOBJECT);
	
	Engine::CGameObject* pGameObject = nullptr;

	// ============== ��ֹ� ���൵ ������ ���� ========================
	// ���൵��, �Ŵ����� �������ִ� ������ ��ü ���ͱ��̿��� �����ǰԲ� �޾ƿ���, ������� �����̶� ���ͱ��� = Z���̴�
	// ������ �����̾ƴ϶� ȸ���� ���¸�, �ش� ������ ���͹޾ƿͼ�, ��ġ�� ��ġ��ŭ�� ���ͱ��̸� ���ؼ�, �ش� ���⺤�͸� ��ֶ������� ��,
	// �ش� ���ͱ��̸�ŭ ���ؼ� ���� X,Y,Z�� ��ֹ��� ��ġ�� �ǵ��� �ϸ�ȴ�.

	// ��ġ�� ��ġ�� ���൵�� �����ؼ�, �ȷο���� �Լ��� ��������, ������ ���Ͱ� = ��ġ�� ��ġ,
	// �浹�� ������ = �뷫 (������Ʈ�� (x,z)ũ�� / ��ü���ͱ��� ) / 2

	_double	dProgress = 0.0;	// ���� �Է��� ���൵��
	_vec3 vPos = { 0.f,0.f,0.f };	// �����Է������ʴ´�. Follow �Լ��� �־��ָ� ��ġ����
	RUNPLAYERMOVESTATE	eMoveState = MOVESTATE_RUN; // �Ⱦ��� ����, ���ڷ� ����ؼ� ����
	_double	dTempAccel;	// �Ⱦ��� ����, ���ڷ� ����ؼ� ����


	// ========================================== �޽��� ��ǥ�� ���� ==========================================
	// Mesh_RunGame_Rock , ������ 0.1 ���� X = 200 , Z = 260
	// ========================================================================================================


	// ===============LINE_BOTTOM_LEFT===================
	dProgress = 0.05;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress,&dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.065;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.08;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.095;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.11;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.125;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.14;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.155;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.17;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.185;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.2;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.60;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.765;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.788;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.7905;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.793;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.815;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.825;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.87;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.873;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.876;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);













	// ===============LINE_BOTTOM_MID===================
	dProgress = 0.05;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.07;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.085;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.1;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.115;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.13;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.145;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.16;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.175;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.19;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.205;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.58;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.685;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.7;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.72;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.735;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.765;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.825;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.85;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);















	// ===============LINE_BOTTOM_RIGHT===================
	dProgress = 0.05;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.065;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.07;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.085;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.09;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.095;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.11;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.115;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.14;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.145;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.16;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.165;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.18;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.185;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.2;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.205;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.56;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.68;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.73;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.75;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.76;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.777;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.825;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.835;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.838;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.841;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.86;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);










	// ===============LINE_WALL_RIGHT_MID===================
	dProgress = 0.22;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.225;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.24;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.245;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.26;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.265;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.28;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.285;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.295;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.3;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.31;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.315;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.325;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.33;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.335;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.345;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.35;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.355;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.37;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.38;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.385;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.53;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.55;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.66;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);








	// ===============LINE_WALL_LEFT_MID===================
	dProgress = 0.22;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.224;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.228;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.240;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.244;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.248;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.260;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.264;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.268;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.280;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.284;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.288;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.3;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.304;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.308;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.320;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.324;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.328;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.34;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.344;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.348;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.36;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.364;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.368;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.38;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.384;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.62;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);




	// ========================================================================================================================================
	// ==============================================================LINE_TOP_MID==============================================================
	// ========================================================================================================================================
	dProgress = 0.41;
	Follow_Line(LINE_TOP_MID , &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.414;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.418;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.43;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.434;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.438;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.45;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.454;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.458;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.47;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.474;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.478;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.49;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.494;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.498;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.51;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.514;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.518;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);

	dProgress = 0.53;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);


	dProgress = 0.64;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Obstacle::Create(m_pGraphicDev, L"Mesh_RunGame_Rock", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(0.03f, 0.015f, 0.03f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_RUN, dProgress, (78.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Obstacle", pGameObject)))
		return E_FAIL;
	m_listObstacle.push_back(pGameObject);
















	// ============================================================== ������ ==============================================================
	// =============== LINE_BOTTOM_LEFT ITME ===================
/*
	dProgress = 0.03;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_RUN, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 30.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.03;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 80.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.03;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 30.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.03;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Recovery", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 30.f, RUNITEM_POTION);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Recovery", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.03;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Big", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 80.f, RUNITEM_POWER);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Big", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.03;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_SpeedUp", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_RUN, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 30.f, RUNITEM_SPEED);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_SpeedUp", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);
*/
	dProgress = 0.03;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_SpeedUp", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_RUN, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 30.f, RUNITEM_SPEED);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_SpeedUp", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);



	// ==== ��ȭ ====

	// ===================== �ٴ� ����

	dProgress = 0.05;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.065;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.08;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.095;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.11;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.125;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.14;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.155;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.17;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.185;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.60;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.788;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.87;
	Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);











	// ===================== �ٴ� �߾�

	dProgress = 0.05;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.07;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.085;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.1;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.115;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.13;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.145;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.16;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.175;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.19;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.205;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.58;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.70;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.72;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.765;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.825;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.85;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.90;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.91;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.92;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.93;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.94;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.95;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.96;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.97;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Jewelry_Blue", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 70.f, RUNITEM_JEWELRY_BLUE);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"RunGame_Item_Jewelry_Blue", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);















	// ===================== �ٴ� ������

	dProgress = 0.065;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.09;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.11;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.14;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.16;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.18;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.20;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.56;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.68;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.72;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.73;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.75;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.76;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.825;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.835;
	Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

















	// ======================= �� ����

	//dProgress = 0.22;
	//Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	//pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
	//	SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	//if (pGameObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
	//	return E_FAIL;
	//m_listItem.push_back(pGameObject);

	dProgress = 0.24;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.26;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.28;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.30;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.32;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.34;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.36;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.38;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.62;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);







	// ======================= �� ������

	//dProgress = 0.22;
	//Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	//pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
	//	SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	//if (pGameObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
	//	return E_FAIL;
	//m_listItem.push_back(pGameObject);

	dProgress = 0.24;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.26;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.28;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.295;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.31;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.325;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.345;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.38;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.53;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.55;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.66;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);





	// ======================= õ��

	//dProgress = 0.41;
	//Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	//pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
	//	SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	//if (pGameObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
	//	return E_FAIL;
	//m_listItem.push_back(pGameObject);

	dProgress = 0.43;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.45;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.47;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.49;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.51;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.53;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	dProgress = 0.64;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(20.f, 20.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_MONEY);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);




	// =============================================== �Ŀ��� ������ ================================

	// ======================= �ٴ� �߾�

	dProgress = 0.698;
	Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Big", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_BOTTOM_MID, OBJ_COLL_RUN, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 30.f, RUNITEM_POWER);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Big", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	// ======================= �� ����

	dProgress = 0.22;
	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Recovery", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 70.f, RUNITEM_POTION);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Potion", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);





	// ======================= �� ������

	dProgress = 0.22;
	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Recovery", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 70.f, RUNITEM_POTION);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Potion", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

	// ======================= õ��

	dProgress = 0.41;
	Follow_Line(LINE_TOP_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Recovery", &vPos, &_vec3(0.f, 0.f, 180.f), &_vec3(50.f, 50.f, 1.f), &_vec3(0.f, 0.f, 0.f),
		SECT_1, LINE_TOP_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_TOP_MID]) *0.5f, 70.f, RUNITEM_POTION);
	if (pGameObject == nullptr)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Potion", pGameObject)))
		return E_FAIL;
	m_listItem.push_back(pGameObject);

/*
	for (_double dPro = 0.05; dPro <= 0.2; dPro += 0.004)
	{
		dProgress = dPro;
		Follow_Line(LINE_BOTTOM_LEFT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
		pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
			SECT_1, LINE_BOTTOM_LEFT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_LEFT]) *0.5f, 80.f, RUNITEM_MONEY);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
			return E_FAIL;
		m_listItem.push_back(pGameObject);

		dProgress = dPro;
		Follow_Line(LINE_BOTTOM_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
		pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
			SECT_1, LINE_BOTTOM_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_MID]) *0.5f, 80.f, RUNITEM_MONEY);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
			return E_FAIL;
		m_listItem.push_back(pGameObject);

		dProgress = dPro;
		Follow_Line(LINE_BOTTOM_RIGHT, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
		pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 0.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
			SECT_1, LINE_BOTTOM_RIGHT, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_BOTTOM_RIGHT]) *0.5f, 80.f, RUNITEM_MONEY);
		if (pGameObject == nullptr)
			return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
			return E_FAIL;
		m_listItem.push_back(pGameObject);
	}
*/

	//// ==== ��ȭ ====
	//for (_double dPro = 0.2; dPro <= 0.39; dPro += 0.004)
	//{
	//	dProgress = dPro;
	//	Follow_Line(LINE_WALL_LEFT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	//	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, -90.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
	//		SECT_1, LINE_WALL_LEFT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_LEFT_MID]) *0.5f, 80.f, RUNITEM_MONEY);
	//	if (pGameObject == nullptr)
	//		return E_FAIL;
	//	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
	//		return E_FAIL;
	//	m_listItem.push_back(pGameObject);

	//	dProgress = dPro;
	//	Follow_Line(LINE_WALL_RIGHT_MID, &eMoveState, &vPos, &_vec3(0.f, 0.f, 0.f), &dProgress, &dTempAccel, 0.0);
	//	pGameObject = CRunObj_Item::Create(m_pGraphicDev, L"Texture_RunGame_Item_Money", &vPos, &_vec3(0.f, 0.f, 90.f), &_vec3(30.f, 30.f, 1.f), &_vec3(0.f, 0.f, 0.f),
	//		SECT_1, LINE_WALL_RIGHT_MID, OBJ_COLL_JUMP, dProgress, (30.f / m_fLineLenth[SECT_1][LINE_WALL_RIGHT_MID]) *0.5f, 80.f, RUNITEM_MONEY);
	//	if (pGameObject == nullptr)
	//		return E_FAIL;
	//	if (FAILED(pLayer->Add_GameObject(L"Run_Item_Money", pGameObject)))
	//		return E_FAIL;
	//	m_listItem.push_back(pGameObject);
	//}


	 
	return S_OK;
}

_bool CRunGameMgr::Change_Line(LINETYPE* eLineType, RUNPLAYERMOVESTATE* eState, KEYDOWN eKey, _vec3* pPos, _vec3* vUp, _vec3 * pAngle, _double* dProgress, _double* dLineAccel, const _double & dTimeDelta)
{ 
	if (*eLineType == LINE_BOTTOM_LEFT)
	{
		if (eKey == KEY_RIGHT)
		{
			*eLineType = LINE_BOTTOM_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 0.f;
				return true;
			}
		}
		else if (eKey == KEY_LEFT)
		{
			*eLineType = LINE_WALL_LEFT_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = -90.f;
				return true;
			}
		}
	}
	else if (*eLineType == LINE_BOTTOM_MID)
	{
		if (eKey == KEY_RIGHT)
		{
			*eLineType = LINE_BOTTOM_RIGHT;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 0.f;
				return true;
			}
		}
		else if (eKey == KEY_LEFT)
		{
			*eLineType = LINE_BOTTOM_LEFT;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 0.f;
				return true;
			}
		}
	}
	else if (*eLineType == LINE_BOTTOM_RIGHT)
	{
		if (eKey == KEY_RIGHT)
		{
			*eLineType = LINE_WALL_RIGHT_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 90.f;
				return true;
			}
		}
		else if (eKey == KEY_LEFT)
		{
			*eLineType = LINE_BOTTOM_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 0.f;
				return true;
			}
		}
	}
	else if (*eLineType == LINE_WALL_RIGHT_MID)
	{
		if (eKey == KEY_RIGHT)
		{
			*eLineType = LINE_TOP_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 180.f;
				return true;
			}
		}
		else if (eKey == KEY_LEFT)
		{
			*eLineType = LINE_BOTTOM_RIGHT;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 0.f;
				return true;
			}
		}
	}
	else if (*eLineType == LINE_WALL_LEFT_MID)
	{
		if (eKey == KEY_RIGHT)
		{
			*eLineType = LINE_BOTTOM_LEFT;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 0.f;
				return true;
			}
		}
		else if (eKey == KEY_LEFT)
		{
			*eLineType = LINE_TOP_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 180.f;
				return true;
			}
		}
	}
	else if (*eLineType == LINE_TOP_MID)
	{
		if (eKey == KEY_RIGHT)
		{
			*eLineType = LINE_WALL_LEFT_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = -90.f;
				return true;
			}
		}
		else if (eKey == KEY_LEFT)
		{
			*eLineType = LINE_WALL_RIGHT_MID;
			if (Follow_Line(*eLineType, eState, pPos, vUp, dProgress, dLineAccel, dTimeDelta))
			{
				pAngle->z = 90.f;
				return true;
			}
		}
	}

	return false;
}

_bool CRunGameMgr::Follow_Line(LINETYPE eLineType, RUNPLAYERMOVESTATE* eState, _vec3* pPos, _vec3* vUp, _double* dProgress, _double* dLineAccel, const _double & dTimeDelta)
{
	for (auto iter : m_vecLine[eLineType])
	{
		if (iter.dProgressStart <= *dProgress && iter.dProgressEnd >= *dProgress)
		{
			_double dLineProgress = iter.dProgressEnd - iter.dProgressStart;
			_double dPlayerProgress = *dProgress - iter.dProgressStart;
			_double dProgressRatio = dPlayerProgress / dLineProgress;

			if (*eState != MOVESTATE_FALL)
			{
				switch (iter.eLineOption)
				{
				case LINE_NORMAL:
				{
					*dLineAccel = 1.f;
					*eState = MOVESTATE_RUN;
					break;
				}
				case LINE_SPEED:
				{
					*dLineAccel = 2.f;
					*eState = MOVESTATE_RUN;
					break;
				}
				case LINE_SLOW:
				{
					*dLineAccel = 0.5f;
					*eState = MOVESTATE_RUN;
					break;
				}
				case LINE_FALL:
				{
					*dLineAccel = 1.0f;
					*eState = MOVESTATE_FALL;
					break;
				}
				default:
					break;
				}

				*pPos = iter.vStartPos + (iter.vDir* (iter.fLength * (_float)dProgressRatio)); // �������� �ؾ��Ѵ�.
			}
			else
			{
				// �������� ���¿���-> ���� ���κ��� �Ʒ����ִٸ� ���� Ÿ�� Ÿ�µ�, FALL���� �ȹٲٸ�ȴ�.
				_vec3 vLinePos = iter.vStartPos + (iter.vDir* (iter.fLength * (_float)dProgressRatio));

				_vec3 vDir = *pPos - vLinePos;
				D3DXVec3Normalize(&vDir, &vDir);

				_float fDot = D3DXVec3Dot(&vDir, vUp);

				if (fDot <= 0.f) // ��� -1�� �ؾ���. ���� Ÿ�� ������ �Ʒ��� �ִٴ� ��,	// �̷��� �ȵ�, �������±����� ������Ž
				{
					*pPos = iter.vStartPos + (iter.vDir* (iter.fLength * (_float)dProgressRatio));
				}
				else
				{
					switch (iter.eLineOption)
					{
					case LINE_NORMAL:
					{
						*dLineAccel = 1.f;
						*eState = MOVESTATE_RUN;
						break;
					}
					case LINE_SPEED:
					{
						*dLineAccel = 2.f;
						*eState = MOVESTATE_RUN;
						break;
					}
					case LINE_SLOW:
					{
						*dLineAccel = 0.5f;
						*eState = MOVESTATE_RUN;
						break;
					}
					case LINE_FALL:
					{
						*dLineAccel = 1.0f;
						*eState = MOVESTATE_FALL;
						break;
					}
					default:
						break;
					}
					*pPos = iter.vStartPos + (iter.vDir* (iter.fLength * (_float)dProgressRatio));
				}
			}
			return true;
		}
	}
	return false;
}

void CRunGameMgr::NullCheck_ItemList()
{
	list<Engine::CGameObject*>::iterator	iter_Begin = m_listItem.begin();
	list<Engine::CGameObject*>::iterator	iter_End = m_listItem.end();

	for (auto& iter = iter_Begin ; iter_Begin != iter_End;)
	{
		if (dynamic_cast<CBaseObject*>(*iter)->Get_isDead())
		{
			iter = m_listItem.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRunGameMgr::NullCheck_ObstacleList()
{
	list<Engine::CGameObject*>::iterator	iter_Begin = m_listObstacle.begin();
	list<Engine::CGameObject*>::iterator	iter_End = m_listObstacle.end();

	for (auto& iter = iter_Begin; iter_Begin != iter_End;)
	{
		if (dynamic_cast<CRunObj_Obstacle*>(*iter)->Get_Fly())
		{
			iter = m_listObstacle.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRunGameMgr::NullCheck_ObstacleList_Dead()
{
	list<Engine::CGameObject*>::iterator	iter_Begin = m_listObstacle.begin();
	list<Engine::CGameObject*>::iterator	iter_End = m_listObstacle.end();

	for (auto& iter = iter_Begin; iter_Begin != iter_End;)
	{
		if (dynamic_cast<CRunObj_Obstacle*>(*iter)->Get_Dissolve())
		{
			iter = m_listObstacle.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRunGameMgr::Clear_List()
{
	m_listItem.clear();
	m_listObstacle.clear();
}
