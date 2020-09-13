#pragma once
#ifndef __CCOLLISIONMGR_H__
#define __CCOLLISIONMGR_H__

#include "SphereCollider.h"

class CBaseObject;

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	CCollisionMgr();
	~CCollisionMgr();

public:
	void	Add_Collision(COLLLIST eListType, CBaseObject* pOBJ);
	void	Update_Collision();

private:
	void	Clear_Collision();

private:
	list<CBaseObject*>		m_listOBJ[COLLLIST::COLL_END];
};

#endif // !__CCOLLISIONMGR_H__
