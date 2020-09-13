#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexNormalTexture
	{
		_vec3			vPos;
		_vec3			vNormal;
		_vec2		 	vTexUV;

	}VTXNORTEX;

	const _ulong		FVF_NORTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef	struct tagUltimateTexture //����
	{
		float fX, fY, fZ;
		float fU, fV;
		float fU2, fV2;

		//_vec3			vPos;
		//_vec2		 	vTexUV1;
		//_vec2		 	vTexUV2;

	}ULTIMATETEX;

	const _ulong		FVF_ULTIMATE = D3DFVF_XYZ | D3DFVF_TEX1 ; //����


	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;


	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix				CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppDiffuseTexture;  // �ؽ��� �迭
		LPDIRECT3DTEXTURE9*		ppNormalTexture;  // �ؽ��� �迭
		LPDIRECT3DTEXTURE9*		ppSpecularTexture;  // �ؽ��� �迭
		LPDIRECT3DTEXTURE9*		ppEmmisiveTexture;  // �ؽ��� �迭

		LPD3DXMESH				pOriMesh;	// ������ ���¸� ������ �ִ� �޽� ��ü(��ǥ �Һ�)

		_ulong					dwNumBones; // ���� ����

		_matrix**				ppFrameCombinedTransformationMatrix;
		// ������ ���ϰ� �ִ� ���� ����� �ּҰ��� �����ϱ� ���� ���� ������

		_matrix*				pFrameOffsetMatrix; // �ִϸ��̼� ������ ������ ������ ���
		_matrix*				pRenderingMatrix;	// ���������� �������� �����ϱ� ���� �ϼ� ������ ���



	}D3DXMESHCONTAINER_DERIVED;

	//typedef struct tagTexturePath
	//{
	//	wstring wstrPath = L"";
	//	wstring wstrObjKey = L"";
	//	wstring wstrStateKey = L"";
	//	int		iCount = 0;
	//}IMAGEPATH;

	typedef struct tagParticleTex
	{
		_vec3			vPos;
		D3DCOLOR		Color;
		_float			fSize;	//��ƼŬ ������ ������ ������ ���ؽ����̴��� �ؾ��Ѵ�.
	}PARTICLETEX;

	const _ulong		FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagParticleAttribute
	{
		tagParticleAttribute()
		{
			fSpeed = 0.f;
			fAcceleration = 0.f;
			fSize = 0.f;
			dLifeTime = 0.;
			dAge = 0.;
			bisDead = true;
		}

		_vec3			vPos;			//��ġ
		_vec3			vDir;			//����
		_float			fSpeed;			//�ӵ�
		_float			fAcceleration;	//���ӵ�
		_float			fSize;			//ũ��
		_double			dLifeTime;		//�ִ����ӽð�
		_double			dAge;			//���ӵ� �ð�
		bool			bisDead;
		D3DXCOLOR		Color;
		D3DXCOLOR		ColorFade;

	}PARTICLE_INFO;

	//����Ʈ
	typedef struct tagEffectUVInfo
	{
		tagEffectUVInfo()
			:iTextureName(0)
			, vUVPos(_vec2(0.f, 0.f))
			, vUVAtlas(_vec2(0.f, 0.f))
			, vUVMovePos(_vec2(0.f, 0.f))
			, vUVMovePosSum(_vec2(0.f, 0.f))
			, iPass(0)
			, dbFrame(0.)
			, dbMaxFrame(0.)
		{

		}
		_uint			iTextureName;
		_vec2			vUVPos;
		_vec2			vUVAtlas;
		_vec2			vUVMovePos;
		_vec2			vUVMovePosSum;
		_uint			iPass;
		_double			dbFrame;
		_double			dbMaxFrame;
	}EFFECT_UV_INFO;

	typedef struct tagCamInfo
	{
		//_matrix matSpline;
		_vec3 vSpline1;		// v1
		_vec3 vHead;		// v2
		_vec3 vTail;		// v3
		_vec3 vSpline2;		// v4

		_float vSpeed;
	}CAM_INFO;
}
#endif // Engine_Struct_h__
