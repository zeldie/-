#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void					Set_Index(const _ulong& dwIndex) { m_dwIndex = dwIndex; }

	__forceinline _float	Get_Hight() { return m_fHight; }
public:
	HRESULT					Ready_NaviMesh(wstring wstrNaviMeshName);
	HRESULT					Load_NaviMesh(wstring wstrNaviMeshName);
	void					Render_NaviMesh();
	void					Find_PosY(_vec3* pRayPos, _ulong dwIndex, _vec3& vPosY);
	_vec3					Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong& pIndex, _bool bIsJump = false);
	void					Find_Index(_ulong& pIndex, _vec3* pRayPos);
	
	_vec3					Move_OnCamNavi(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong& pIndex, _bool bIsJump = false);
	
	__forceinline _bool		Check_Landing(_vec3* pRayPos, _ulong dwIndex);
	_bool					Check_Index(_vec3 Target, _ulong dwIndex);

	_vec3					CheckY(_vec3 Target, _ulong dwIndex);
private:
	HRESULT					Link_Cell();

private:
	vector<CCell*>			m_vecCell;
	
	_ulong					m_dwIndex;
	_ulong					m_dwNewIndex;
	
	_bool					m_bIsLanding;
	
	_float					m_fHight;
public:
	static  CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrNaviMeshName);
	virtual CComponent*		Clone();
	virtual void			Free();

};
END
#endif // NaviMesh_h__
