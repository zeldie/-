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
	//UI �ؽ�ó UV���� �Լ� -> ����
	void VertexXControl_LeftDir(_float fSizeX); //����
	void VertexXControl_RightDir(_float fSizeX); //����
	void VertexYControl_UpDir(_float fSizeY); //���� �Ʒ�->��
	void VertexYControl_DownDir(_float fSizeY); //���� ��->�Ʒ�
	void VertexXYControl(_float fSizeX, _float fSizeY); //��ü
	void VertexOriginControl(); //����
	
	//UI �ؽ�ó ��ġ ���� ->����
	void VertexPositionXControl(_float fPos);
	void VertexPositionYControl(_float fPos);

	void VertexPositionXControl2(_float fPos);
	void VertexPositionYControl2(_float fPos);

	void VertexPositionXYControl2(_vec2 vPos); //->�����ϴ°�

	//UI HP Z��ġ ����(���� HP������ ��밡���ҵ�) -> ����
	void VertexZControl();

	//���� ������ ���� - �α�
	void Resize_Buffer(_float fX, _float fY);

	//���콺 ��ġ ��ȯ / �ֺ��� �ִ��� �˻�(vector2 ����)
	_bool CheckAroundMouse(HWND hWnd, _vec3* pMousePos=nullptr, _vec3 vSizeRatio=_vec3(0.f,0.f,0.f));

public:
	//UI��ġ ������ -> ����
	void VertexCustomizeTest(const _double& dTimeDelta, _float fSpeed, CUSTOMIZE CustomizeType);

private:
	//UI��ġ ������ -> ����
	_vec3 m_vStartPos;
	_vec2 m_vSize;	

public:
	static CScreenTex*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY, _float fViewZ=0.f);
	virtual CComponent* Clone();
	virtual void Free();
};

END

