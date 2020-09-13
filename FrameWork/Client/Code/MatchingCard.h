#pragma once
#ifndef __CMatchingCard_h__
#define __CMatchingCard_h__

#include "BaseObject.h"
#include "ScreenTex.h"
#include "Texture.h"

class CMatchingCard : public CBaseObject
{
public:
	enum CARDNUMBER
	{
		NUM_BACK,
		NUM_BACK_SELECT,
		NUM_1_A,
		NUM_1_B,
		NUM_2_A,
		NUM_2_B,
		NUM_3_A,
		NUM_3_B,
		NUM_4_A,
		NUM_4_B,
		NUM_5_A,
		NUM_5_B,
		NUM_6_A,
		NUM_6_B,
		NUM_7_A,
		NUM_7_B,
		NUM_8_A,
		NUM_8_B,
		NUM_9_A,
		NUM_9_B,
		NUM_10_A,
		NUM_10_B,
		NUM_UNKNOWN,
		NUM_END
	};
public:
	explicit CMatchingCard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMatchingCard();

public:
	//Setter
	void	Set_Choose(_bool bBool) { m_bChoose = bBool; }

	void	Set_Open(_bool bOpne) { m_bOpen = bOpne; }
	void	Set_CardNum(CARDNUMBER eNum) { m_eCardNum = eNum; }
	void	Set_CardState(CARDNUMBER eNum) { m_eCardState = eNum; }
	void	Set_Cheat(_bool bCheat) { m_bCheat = bCheat; }
	void	Set_Delay(_bool bDelay) { m_bDelay = bDelay; }
	void	Set_Index(_uint iIndex) { m_iIndex = iIndex; }

	CARDNUMBER		Get_CardNum() { return m_eCardNum; }
	_uint			Get_Index() { return m_iIndex; }

public:
	HRESULT				Ready_GameObject(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ );
	virtual _int		Update_GameObject(const _double& dTimeDelta) override;
	virtual _int		LateUpdate_GameObject(const _double& dTimeDelta) override;
	virtual void		Render_Geometry(const _double& dTimeDelta) override;

private:
	HRESULT				Clone_Component(_float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ);
	HRESULT				Setup_ShaderProps(LPD3DXEFFECT& pEffect);

public:
	Engine::CScreenTex*		m_pScreenTexBufferCom;
	Engine::CTexture*		m_pTextureCom;
	_bool					m_bChoose;		//지금 마우스가 올라가있는지
	_bool					m_bOpen;	// 카드가 오픈된 상태인지
	CARDNUMBER				m_eCardNum;	// 카드번호, 디퓨즈텍스쳐 이름값으로도 쓰임.
	CARDNUMBER				m_eCardState; // 카드의 텍스쳐, 앞,뒤
	_bool					m_bDelay;
	_bool					m_bCheat; // 치트키, 카드 다보여주는거
	_uint					m_iIndex; // 매니저에서의 인덱스

public:
	static CMatchingCard*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fX, _float fY, _float fSizeX, _float fSizeY, _float fViewZ =0.f);
	virtual void		Free();
};

#endif