#include "stdafx.h"
#include "DynamicCamera.h"


#include "Transform.h"
#include "Calculator.h"

#include "MyForm.h"
#include "MapScene.h"

#include "MapTool.h"
#include "MeshTool.h"
#include "NavigationTool.h"
#include "EffectTool.h"
#include "CameraTool.h"
#include "RunTool.h"

#include "ToolDynamicMeshObject.h"
#include "ToolNaviMeshObject.h"
#include "ToolNaviMeshObjectVertex.h"
CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CCamera(pGraphicDev),
	m_fSpeed(0.f),
	m_bClick(false),
	m_bFix(false),
	m_pCalculatorCom(nullptr)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_fSpeed = 1000.f;
	if (FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;
	if (FAILED(Clone_Component()))
		return E_FAIL;
	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _double& dTimeDelta)
{
	Key_Input(dTimeDelta);
	if (true == m_bFix)
	{
		Fix_Mouse();
		Mouse_Move(dTimeDelta);
	}
	_int iExit = Engine::CCamera::Update_GameObject(dTimeDelta);
	return iExit;
}

_int CDynamicCamera::LateUpdate_GameObject(const _double& dTimeDelta)
{
	_int iExit = Engine::CCamera::LateUpdate_GameObject(dTimeDelta);

	return iExit;
}


CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void CDynamicCamera::Free()
{
	Engine::CCamera::Free();
}

HRESULT CDynamicCamera::Clone_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// Picking
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone(Engine::RESOURCE_STATIC, L"CalculatorCom"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent[Engine::ID_DYNAMIC].emplace(Engine::CALCULATOR, pComponent);

	return S_OK;
}

void CDynamicCamera::Key_Input(const _double& dTimeDelta)
{
	CToolGameObject*	pGameObject = nullptr;
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (Engine::KeyPressing(DIK_W))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vLook, &vLook) * _float(m_fSpeed * dTimeDelta);

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::KeyPressing(DIK_S))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vLook, &vLook) * _float(m_fSpeed * dTimeDelta);

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::KeyPressing(DIK_A))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vRight, &vRight) * _float(m_fSpeed * dTimeDelta);

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::KeyPressing(DIK_D))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vRight, &vRight) * _float(m_fSpeed * dTimeDelta);

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::KeyPressing(DIK_Q))
	{
		_vec3	vUp;
		memcpy(&vUp, &matCamWorld.m[1][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vUp, &vUp) * _float(m_fSpeed * dTimeDelta);

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::KeyPressing(DIK_E))
	{
		_vec3	vUp;
		memcpy(&vUp, &matCamWorld.m[1][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vUp, &vUp) *_float(m_fSpeed * dTimeDelta);

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::KeyDown(DIK_Z))
	{
		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}


	if (g_pMyForm->m_eCurToolType == CMyForm::MAPTOOL)
	{
	}
	else if (g_pMyForm->m_eCurToolType == CMyForm::MESHTOOL)
	{
		if (Engine::KeyPressing(DIK_LCONTROL))
		{
			if (Engine::MouseDown(Engine::DIM_RB))
			{
				CToolGameObject* pPickingObject = nullptr;
				_float fOldDist = 50000.f;
				map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
				for (auto& ObjTag : mapStaticMesh)
				{
					for (auto& rGameObject : ObjTag.second)
					{
						_vec3 vOutPos;
						if (m_pCalculatorCom->PickMesh(g_hWnd, rGameObject, &vOutPos, &fOldDist))
						{
							g_pMeshTool->Set_Transform(static_cast<CToolGameObject*>(rGameObject)->Get_Transform());
							g_pMeshTool->m_vecPos = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_Info(Engine::INFO_POS));
							g_pMeshTool->m_vecRot = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_Angle());
							g_pMeshTool->m_vecScale = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_Scale());
							g_pMeshTool->m_vSelfRotAngle = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_SelfRotAngle());
							pPickingObject = static_cast<CToolGameObject*>(rGameObject);
							g_pMeshTool->UpdateData(FALSE);
						}
						static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
					}
				}

				map<wstring, list<CGameObject*>> mapDynamicMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::DYNAMICMESH)->Get_mapGameObject();
				for (auto& ObjTag : mapDynamicMesh)
				{
					for (auto& rGameObject : ObjTag.second)
					{
						_vec3 vOutPos;
						if (m_pCalculatorCom->PickDynamicMesh(g_hWnd, rGameObject, static_cast<CToolDynamicMeshObject*>(rGameObject)->Get_PickingSphere(), &vOutPos, &fOldDist))
						{
							g_pMeshTool->Set_Transform(static_cast<CToolGameObject*>(rGameObject)->Get_Transform());
							g_pMeshTool->m_vecPos = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_Info(Engine::INFO_POS));
							g_pMeshTool->m_vecRot = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_Angle());
							g_pMeshTool->m_vecScale = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_Scale());
							g_pMeshTool->m_vSelfRotAngle = *(static_cast<CToolGameObject*>(rGameObject)->Get_Transform()->Get_SelfRotAngle());
							pPickingObject = static_cast<CToolGameObject*>(rGameObject);
							g_pMeshTool->UpdateData(FALSE);
						}
						static_cast<CToolGameObject*>(rGameObject)->Set_WireFrame(false);
					}
				}
				if (pPickingObject == nullptr)
					return;
				pPickingObject->Set_WireFrame(true);
			}
		}
		else
		{
			if (Engine::MouseDown(Engine::DIM_RB))
			{
				_float fOldDist = 99999.f;
				_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);

				m_pCalculatorCom->PickMesh(g_hWnd, (Engine::CGameObject*)g_pMapScene->m_pTerrain, &vOutPos, &fOldDist);
				g_pMeshTool->m_vecPos = vOutPos;
				g_pMeshTool->m_vecRot = _vec3(0.f, 0.f, 0.f);
				g_pMeshTool->m_vecScale = _vec3(0.1f, 0.1f, 0.1f);
				g_pMeshTool->UpdateData(FALSE);

				map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
				for (auto& ObjTag : mapStaticMesh)
				{
					for (auto& rGameObject : ObjTag.second)
					{
						if (m_pCalculatorCom->PickMesh(g_hWnd, rGameObject, &vOutPos, &fOldDist))
						{
							g_pMeshTool->m_vecPos = vOutPos;
							g_pMeshTool->m_vecRot = _vec3(0.f, 0.f, 0.f);
							g_pMeshTool->m_vecScale = _vec3(0.1f, 0.1f, 0.1f);
							g_pMeshTool->m_vSelfRotAngle = _vec3(0.f, 0.f, 0.f);
							g_pMeshTool->UpdateData(FALSE);
						}
					}
				}


			}
		}

	}
	else if (g_pMyForm->m_eCurToolType == CMyForm::NAVIGATIONTOOL)
	{
		if (g_pNavigationTool->m_eNaviMode == CNavigationTool::ADD)
		{
			if (Engine::MouseDown(Engine::DIM_RB))
			{
				_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);

				for (auto& NaviMeshObject : g_pNavigationTool->m_vecNaviMesh)
				{
					vector<CToolNaviMeshObjectVertex*> vertex = NaviMeshObject->Get_vecVertex();
					for (auto& vecVertex : vertex)
					{
						if (vOutPos != _vec3(0.f, 0.f, 0.f))
							continue;
						if (m_pCalculatorCom->PickNaviMesh(g_hWnd, vecVertex, vecVertex->Get_Sphere(), &vOutPos))
						{
							vOutPos = *(vecVertex->Get_Transform()->Get_Info(Engine::INFO_POS));
						}
					}
				}

				if (vOutPos == _vec3(0.f, 0.f, 0.f))
				{
					_float fOldDist = 50000.f;

					m_pCalculatorCom->PickStaticMesh(g_hWnd, (Engine::CGameObject*)g_pMapScene->m_pTerrain, &vOutPos, &fOldDist);
					//g_pNavigationTool->m_vecPos = vOutPos;
					map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
					for (auto& ObjTag : mapStaticMesh)
					{
						for (auto& rGameObject : ObjTag.second)
						{
							if (m_pCalculatorCom->PickStaticMesh(g_hWnd, rGameObject, &vOutPos, &fOldDist))
							{
								//g_pNavigationTool->m_vecPos = vOutPos;
							}
						}
					}
					
				}

				if (vOutPos == _vec3(0.f, 0.f, 0.f))
					return;

				//네비매쉬 먼저 만듬
				// 맨 마지막 원소가 버텍스 3개를 가지고 있으면 새로 만들어야함
				if ((g_pNavigationTool->m_vecNaviMesh.empty()) || (dynamic_cast<CToolNaviMeshObject*>(g_pNavigationTool->Get_LastMeshObject())->Get_VertexNum() >= 3))
				{

					CString cstrMeshNum = _T("");
					cstrMeshNum.Format(_T("%d"), g_pNavigationTool->m_iMeshNum);
					g_pNavigationTool->m_ParentItem = g_pNavigationTool->m_treeNavigation.InsertItem(cstrMeshNum);
					pGameObject = CToolNaviMeshObject::Create(m_pGraphicDev, g_pNavigationTool->m_eNaviType);
					Engine::Add_GameObject(Engine::NAVIMESH, L"NaviMesh", pGameObject);

					g_pNavigationTool->m_vecNaviMesh.emplace_back((CToolNaviMeshObject*)pGameObject);
					g_pNavigationTool->m_iMeshNum++;
					g_pNavigationTool->m_treeNavigation.Expand(g_pNavigationTool->m_ParentItem, TVE_EXPAND);
				}
				else
				{

				}
				// vOutPos.x << " , " << vOutPos.y << vOutPos.z << endl;
				pGameObject = CToolNaviMeshObjectVertex::Create(m_pGraphicDev, vOutPos);
				Engine::Add_GameObject(Engine::NAVIMESH, L"Vertex", pGameObject);
				_uint iVertexNum = dynamic_cast<CToolNaviMeshObject*>(g_pNavigationTool->Get_LastMeshObject())->Get_VertexNum();
				dynamic_cast<CToolNaviMeshObject*>(g_pNavigationTool->Get_LastMeshObject())->Add_Vertex((CToolNaviMeshObjectVertex*)pGameObject);
				CString cstrVertexNum = _T("");
				cstrVertexNum.Format(_T("%d"), iVertexNum);

				g_pNavigationTool->m_ChildItem = g_pNavigationTool->m_treeNavigation.InsertItem(cstrVertexNum, 0, 0, g_pNavigationTool->m_ParentItem, TVI_LAST);
				g_pNavigationTool->m_vecPos = vOutPos;
				g_pNavigationTool->UpdateData(FALSE);
			}
		}
		else if (g_pNavigationTool->m_eNaviMode == CNavigationTool::EDIT)
		{
			if (Engine::MouseDown(Engine::DIM_RB))
			{
				_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);

				for (auto& NaviMeshObject : g_pNavigationTool->m_vecNaviMesh)
				{
					vector<CToolNaviMeshObjectVertex*> vertex = NaviMeshObject->Get_vecVertex();
					for (auto& vecVertex : vertex)
					{

						if (m_pCalculatorCom->PickNaviMesh(g_hWnd, vecVertex, vecVertex->Get_Sphere(), &vOutPos))
						{
							vecVertex->Set_Pick(true);
							g_pNavigationTool->m_vecPos = *vecVertex->Get_Transform()->Get_Info(Engine::INFO_POS);
						}
						else
							vecVertex->Set_Pick(false);
					}
				}
				g_pNavigationTool->UpdateData(FALSE);
			}
			if (Engine::MousePressing(Engine::DIM_RB))
			{
				_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);
				for (auto& NaviMeshObject : g_pNavigationTool->m_vecNaviMesh)
				{
					vector<CToolNaviMeshObjectVertex*> vertex = NaviMeshObject->Get_vecVertex();
					for (auto& vecVertex : vertex)
					{
						if (vecVertex->Get_Pick())
						{
							_float fOldDist = 50000.f;
							if (m_pCalculatorCom->PickMesh(g_hWnd, (Engine::CGameObject*)g_pMapScene->m_pTerrain, &vOutPos, &fOldDist))
							{
								vecVertex->Get_Transform()->Set_Info(&vOutPos, Engine::INFO_POS);
								g_pNavigationTool->m_vecPos = vOutPos;
							}
						}
					}
				}
			}
			g_pNavigationTool->UpdateData(FALSE);
		}
	}
	else if (g_pMyForm->m_eCurToolType == CMyForm::EFFECTTOOL)
	{

	}
	else if (g_pMyForm->m_eCurToolType == CMyForm::CAMTOOL)
	{
		if (Engine::MouseDown(Engine::DIM_LB))
		{
		/*	_float fOldDist = 9999.f;
			_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);

			m_pCalculatorCom->PickMesh(g_hWnd, (Engine::CGameObject*)g_pMapScene->m_pTerrain, &vOutPos, &fOldDist);
			
			if (g_pCamTool->m_eMode == CCameraTool::HEAD)
			{
				g_pCamTool->Create_Eye();
				vOutPos.y += 100.f;
				g_pCamTool->m_vecEye.back()->vHead = vOutPos;
				g_pCamTool->m_eMode = CCameraTool::TAIL;
			}

			else if (g_pCamTool->m_eMode == CCameraTool::TAIL)
			{
				vOutPos.y += 100.f;
				g_pCamTool->m_vecEye.back()->vTail = vOutPos;
				g_pCamTool->m_eMode = CCameraTool::SPLINE;
			}

			else if (g_pCamTool->m_eMode == CCameraTool::SPLINE)
			{
				vOutPos.y += 100.f;
				g_pCamTool->m_vecEye.back()->vSpline1 = vOutPos;
				g_pCamTool->Set_Spline();
				g_pCamTool->m_eMode = CCameraTool::HEAD;
			}*/

		}
	}
	else if (g_pMyForm->m_eCurToolType == CMyForm::RUNTOOL)
	{

		if (g_pRunTool->m_eRunMode == CRunTool::ADD)
		{
			if (Engine::MouseDown(Engine::DIM_RB))
			{
				_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);

				for (auto& RunLine : g_pRunTool->m_vecRunLine)
				{
					vector<CToolNaviMeshObjectVertex*> vertex = RunLine->Get_vecVertex();
					for (auto& vecVertex : vertex)
					{
						if (vOutPos != _vec3(0.f, 0.f, 0.f))
							continue;
						if (m_pCalculatorCom->PickNaviMesh(g_hWnd, vecVertex, vecVertex->Get_Sphere(), &vOutPos))
						{
							vOutPos = *(vecVertex->Get_Transform()->Get_Info(Engine::INFO_POS));
						}
					}
				}

				if (vOutPos == _vec3(0.f, 0.f, 0.f))
				{
					_float fOldDist = 50000.f;

					m_pCalculatorCom->PickStaticMesh(g_hWnd, (Engine::CGameObject*)g_pMapScene->m_pTerrain, &vOutPos, &fOldDist);
					//g_pNavigationTool->m_vecPos = vOutPos;
					map<wstring, list<Engine::CGameObject*>> mapStaticMesh = g_pMapScene->Get_Layer(Engine::LAYERTYPE::STATICMESH)->Get_mapGameObject();
					for (auto& ObjTag : mapStaticMesh)
					{
						for (auto& rGameObject : ObjTag.second)
						{
							if (m_pCalculatorCom->PickStaticMesh(g_hWnd, rGameObject, &vOutPos, &fOldDist))
							{
								g_pRunTool->m_vPos = vOutPos; // 주석이었음
							}
						}
					}

				}

				if (vOutPos == _vec3(0.f, 0.f, 0.f))
					return;

				//네비매쉬 먼저 만듬
				// 맨 마지막 원소가 버텍스 3개를 가지고 있으면 새로 만들어야함
				if ((g_pRunTool->m_vecRunLine.empty()) || (dynamic_cast<CToolRunGameLine*>(g_pRunTool->Get_LastMeshObject())->Get_VertexNum() >= 2))
				{

					CString cstrMeshNum = _T("");
					cstrMeshNum.Format(_T("%d"), g_pRunTool->m_iLineNum);
					g_pRunTool->m_ParentItem = g_pRunTool->m_treeCtrl.InsertItem(cstrMeshNum);
					pGameObject = CToolRunGameLine::Create(m_pGraphicDev, g_pRunTool->m_eOption);
					Engine::Add_GameObject(Engine::NAVIMESH, L"RunGameLine", pGameObject);

					g_pRunTool->m_vecRunLine.emplace_back((CToolRunGameLine*)pGameObject);
					g_pRunTool->m_iLineNum++;
					g_pRunTool->m_treeCtrl.Expand(g_pRunTool->m_ParentItem, TVE_EXPAND);
				}
				else
				{

				}
				// vOutPos.x << " , " << vOutPos.y << vOutPos.z << endl;
				pGameObject = CToolNaviMeshObjectVertex::Create(m_pGraphicDev, vOutPos);
				Engine::Add_GameObject(Engine::NAVIMESH, L"RunGameLineVtx", pGameObject);
				_uint iVertexNum = dynamic_cast<CToolRunGameLine*>(g_pRunTool->Get_LastMeshObject())->Get_VertexNum();
				dynamic_cast<CToolRunGameLine*>(g_pRunTool->Get_LastMeshObject())->Add_Vertex((CToolNaviMeshObjectVertex*)pGameObject);
				CString cstrVertexNum = _T("");
				cstrVertexNum.Format(_T("%d"), iVertexNum);

				g_pRunTool->m_ChildItem = g_pRunTool->m_treeCtrl.InsertItem(cstrVertexNum, 0, 0, g_pRunTool->m_ParentItem, TVI_LAST);
				g_pRunTool->m_vPos = vOutPos;
				g_pRunTool->UpdateData(FALSE);
			}
		}
		else if (g_pRunTool->m_eRunMode == CRunTool::EDIT)
		{
			if (Engine::MouseDown(Engine::DIM_RB))
			{
				_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);

				for (auto& LineObject : g_pRunTool->m_vecRunLine)
				{
					vector<CToolNaviMeshObjectVertex*> vertex = LineObject->Get_vecVertex();
					for (auto& vecVertex : vertex)
					{

						if (m_pCalculatorCom->PickNaviMesh(g_hWnd, vecVertex, vecVertex->Get_Sphere(), &vOutPos))
						{
							vecVertex->Set_Pick(true);
							g_pRunTool->m_vPos = *vecVertex->Get_Transform()->Get_Info(Engine::INFO_POS);
						}
						else
							vecVertex->Set_Pick(false);
					}
				}
				g_pRunTool->UpdateData(FALSE);
			}
			if (Engine::MousePressing(Engine::DIM_RB))
			{
				_vec3 vOutPos = _vec3(0.f, 0.f, 0.f);
				for (auto& LineObject : g_pRunTool->m_vecRunLine)
				{
					vector<CToolNaviMeshObjectVertex*> vertex = LineObject->Get_vecVertex();
					for (auto& vecVertex : vertex)
					{
						if (vecVertex->Get_Pick())
						{
							_float fOldDist = 50000.f;
							if (m_pCalculatorCom->PickMesh(g_hWnd, (Engine::CGameObject*)g_pMapScene->m_pTerrain, &vOutPos, &fOldDist))
							{
								vecVertex->Get_Transform()->Set_Info(&vOutPos, Engine::INFO_POS);
								g_pRunTool->m_vPos = vOutPos;
							}
						}
					}
				}
			}
			g_pRunTool->UpdateData(FALSE);
		}
	}
}

void CDynamicCamera::Mouse_Move(const _double& dTimeDelta)
{
	_long		dwMouseMove = 0;
	_matrix		matWorld, matRot;
	D3DXMatrixInverse(&matWorld, nullptr, &m_matView);
	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_Y))
	{
		_vec3 vRight;
		memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

		_vec3 vLook = m_vAt - m_vEye;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove*0.1f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::MouseMove(Engine::DIMS_X))
	{
		_vec3	vUp = _vec3(0.f, 1.f, 0.f);
		_vec3	vLook = m_vAt - m_vEye;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove*0.1f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_vAt = m_vEye + vLook;
	}

}

void CDynamicCamera::Fix_Mouse()
{
	POINT	ptMouse{ TOOL_WINCX >> 1, TOOL_WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

