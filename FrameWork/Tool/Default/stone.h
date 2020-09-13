#ifndef Stone_h__
#define Stone_h__

#include "ToolGameObject.h"


BEGIN(ENGINE)

class CStaticMesh;
class CPicking;
class CCubeCollider;
END

class CStone : public CToolGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStone(void);

public:
	virtual HRESULT	Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float& fDeltaTime) override;
	virtual _int	LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual void	Render_GameObject(void) override;

private:
	HRESULT			Add_Component(void);
	void			SetUp_OnTerrain(void);
	_vec3			PickUp_OnTerrain(void);

private:
	ENGINE::CPicking*		m_pPikcingCom = nullptr;
	ENGINE::CStaticMesh*	m_pMeshCom = nullptr;
	ENGINE::CCubeCollider*	m_pColliderCom = nullptr;
	_vec3					m_vDirection;
	_float					m_fSpeed = 5.f;
public:
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vecPos);

private:
	virtual void			Free(void);


};


#endif // Stone_h__
