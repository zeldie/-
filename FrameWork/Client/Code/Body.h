#pragma once

#include "DynamicMeshObject.h"
#include "Texture.h"
#include "Top.h"
#include "Pants.h"
#include "Foot.h"
#include "Hands.h"
#include "FrontHair.h"
#include "SideHair.h"
#include "TailHair.h"
#include "BackHair.h"
class CBody : public CDynamicMeshObject
{
public:
	enum SEX
	{
		MALE,
		FEMALE,
		SEX_END
	};

	enum TEXTURE_CHANGE
	{
		BROW,
		EYE_D,
		EYE_N,
		TEXTURE_CHANGE_END
	};
private:
	explicit CBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBody();

public:
	////Getter 
	//ÈñÁ¤Ãß°¡
	_uint				Get_iEyeNum() { return m_iEyeNum; } 
	_uint				Get_iBrowNum() { return m_iBrowNum; }
	_uint				Get_FrontHairIndex(); 
	_uint				Get_TailHairIndex(); 
	_uint				Get_BackHairIndex();
	_uint				Get_SideHairIndex(); 
	_uint				Get_TopIndex();
	_uint				Get_PantsIndex();
	_uint				Get_FootIndex();
	_uint				Get_HandsIndex();

	//Setter
	//ÈñÁ¤Ãß°¡
	void				Set_AnimationNum(_uint iNum) { m_iAnimationNum = iNum; }

public:
	HRESULT				Link_Top(_uint iTopNum);
	HRESULT				Link_Pants(_uint iPantsNum);
	HRESULT				Link_Foot(_uint iFootNum);
	HRESULT				Link_Hands(_uint iHandsNum);
	HRESULT				Link_FrontHair(_uint iFrontHairNum);
	HRESULT				Link_TailHair(_uint iTailHairNum);
	HRESULT				Link_BackHair(_uint iBackHairNum);
	HRESULT				Link_SideHair(_uint iSideHairNum);
	HRESULT				Change_Top(_uint iTopNum);
	HRESULT				Change_Pants(_uint iPantsNum);
	HRESULT				Change_Foot(_uint iFootNum);
	HRESULT				Change_Hands(_uint iHandsNum);
	HRESULT				Change_FrontHair(_uint iFrontHairNum);
	HRESULT				Change_TailHair(_uint iTailHairNum);
	HRESULT				Change_BackHair(_uint iBackHairNum);
	HRESULT				Change_SideHair(_uint iSideHairNum);
	void				Change_HairColor(_vec4 vHairColor);
	void				Change_BodyColor(_vec4 vBodyColor) { m_vBodyColor = vBodyColor; }
	void				Change_Brow(_uint iBroNum);
	void				Change_Eye(_uint iEyeNum);

	void				Update_Parts(const _double& dTimeDelta, _vec3* pPos, _vec3* pAngle, _vec3* pScale, _uint iAnimationNum);
	void				LateUpdate_Parts(const _double& dTimeDelta);
public:
	HRESULT				Ready_GameObject(SEX eSexType, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual	_int		Update_GameObject(const _double& dTimeDelta) override;
	virtual	_int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual	void		Render_Geometry(const _double& dTimeDelta) override;
	virtual void		Render_PostEffect(const _double& dTimeDelta) override;
private:
	HRESULT				Clone_Component(SEX eSexType);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

private:
	CTop*				m_pTop;
	CPants*				m_pPants;
	CFoot*				m_pFoot;
	CHands*				m_pHands;
	CFrontHair*			m_pFrontHair;
	CTailHair*			m_pTailHair;
	CBackHair*			m_pBackHair;
	CSideHair*			m_pSideHair;
	_uint				m_iAnimationNum;

	_bool				m_bIsHead; //¾ó±¼·»´õÇÏ°íÀÖÀ¸¸é true, ¸ö ·»´õÇÏ°íÀÖÀ¸¸é false
	Engine::CTexture*	m_pTextureCom[TEXTURE_CHANGE_END]; //´«, ´«½ç

	_vec4				m_vBodyColor; // ¸ö»ö±ò
	_uint				m_iBrowNum; // ÅØ½ºÃÄÄÄÆ÷³ÍÆ®¿¡¼­ ¾µ ´«½ç¹øÈ£
	_vec2				m_vBrowUV; // ÇØ´ç ´« UV
	_uint				m_iEyeNum; //ÅØ½ºÃÄÄÄÆ÷³ÍÆ®¿¡¼­ ¾µ ´« ¹øÈ£

	Engine::CShader*	m_pNormalShaderCom;
	_bool				m_bRenderNon; //true¸é NonAlpha¿¡¼­ Ãâ·Â , false¸é Alpha¿¡¼­ Ãâ·Â

	//Render¼ø¼­
	//¸Ó¸®(ÆäÀÌ½º, ´«½ç, ´« ¿ÞÂÊ(³»°¡ ÁÖÀÎ°øÀÏ¶§ÀÇ ¿ÞÂÊ), ´« ¿À¸¥ÂÊ)
	//¸ö(¸ö, ¼Ó¿Ê)
public:
	static	CBody*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SEX eSexType, _vec3* pPos, _vec3* pAngle, _vec3* pScale);
	virtual void	Free();
};

