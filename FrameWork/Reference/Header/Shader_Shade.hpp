#include "Shader_Base.hpp"

texture g_DepthTexture;

sampler DepthSampler = sampler_state // 노말 텍스쳐에 필터링은 절대 해선 안된다.
{
	texture = g_DepthTexture;
};

vector	g_vLightDir;
vector	g_vLightDiffuse;
vector	g_vLightAmbient;

vector	g_vMtrlDiffuse = (vector)1.f;
vector	g_vMtrlAmbient = (vector)1.f;

vector	g_vCamPos;
float	g_fPower;

matrix	g_matProjInv;
matrix	g_matViewInv;

vector	g_vLightPos;
float	g_fRange;

float	g_fView;
struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
};

PS_OUT		PS_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	float ndotL = saturate(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal))) * 0.5f + 0.5f;
	//ndotL = ceil(ndotL * 5.f) * 0.2f;
	ndotL = pow(ndotL, 3);

	//halfLambert = ceil(halfLambert * 5.f) * 0.2f;
	//halfLambert = floor(halfLambert * 5) * (1 / 5);

	//Out.vShade = g_vLightDiffuse * saturate(ndotL + g_vLightAmbient * g_vMtrlAmbient);
	//Out.vShade = g_vLightDiffuse * saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));
	Out.vShade = ndotL * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	Out.vShade = ceil(Out.vShade * 2.f) * 0.5f;
	//Out.vShade *= 5.f;
	Out.vShade.a = 1.f;

	vector	vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);

	vector	vDepth = tex2D(DepthSampler, In.vTexUV);

	float	fViewZ = vDepth.y * g_fView;

	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector	vLook = normalize(g_vCamPos - vPosition);

	/*Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower);
	Out.vSpecular.a = 0.f;*/

	return Out;
}


PS_OUT		PS_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	vector	vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);
	vector	vDepth = tex2D(DepthSampler, In.vTexUV);

	float	fViewZ = vDepth.y * g_fView;

	vector vPosition; // 텍스쳐 좌표에서 투영 좌표로 변환

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);


	vector	vLightDir = vPosition - g_vLightPos;
	float	fDistance = length(vLightDir);

	float	fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);

	Out.vShade = (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	//Out.vShade = ceil(Out.vShade * 4.f) * 0.25f;
	Out.vShade.a = 1.f;

	//vector	vLook = normalize(g_vCamPos - vPosition);
	//Out.vSpecular = pow(saturate(dot(vLook, vReflect)), g_fPower) * fAtt;;
	//Out.vSpecular.a = 0.f;

	return Out;
}

technique		Default_Device
{
	// 기능의 캡슐화
	pass Directional
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_DIRECTIONAL();
	}

	pass	Point
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		zwriteenable = false;
		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_POINT();
	}
};