#include "stdafx.h"
#include "CollisionMgr.h"
#include "BaseObject.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
	Clear_Collision();
}

void CCollisionMgr::Add_Collision(COLLLIST eListType, CBaseObject * pOBJ)
{
	m_listOBJ[eListType].push_back(pOBJ);
	pOBJ->AddRef();
}

void CCollisionMgr::Update_Collision()
{
	vector<Engine::CSphereCollider*>*	pOutCollSphere = nullptr;
	vector<Engine::CSphereCollider*>*	pInCollSphere = nullptr;
	vector<Engine::CCubeCollider*>*		pInCollCube = nullptr;
	_vec3	vOutPos;
	_vec3	vInPos;
	_float	fDist = 0.f, fLength = 0.f;

	// 구 : 구 충돌
	for (int i = 0; i < COLLLIST::COLL_PLAYER_EFFECT; ++i)
	{
		// 바깥 for
		for (auto Outiter : m_listOBJ[i])
		{
			// 컬라이더 벡터포인터 받아온다
			pOutCollSphere = Outiter->Get_CollSpherVector();
			if (pOutCollSphere == nullptr)
				return;
			// 1차 충돌 구체의 위치
			vOutPos = pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST].front()->Get_Pos();
			// 사실 for가 아니다. if문으로 해도된다.
			for (int j = i + 1; j < COLLLIST::COLL_MONSTER_EFFECT_FAN; j++)
			{
				if ((i == COLLLIST::COLL_PLAYER) && (j == COLLLIST::COLL_PLAYER_EFFECT))
					continue;
				if ((i == COLLLIST::COLL_MONSTER) && (j == COLLLIST::COLL_MONSTER_EFFECT))
					continue;
				// 안쪽 for
				for (auto Initer : m_listOBJ[j])
				{
					pInCollSphere = Initer->Get_CollSpherVector();
					if (pInCollSphere == nullptr)
						return;

					//1차 충돌체크 바깥 구 : 바깥 구
					for (auto OutFirst : pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST])
					{
						vOutPos = OutFirst->Get_Pos();
						for (auto InFirst : pInCollSphere[COLLSPHERETYPE::SPHERE_FIRST])
						{
							vInPos = InFirst->Get_Pos();
							fLength = D3DXVec3Length(&(vOutPos - vInPos));
							fDist = OutFirst->Get_Radius() + InFirst->Get_Radius();

							if (fLength <= fDist)
							{
								Outiter->Set_Collision(Initer, COLLIDER::COLLIDER_DEF_HIT);
								// 2차충돌체크  공 : 방
								for (auto OutAtt : pOutCollSphere[COLLSPHERETYPE::SPHERE_ATT])
								{
									vOutPos = OutAtt->Get_Pos();
									for (auto InDef : pInCollSphere[COLLSPHERETYPE::SPHERE_DEF])
									{
										vInPos = InDef->Get_Pos();
										fLength = D3DXVec3Length(&(vOutPos - vInPos));
										fDist = OutAtt->Get_Radius() + InDef->Get_Radius();

										if (fLength <= fDist)
										{
											Initer->Set_Collision(Outiter, COLLIDER::COLLIDER_ATT_HIT);
										}
									}
								}// 공 : 방
								// 2차충돌체크  방 : 공
								for (auto OutDef : pOutCollSphere[COLLSPHERETYPE::SPHERE_DEF])
								{
									vOutPos = OutDef->Get_Pos();
									for (auto InAtt : pInCollSphere[COLLSPHERETYPE::SPHERE_ATT])
									{
										vInPos = InAtt->Get_Pos();
										fLength = D3DXVec3Length(&(vOutPos - vInPos));
										fDist = OutDef->Get_Radius() + InAtt->Get_Radius();

										if (fLength <= fDist)
										{
											Outiter->Set_Collision(Initer, COLLIDER::COLLIDER_ATT_HIT);
										}
									}
								}// 방 : 공

								// 2차충돌체크  방 : 방
								for (auto OutDef : pOutCollSphere[COLLSPHERETYPE::SPHERE_DEF])
								{
									vOutPos = OutDef->Get_Pos();
									for (auto InDef : pInCollSphere[COLLSPHERETYPE::SPHERE_DEF])
									{
										vInPos = InDef->Get_Pos();
										fLength = D3DXVec3Length(&(vOutPos - vInPos));
										fDist = OutDef->Get_Radius() + InDef->Get_Radius();

										/*if (fLength <= fDist)
										{
											Outiter->Set_Collision(Initer, COLLIDER::COLLIDER_DEF_HIT);
											Initer->Set_Collision(Outiter, COLLIDER::COLLIDER_DEF_HIT);											
										}*/
									}
								}// 방 : 방
							}
						}
					}// 1차 충돌체크,  바깥 구 : 바깥 구
				}// 안쪽 for
			}
		}// 바깥 for
	}


	// 부채꼴 : 구 충돌
	for (int i = 0; i < COLLLIST::COLL_PLAYER_EFFECT; ++i)
	{
		// 바깥 for
		for (auto Outiter : m_listOBJ[i])
		{
			// 컬라이더 벡터포인터 받아온다
			pOutCollSphere = Outiter->Get_CollSpherVector();
			if (pOutCollSphere == nullptr)
				return;
			// 1차 충돌 구체의 위치
			vOutPos = pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST].front()->Get_Pos();
			// 사실 for가 아니다. if문으로 해도된다.
			for (int j = i + 1; j < COLLLIST::COLL_MONSTER_EFFECT_CAPSULE; j++)
			{
				if (j == COLLLIST::COLL_MONSTER)
					continue;
				if (j == COLLLIST::COLL_PLAYER_EFFECT)
					continue;
				if (j == COLLLIST::COLL_MONSTER_EFFECT)
					continue;
				if (i == COLLLIST::COLL_MONSTER && j == COLLLIST::COLL_MONSTER_EFFECT_FAN)
					continue;
				// 안쪽 for
				for (auto Initer : m_listOBJ[j])
				{
					pInCollSphere = Initer->Get_CollSpherVector();
					if (pInCollSphere == nullptr)
						return;

					//1차 충돌체크 바깥 구 : 바깥 구
					for (auto OutFirst : pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST])
					{
						vOutPos = OutFirst->Get_Pos();
						for (auto InFirst : pInCollSphere[COLLSPHERETYPE::SPHERE_FIRST])
						{
							vInPos = InFirst->Get_Pos();
							fLength = D3DXVec3Length(&(vOutPos - vInPos));
							fDist = OutFirst->Get_Radius() + InFirst->Get_Radius();

							if (fLength <= fDist)
							{
								for (auto OutAtt : pOutCollSphere[COLLSPHERETYPE::SPHERE_DEF])
								{
									vOutPos = OutAtt->Get_Pos();
									for (auto InDef : pInCollSphere[COLLSPHERETYPE::SPHERE_FIRST])
									{
										vInPos = InDef->Get_Pos();
										fLength = D3DXVec3Length(&(vOutPos - vInPos));
										fDist = OutAtt->Get_Radius() + InDef->Get_Radius();

										if (fLength <= fDist)
										{
											_vec3 vDir, vLook;
											vOutPos = Outiter->Get_TransformCom()->m_vInfo[Engine::INFO_POS];
											vInPos = Initer->Get_TransformCom()->m_vInfo[Engine::INFO_POS];

											vDir = vOutPos - vInPos;
											D3DXVec3Normalize(&vDir, &vDir);
											vLook = Initer->Get_BaseInfo()->vFanLook;
											D3DXVec3Normalize(&vLook, &vLook);

											_float fDot = D3DXVec3Dot(&vLook, &vDir);
											_float fHalfDot = cosf(D3DXToRadian(Initer->Get_BaseInfo()->fHalfAngle));

											if (fDot >= fHalfDot)
											{
												Outiter->Set_Collision(Initer, COLLIDER::COLLIDER_ATT_HIT);
											}
										}
									}
								}
							}
						}
					}// 1차 충돌체크,  바깥 구 : 바깥 구
				}// 안쪽 for
			}
		}// 바깥 for
	}

	// 캡슐 : 구 충돌
	for (int i = 0; i < COLLLIST::COLL_MONSTER; ++i)
	{
		// 바깥 for
		for (auto Outiter : m_listOBJ[i])
		{
			// 컬라이더 벡터포인터 받아온다
			pOutCollSphere = Outiter->Get_CollSpherVector();
			if (pOutCollSphere == nullptr)
				return;
			// 1차 충돌 구체의 위치
			vOutPos = pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST].front()->Get_Pos();

			// 안쪽 for
			for (auto Initer : m_listOBJ[COLLLIST::COLL_MONSTER_EFFECT_CAPSULE])
			{
				vInPos = Initer->Get_TransformCom()->m_vInfo[Engine::INFO_POS];
				vInPos.y = 50.f;
				vOutPos.y = 50.f;
				_vec3 vLook = Initer->Get_TransformCom()->m_vInfo[Engine::INFO_LOOK];
				D3DXVec3Normalize(&vLook,&vLook);
				vLook *= 3000.f;
				vLook.y = 50.f;
				_vec3 vCapsuleEndPos = vLook - vInPos;
				_vec3 vTarget = vOutPos - vInPos;
				_vec3 vTarget2 = vOutPos - vInPos;

				D3DXVec3Normalize(&vCapsuleEndPos, &vCapsuleEndPos);
				D3DXVec3Normalize(&vTarget, &vTarget);
				_vec3 vResultCapsule = vTarget2 * D3DXVec3Dot(&vCapsuleEndPos, &vTarget);
				vCapsuleEndPos *= D3DXVec3Length(&vResultCapsule);

				float fRadius = pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST].front()->Get_Radius() + Initer->Get_CollSpherVector()->front()->Get_Radius();
				// 1차 충돌체크 레이저 : 바깥
				if (fRadius > D3DXVec3Length(&(vCapsuleEndPos - vOutPos)))
				{
					// 2차 충돌체크 레이저 : 몸통
					for (auto OutFirst : pOutCollSphere[COLLSPHERETYPE::SPHERE_DEF])
					{
						vOutPos = OutFirst->Get_Pos();	//플레이어 몸통
						vOutPos.y = 50.f;

						_vec3 vCapsuleEndPos = vLook - vInPos;	//레이저 벡터
						_vec3 vTarget = vOutPos - vInPos;
						_vec3 vTarget2 = vOutPos - vInPos;

						D3DXVec3Normalize(&vCapsuleEndPos, &vCapsuleEndPos);
						D3DXVec3Normalize(&vTarget, &vTarget);
						_vec3 vResultCapsule = vTarget2 * D3DXVec3Dot(&vCapsuleEndPos, &vTarget);
						vCapsuleEndPos *= D3DXVec3Length(&vResultCapsule);

						float fRadius = OutFirst->Get_Radius() + Initer->Get_CollSpherVector()->front()->Get_Radius();
						float fLength = D3DXVec3Length(&(vCapsuleEndPos - vOutPos));
						if (fRadius > fLength)
						{
							Outiter->Set_Collision(Initer, COLLIDER::COLLIDER_ATT_HIT);
						}
					}
				}
			}// 안쪽 for
		}// 바깥 for
	}

	//// OBB : 구 충돌
	//for (int i = 0; i < COLLLIST::COLL_PLAYER_EFFECT; ++i)
	//{
	//	// 바깥 for
	//	for (auto Outiter : m_listOBJ[i])
	//	{
	//		// 컬라이더 벡터포인터 받아온다
	//		pOutCollSphere = Outiter->Get_CollSpherVector();
	//		if (pOutCollSphere == nullptr)
	//			return;
	//		// 1차 충돌 구체의 위치
	//		vOutPos = pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST].front()->Get_Pos();

	//		// 사실 for가 아니다. if문으로 해도된다.
	//		//for (int j = i + 1; j < COLLLIST::COLL_MONSTER_EFFECT_BOX; j++);


	//			// 안쪽 for
	//			for (auto Initer : m_listOBJ[COLLLIST::COLL_MONSTER_EFFECT_BOX])
	//			{
	//				pInCollCube = Initer->Get_CollBoxVector();
	//				if (pInCollCube == nullptr)
	//					return;

	//				// 1차 OBB : 바깥 구
	//				//1차 충돌체크 바깥 구 : 바깥 구
	//				for (auto OutFirst : pOutCollSphere[COLLSPHERETYPE::SPHERE_FIRST])
	//				{
	//					vOutPos = OutFirst->Get_Pos();
	//					for (auto InFirst : pInCollCube[COLLBOXTYPE::BOX_ATT])
	//					{

	//						if (Engine::ColSphere_OBB(InFirst->Get_Min(), InFirst->Get_Max(), &InFirst->Get_WorldMatrix(), vOutPos, OutFirst->Get_Radius()))
	//						{

	//							for (auto OutDef : pOutCollSphere[COLLSPHERETYPE::SPHERE_DEF])
	//							{

	//								if (Engine::ColSphere_OBB(InFirst->Get_Min(), InFirst->Get_Max(), &InFirst->Get_WorldMatrix(), OutDef->Get_Pos(), OutDef->Get_Radius()))
	//								{

	//									Outiter->Set_Collision(Outiter, COLLIDER::COLLIDER_ATT_HIT);

	//								}
	//							}
	//						}
	//						
	//					}
	//				}// 1차 충돌체크,  바깥 구 : 바깥 구
	//			}// 안쪽 for
	//		
	//	}// 바깥 for
	//}

	Clear_Collision();
}

void CCollisionMgr::Clear_Collision()
{
	for (_uint i = 0; i < COLLLIST::COLL_END; ++i)
	{
		for_each(m_listOBJ[i].begin(), m_listOBJ[i].end(), Engine::CDeleteObj());
		m_listOBJ[i].clear();
	}
}