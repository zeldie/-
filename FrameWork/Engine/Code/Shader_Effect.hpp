#include "Shader_Base.hpp"

texture			g_MaskingTexture1;

sampler MaskingSampler1 = sampler_state
{
	texture = g_MaskingTexture1;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_MaskingTexture2;

sampler MaskingSampler2 = sampler_state
{
	texture = g_MaskingTexture2;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_MaskingTexture3;

sampler MaskingSampler3 = sampler_state
{
	texture = g_MaskingTexture3;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_DistortionTexture;

sampler DistortionSampler = sampler_state
{
	texture = g_DistortionTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_AddTexture;

sampler AddSampler = sampler_state
{
	texture = g_AddTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

// x == 시작위치, y == 아틀라스값 , z == UV애니메이션값 , w == 안쓴다
struct Props
{
	float3 vMainUV_U;
	float3 vMainUV_V;
	float3 vMask1UV_U;
	float3 vMask1UV_V;
	float3 vMask2UV_U;
	float3 vMask2UV_V;
	vector vUseMask1;
	vector vUseMask2;
	vector vUseDistortion;
	float2 vMainUVOffset;
	float2 vMask1UVOffset;
	float2 vMask2UVOffset;
	float2 vMainOffsetSpeed;
	float2 vMask1OffsetSpeed;
	float2 vMask2OffsetSpeed;
	float2 vUVMovePosSum;
	float2 vUVMovePosSum2;
	float2 vUVMovePosSum3;
	vector vDiminishRate;
	vector vUseAdd;
};

Props		g_EffectProps;
float		g_fPower;

vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightSpecular;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse;
vector		g_vMtrlSpecular;
vector		g_vMtrlAmbient;

vector		g_vCamPos;

float2		g_UVOffset;

float2		g_vInvAtlas;

struct VS_EFFECT_IN
{
	vector		vPosition		: POSITION;
	float2		vTexUV			: TEXCOORD0;
	float3		vNormal			: NORMAL;
};

struct VS_EFFECT_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vShade : COLOR0;
};


// 버텍스 쉐이딩

VS_EFFECT_OUT	VS_EFFECT_MAIN(VS_EFFECT_IN In)
{
	VS_EFFECT_OUT		Out = (VS_EFFECT_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct	PS_EFFECT_IN
{
	float2		vTexUV : TEXCOORD0;
	vector		vShade : COLOR0;
};

struct PS_EFFECT_OUT
{
	vector		vColor	: COLOR0;
};

vector vChangeEffectColor;
PS_EFFECT_OUT PS_EFFECT_MAIN(PS_EFFECT_IN In)
{

	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	// UV 애니메이션
	float2 vTexMainUV = In.vTexUV;
	vTexMainUV.x *= g_vInvAtlas.x;
	vTexMainUV.x += g_EffectProps.vMainUV_U.x + g_EffectProps.vUVMovePosSum.x;
	vTexMainUV.y *= g_vInvAtlas.y;
	vTexMainUV.y += g_EffectProps.vMainUV_V.x + g_EffectProps.vUVMovePosSum.y;

	vector vMainColor = tex2D(DiffuseSampler, vTexMainUV);
	vMainColor *= vChangeEffectColor;
	//vMainColor = ceil(vMainColor * 4.f) * 0.25f;
	float2 vTexMasking1UV = In.vTexUV;
	vTexMasking1UV.x *= g_vInvAtlas.x;
	vTexMasking1UV.x += g_EffectProps.vMask1UV_U.x + g_EffectProps.vUVMovePosSum2.x;
	vTexMasking1UV.y *= g_vInvAtlas.y;
	vTexMasking1UV.y += g_EffectProps.vMask1UV_V.x + g_EffectProps.vUVMovePosSum2.y;

	vector vMasking1Color = tex2D(MaskingSampler1, vTexMasking1UV);
	vMasking1Color += g_EffectProps.vUseMask1;

	float2 vTexMasking2UV = In.vTexUV;
	vTexMasking2UV.x *= g_vInvAtlas.x;
	vTexMasking2UV.x += g_EffectProps.vMask2UV_U.x + g_EffectProps.vUVMovePosSum3.x;
	vTexMasking2UV.y *= g_vInvAtlas.y;
	vTexMasking2UV.y += g_EffectProps.vMask2UV_V.x + g_EffectProps.vUVMovePosSum3.y;

	vector vMasking2Color = tex2D(MaskingSampler2, vTexMasking2UV);
	vMasking2Color += g_EffectProps.vUseMask2;

	vector vAddColor = tex2D(AddSampler, In.vTexUV);
	vAddColor *= g_EffectProps.vUseAdd;
	Out.vColor = vMainColor * vMasking1Color * vMasking2Color * g_EffectProps.vDiminishRate + vAddColor;

	return Out;
}

//트레일
PS_EFFECT_OUT PS_EFFECT_TRAIL(PS_EFFECT_IN In)
{

	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	// UV 애니메이션
	float2 vTexMainUV = In.vTexUV;
	vTexMainUV.x *= g_EffectProps.vMainUV_U.y;
	vTexMainUV.x += g_EffectProps.vMainUV_U.x + g_EffectProps.vMainUVOffset.x;
	vTexMainUV.y *= g_EffectProps.vMainUV_V.y;
	vTexMainUV.y += g_EffectProps.vMainUV_V.x + g_EffectProps.vMainUVOffset.y;

	vector vMainColor = tex2D(DiffuseSampler, vTexMainUV);
	vMainColor *= vChangeEffectColor;
	//vMainColor = ceil(vMainColor * 4.f) * 0.25f;
	if (vTexMainUV.x > g_EffectProps.vMainUV_U.y + g_EffectProps.vMainUV_U.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMainUV.x < g_EffectProps.vMainUV_U.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMainUV.y > g_EffectProps.vMainUV_V.y + g_EffectProps.vMainUV_V.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMainUV.y < g_EffectProps.vMainUV_V.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);

	float2 vTexMasking1UV = In.vTexUV;
	vTexMasking1UV.x *= g_EffectProps.vMask1UV_U.y;
	vTexMasking1UV.x += g_EffectProps.vMask1UV_U.x+ g_EffectProps.vMask1UVOffset.x;
	vTexMasking1UV.y *= g_EffectProps.vMask1UV_V.y;
	vTexMasking1UV.y += g_EffectProps.vMask1UV_V.x+ g_EffectProps.vMask1UVOffset.y;

	vector vMasking1Color = tex2D(MaskingSampler1, vTexMasking1UV);
	vMasking1Color += g_EffectProps.vUseMask1;
	if (vTexMasking1UV.x > g_EffectProps.vMask1UV_U.y + g_EffectProps.vMask1UV_U.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking1UV.x < g_EffectProps.vMask1UV_U.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking1UV.y > g_EffectProps.vMask1UV_V.y + g_EffectProps.vMask1UV_V.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking1UV.y < g_EffectProps.vMask1UV_V.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);

	float2 vTexMasking2UV = In.vTexUV;
	vTexMasking2UV.x *= g_EffectProps.vMask2UV_U.y;
	vTexMasking2UV.x += g_EffectProps.vMask2UV_U.x+ g_EffectProps.vMask2UVOffset.x;
	vTexMasking2UV.y *= g_EffectProps.vMask2UV_V.y;
	vTexMasking2UV.y += g_EffectProps.vMask2UV_V.x+ g_EffectProps.vMask2UVOffset.y;

	vector vMasking2Color = tex2D(MaskingSampler2, vTexMasking2UV);
	vMasking2Color += g_EffectProps.vUseMask2;
	if (vTexMasking2UV.x > g_EffectProps.vMask2UV_U.y + g_EffectProps.vMask2UV_U.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking2UV.x < g_EffectProps.vMask2UV_U.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking2UV.y > g_EffectProps.vMask2UV_V.y + g_EffectProps.vMask2UV_V.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking2UV.y < g_EffectProps.vMask2UV_V.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	Out.vColor = vMainColor * vMasking1Color * vMasking2Color * g_EffectProps.vDiminishRate;
	//Out.vColor = vMainColor;

	return Out;
}


//아틀라스 + 트레일
PS_EFFECT_OUT PS_EFFECT_ATLAS_TRAIL(PS_EFFECT_IN In)
{

	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	// UV 애니메이션
	float2 vTexMainUV = In.vTexUV;
	vTexMainUV.x *= g_vInvAtlas.x;
	vTexMainUV.x += g_EffectProps.vMainUV_U.x +g_EffectProps.vMainUVOffset.x;
	vTexMainUV.y *= g_vInvAtlas.y;
	vTexMainUV.y += g_EffectProps.vMainUV_V.x +g_EffectProps.vMainUVOffset.y;

	vector vMainColor = tex2D(DiffuseSampler, vTexMainUV);
	vMainColor *= vChangeEffectColor;
	//vMainColor = ceil(vMainColor * 4.f) * 0.25f;
	if (vTexMainUV.x > g_vInvAtlas.x + g_EffectProps.vMainUV_U.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMainUV.x < g_EffectProps.vMainUV_U.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMainUV.y > g_vInvAtlas.y + g_EffectProps.vMainUV_V.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMainUV.y < g_EffectProps.vMainUV_V.x)
		vMainColor = vector(0.f, 0.f, 0.f, 1.f);

	float2 vTexMasking1UV = In.vTexUV;
	vTexMasking1UV.x *= g_vInvAtlas.x;
	vTexMasking1UV.x += g_EffectProps.vMask1UV_U.x + g_EffectProps.vMask1UVOffset.x;
	vTexMasking1UV.y *= g_vInvAtlas.y;
	vTexMasking1UV.y += g_EffectProps.vMask1UV_V.x + g_EffectProps.vMask1UVOffset.y;

	vector vMasking1Color = tex2D(MaskingSampler1, vTexMasking1UV);
	vMasking1Color += g_EffectProps.vUseMask1;
	if (vTexMasking1UV.x > g_vInvAtlas.x + g_EffectProps.vMask1UV_U.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking1UV.x < g_EffectProps.vMask1UV_U.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking1UV.y > g_vInvAtlas.y + g_EffectProps.vMask1UV_V.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking1UV.y < g_EffectProps.vMask1UV_V.x)
		vMasking1Color = vector(0.f, 0.f, 0.f, 1.f);

	float2 vTexMasking2UV = In.vTexUV;
	vTexMasking2UV.x *= g_vInvAtlas.x;
	vTexMasking2UV.x += g_EffectProps.vMask2UV_U.x  + g_EffectProps.vMask2UVOffset.x;
	vTexMasking2UV.y *= g_vInvAtlas.y;
	vTexMasking2UV.y += g_EffectProps.vMask2UV_V.x  + g_EffectProps.vMask2UVOffset.y;

	vector vMasking2Color = tex2D(MaskingSampler2, vTexMasking2UV);
	vMasking2Color += g_EffectProps.vUseMask2;
	if (vTexMasking2UV.x > g_vInvAtlas.x + g_EffectProps.vMask2UV_U.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking2UV.x < g_EffectProps.vMask2UV_U.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking2UV.y > g_vInvAtlas.y + g_EffectProps.vMask2UV_V.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	if (vTexMasking2UV.y < g_EffectProps.vMask2UV_V.x)
		vMasking2Color = vector(0.f, 0.f, 0.f, 1.f);
	Out.vColor = vMainColor * vMasking1Color * vMasking2Color * g_EffectProps.vDiminishRate;
	//Out.vColor = vMainColor;

	return Out;
}

technique		Default_Device
{

	pass	Main //0
	{
		cullmode = none;

		vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
		pixelshader = compile ps_3_0 PS_EFFECT_MAIN();
	}

	pass	Aplhablend //1
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
		pixelshader = compile ps_3_0 PS_EFFECT_MAIN();
	}

	pass	AlphaTest //2
	{
		alphatestenable = true;
		Alphafunc = greater;
		alpharef = 0xb0;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
		pixelshader = compile ps_3_0 PS_EFFECT_MAIN();
	}

	pass	Aplhablend //3, 트레일
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;
		cullmode = none;
		zwriteEnable = false;

		vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
		pixelshader = compile ps_3_0 PS_EFFECT_TRAIL();
	}
	pass	Aplhablend //4 , 아틀라스 + 트레일
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;

	vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
	pixelshader = compile ps_3_0 PS_EFFECT_ATLAS_TRAIL();
	}
	pass	Aplhablend //5, UV 애니메이션
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;
	zwriteEnable = false;

	vertexshader = compile vs_3_0 VS_EFFECT_MAIN();
	pixelshader = compile ps_3_0 PS_EFFECT_MAIN();
	}
};