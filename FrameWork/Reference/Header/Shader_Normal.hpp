#include "Shader_Base.hpp"

texture				g_MaskingTexture;

sampler				MaskingSampler = sampler_state
{
	texture = g_MaskingTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
AddressU = Clamp;
AddressV = Clamp;
};

texture			g_DissolveTexture;

sampler DissolveSampler = sampler_state
{
	texture = g_DissolveTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_DissolveEdgeTexture;

sampler DissolveEdgeSampler = sampler_state
{
	texture = g_DissolveEdgeTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};
////////////////////////////////////
// x == ������ġ, y == ��Ʋ�󽺰� , z == UV�ִϸ��̼ǰ� , w == �Ⱦ���
vector				g_vUV_U;
vector				g_vUV_V;


vector				g_vRGBA;
vector				g_UseMasking; // ����ŷ�� ����ҰŸ� setvector���� 0,0,0,0 �Ѱ��ְ� ��� ���ҰŸ� 1,1,1,1 �Ѱ����

float				g_fDiminishColor;

struct VS_IN
{
	vector		vPosition	: POSITION;		// semantic
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
};

// ���ؽ� ���̵�

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	return Out;
}

float2 ChangeUV;
VS_OUT	VS_CHANGEUV(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV +ChangeUV;
	Out.vProjPos = Out.vPosition;
	return Out;
}

struct	PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);		// 2D �ؽ�ó�κ��� UV���� �����ϴ� �Լ� ,��ȯŸ���� VECTOR Ÿ��
	
	return Out;
}

PS_OUT PS_SIWON_UV(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	// UV �ִϸ��̼�
	In.vTexUV.x *= g_vUV_U.y;
	In.vTexUV.x += g_vUV_U.x + g_vUV_U.z;
	In.vTexUV.y *= g_vUV_V.y;
	In.vTexUV.y += g_vUV_V.x + g_vUV_V.z;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);		// 2D �ؽ�ó�κ��� UV���� �����ϴ� �Լ� ,��ȯŸ���� VECTOR Ÿ��

	return Out;
}

PS_OUT PS_SIWON_UV_RGB(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	// UV �ִϸ��̼�
	In.vTexUV.x *= g_vUV_U.y;
	In.vTexUV.x += g_vUV_U.x + g_vUV_U.z;
	In.vTexUV.y *= g_vUV_V.y;
	In.vTexUV.y += g_vUV_V.x + g_vUV_V.z;

	vector vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor = vColor * g_vRGBA;

	return Out;
}

PS_OUT PS_UI_COLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(DiffuseSampler, In.vTexUV);		// 2D �ؽ�ó�κ��� UV���� �����ϴ� �Լ� ,��ȯŸ���� VECTOR Ÿ��
	Out.vColor = vColor * g_vRGBA;

	return Out;
}

PS_OUT PS_UI_UVCONTROL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	In.vTexUV.x = (In.vTexUV.x* g_vUV_U.y) + g_vUV_U.x;
	In.vTexUV.y = (In.vTexUV.y* g_vUV_V.y) + g_vUV_V.x;

	vector vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor = vColor * g_vRGBA;

	return Out;
}

//��������, ����ŷ �Ѵ� UV �ȿ�����
PS_OUT PS_MASKING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vOriColor = tex2D(DiffuseSampler, In.vTexUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	Out.vColor = vOriColor * vMaskingColor;

	return Out;
}

//���������� UV�� ������, ����ŷ UV �ȿ�����
PS_OUT PS_UV_MASKING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vMaskingUV = In.vTexUV ;
	// UV �ִϸ��̼�
	vMaskingUV.x *= g_vUV_U.y;
	vMaskingUV.x += g_vUV_U.x + g_vUV_U.z;
	vMaskingUV.y *= g_vUV_V.y;
	vMaskingUV.y += g_vUV_V.x + g_vUV_V.z;

	vector vOriColor = tex2D(DiffuseSampler, vMaskingUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	Out.vColor = vOriColor * vMaskingColor;

	return Out;
}

//���������� UV�� ������, ����ŷ UV �ȿ�����
PS_OUT PS_UV_MASKING_DISAPPEAR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vMaskingUV = In.vTexUV;
	// UV �ִϸ��̼�
	vMaskingUV.x *= g_vUV_U.y;
	vMaskingUV.x += g_vUV_U.x + g_vUV_U.z;
	vMaskingUV.y *= g_vUV_V.y;
	vMaskingUV.y += g_vUV_V.x + g_vUV_V.z;

	vector vOriColor = tex2D(DiffuseSampler, vMaskingUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	Out.vColor = vOriColor * vMaskingColor * g_fDiminishColor;

	return Out;
}

PS_OUT PS_UV_MASKING_RGB(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vMaskingUV = In.vTexUV;
	// UV �ִϸ��̼�
	vMaskingUV.x *= g_vUV_U.y;
	vMaskingUV.x += g_vUV_U.x + g_vUV_U.z;
	vMaskingUV.y *= g_vUV_V.y;
	vMaskingUV.y += g_vUV_V.x + g_vUV_V.z;

	vector vOriColor = tex2D(DiffuseSampler, vMaskingUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	Out.vColor = vOriColor * vMaskingColor * g_vRGBA;

	return Out;
}

//�ؽ��� ���� ��
PS_OUT PS_ADD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vFirstColor = tex2D(DiffuseSampler, In.vTexUV);
	vector vSecondColor = tex2D(MaskingSampler, In.vTexUV);
	vector vAddColor = vFirstColor + vSecondColor;
	Out.vColor = vAddColor;
	return Out;
}

//�ؽ��� ���� ��
PS_OUT PS_DISSAPEAR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = tex2D(DiffuseSampler, In.vTexUV);
	vColor *= g_vRGBA;
	Out.vColor = vColor;
	return Out;
}

//���������� UV�� ������, ����ŷ UV �ȿ�����, ������
float fTime;
PS_OUT PS_UV_MASKING_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vMaskingUV = In.vTexUV;
	// UV �ִϸ��̼�
	vMaskingUV.x *= g_vUV_U.y;
	vMaskingUV.x += g_vUV_U.x + g_vUV_U.z;
	vMaskingUV.y *= g_vUV_V.y;
	vMaskingUV.y += g_vUV_V.x + g_vUV_V.z;

	vector vOriColor = tex2D(DiffuseSampler, vMaskingUV);
	vector vDissolve = tex2D(DissolveSampler, In.vTexUV);
	vector ClipAmount = vDissolve - vector(fTime, fTime, fTime, fTime);
	clip(ClipAmount.r < 0.f ? -1.f : 1.f);
	if (ClipAmount.r < 0.04f && fTime > 0.f)
	{
		vector vEdgeColor = tex2D(DissolveEdgeSampler, float2(ClipAmount.r * (1 / 0.04f), 0.f));
		vOriColor += vEdgeColor;
	}
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	if(ClipAmount.r > 0.05f)
		Out.vColor = vOriColor * vMaskingColor;

	return Out;
}

technique		Default_Device
{
	// ����� ĸ��ȭ
	pass
{
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
}

pass		Alphablend
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;	

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
}

pass		AlphaTest
{
	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xa0;
	
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
}
//3
pass		SiwonUV
{
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_SIWON_UV();
}
//4 �⺻ �ؽ��� ����Ʈ : ���ĺ��� 
pass		SiwonUVAlpha
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_SIWON_UV();
}

//5
pass		 UI_COLOR //->�������� (Color ����)
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_UI_COLOR();
}

//6
pass		UI_UVCONTROL //->�������� (UV + Color����)
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_UI_UVCONTROL();
}
//7
pass		SiwonUVAlphaTest
{
	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xc0;


	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_SIWON_UV();
}

//8
pass Masking
{
	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xc0;


vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MASKING();
}

//9 // ����ŷ + UV ���� +���ĺ���
pass UV_Masking
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;
	//zwriteEnable = false;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_UV_MASKING();
}

//10 ->�������ڽ�
pass		 UI_COLOR_DAMAGEBOX //->�������� (Color ����)
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	zenable = false;
	zwriteenable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_UI_COLOR();
}

//11 ->tired����
pass		AlphaTest_Tired
{
	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xc0;
	
	zenable = false;
	zwriteenable = false;	
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
}

//12 ->ultimate����
pass		AlphaTest_ultimate
{
	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xc0;

	cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
}
//13 �ؽ��� �ΰ��� ��
pass texture_Add
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;
vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_ADD();
}
//14 �⺻ �ؽ��� ����Ʈ : ���ĺ���  + RGB�� �߰�
pass		SiwonUVAlpha
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = one;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_SIWON_UV_RGB();
}
//15 // ����ŷ + UV ���� +���ĺ��� + RGB
pass UV_Masking
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = one;
cullmode = none;
//zwriteEnable = false;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_UV_MASKING_RGB();
}
//16 �⺻�ε� ������ ���ķ� �ձ�
pass BLACK_ALPHA_REMOVE
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}
//17 // ����ŷ + UV ���� +���ĺ���  , 9���н��� ���Ұ� �ִ°�
pass UV_Masking
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = one;
cullmode = none;
//zwriteEnable = false;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_UV_MASKING_DISAPPEAR();
}
//18 �⺻ �ؽ��� ����Ʈ : ���ĺ��� +������ �÷� ���� + RGB�� �߰�
pass		TEXTUREEFFECT_DUST_TS
{
	alphablendenable = true;
srcblend = srccolor;
destblend = one;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_SIWON_UV_RGB();
}


//19 // ����ŷ + UV ���� +���ĺ���
pass UV_Masking
{
	alphablendenable = true;
srcblend = srccolor;
destblend = one;
cullmode = none;
//zwriteEnable = false;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_UV_MASKING();
}

//20 ������ �����
pass DIssapear
{
	alphablendenable = true;
srcblend = srccolor;
destblend = one;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_DISSAPEAR();
}

//21 ���� ���� - ���ĺ��� + �ø�� ��
pass		AlphablendTree
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}
//22 ����ŷ, ���ĺ���
pass		AlphablendTree
{
	alphablendenable = true;
	srcblend = srccolor;
	destblend = one;
	cullmode = none;
	zwriteenable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MASKING();
}
//23 
pass ForKarmaOverHead // ���� �߰� : �������ձ�+ RGBA ����
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_UI_COLOR();
}
//24
pass		ForAICondition //���� �߰� : �⺻���ĺ��� + zenable false (AI�� �Ӹ���UI)
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	zenable = false;
	zwriteenable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
}
//25
pass		 UI_alphatest_COLOR //->�������� �����׽�Ʈ(Color ����)
{
	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xa0;

	
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_UI_COLOR();
}
//26
pass		Alphablend_Color
{
	alphablendenable = true;
srcblend = srcColor;
destblend = invsrcColor;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}

//27 ����ŷ + UV ���� +���ĺ��� + ������
pass UV_Masking
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = one;
cullmode = none;
//zwriteEnable = false;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_UV_MASKING_DISSOLVE();
}
//28
pass ChangeUV
{
	alphatestenable = true;
alphafunc = greater;
alpharef = 0xa0;

vertexshader = compile vs_3_0 VS_CHANGEUV();
pixelshader = compile ps_3_0 PS_MAIN();
}
};