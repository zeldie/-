#include "Shader_Base.hpp"
texture			g_ShadowTexture;

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
};

matrix g_matLightView;
matrix g_matLightProj;

struct VS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float3		vNormal : NORMAL;
	float3		vTangent : TANGENT;
	float3		vBiNormal : BINORMAL;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float3		vNormal : TEXCOORD1;
	float3		vTangent : TEXCOORD2;
	float3		vBiNormal : TEXCOORD3;
	vector		vLightProjPos : TEXCOORD4;	//Light¿¡¼­
};


VS_OUT VS_SHADOW(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);


	matrix matLightWV, matLightWVP;
	matLightWV = mul(g_matWorld, g_matLightView);
	matLightWVP = mul(matLightWV, g_matLightProj);

	Out.vLightProjPos = mul(vector(In.vPosition.xyz, 1.f), matLightWVP);
	Out.vTexUV = In.vTexUV;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.vNormal = normalize(worldNormal);
	float3 worldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	Out.vTangent = normalize(worldTangent);
	float3 worldBinormal = mul(In.vBiNormal, (float3x3)g_matWorld);
	Out.vBiNormal = normalize(worldBinormal);
	return Out;
}

struct	PS_IN
{
	float2			vTexUV : TEXCOORD0;
	float3			vNormal : TEXCOORD1;
	float3			vTangent : TEXCOORD2;
	float3			vBiNormal : TEXCOORD3;
	vector			vLightProjPos : TEXCOORD4;
};


struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_SHADOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	
	return Out;
}

technique Default_Device
{
	pass Shadow
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_SHADOW();
	}


};