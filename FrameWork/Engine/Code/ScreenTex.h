#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CScreenTex : public CVIBuffer
{
public:
	enum CUSTOMIZE{POSX,POSY, SIZEX, SIZEY};

private:
	explicit CScreenTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScreenTex(const CScreenTex& rhs);
	virtual ~CScreenTex();

public:
	//Getter
	_vec3& Get_vStartPos() { return m_vStartPos; }
	_vec2& Get_vSize() { return m_vSize; }

public:
	virtual HRESULT Ready_Prototype(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY, _float fViewZ);
	virtual HRESULT Ready_Buffer();

public:
	//UI 텍스처 UV조절 함수 -> 희정
	void VertexXControl_LeftDir(_float fSizeX); //가로
	void VertexXControl_RightDir(_float fSizeX); //가로
	void VertexYControl_UpDir(_float fSizeY); //세로 아래->위
	void VertexYControl_DownDir(_float fSizeY); //세로 위->아래
	void VertexXYControl(_float fSizeX, _float fSizeY); //전체
	void VertexOriginControl(); //복원
	
	//UI 텍스처 위치 조절 ->희정
	void VertexPositionXControl(_float fPos);
	void VertexPositionYControl(_float fPos);

	void VertexPositionXControl2(_float fPos);
	void VertexPositionYControl2(_float fPos);

	void VertexPositionXYControl2(_vec2 vPos); //->고정하는거

	//UI HP Z위치 조절(몬스터 HP에서만 사용가능할듯) -> 희정
	void VertexZControl();

	//버퍼 사이즈 조절 - 인균
	void Resize_Buffer(_float fX, _float fY);

	//마우스 위치 반환 / 주변에 있는지 검사(vector2 내적)
	_bool CheckAroundMouse(HWND hWnd, _vec3* pMousePos=nullptr, _vec3 vSizeRatio=_vec3(0.f,0.f,0.f));

public:
	//UI위치 조절용 -> 희정
	void VertexCustomizeTest(const _double& dTimeDelta, _float fSpeed, CUSTOMIZE CustomizeType);

private:
	//UI위치 조절용 -> 희정
	_vec3 m_vStartPos;
	_vec2 m_vSize;	

public:
	static CScreenTex*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY, _float fViewZ=0.f);
	virtual CComponent* Clone();
	virtual void Free();
};

END

