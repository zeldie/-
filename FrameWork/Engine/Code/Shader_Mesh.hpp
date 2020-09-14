#include "Shader_Base.hpp"

texture			g_ShadowTexture;

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;
	AddressU = Clamp;
	AddressV = Clamp;
};

texture			g_PreDepthTexture;

sampler PreDepthSampler = sampler_state
{
	texture = g_PreDepthTexture;
AddressU = Clamp;
AddressV = Clamp;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_CubeTexture;

sampler CubeSampler = sampler_state
{
	texture = g_CubeTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;

addressU = wrap;
addressV = wrap;
};

texture			g_DistortionTexture;

sampler DistortionSampler = sampler_state
{
	texture = g_DistortionTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_MaskingTexture;

sampler MaskingSampler = sampler_state
{
	texture = g_MaskingTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
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

texture			g_ShadowFilterTexture;

sampler ShadowFilterSampler = sampler_state
{
	texture = g_ShadowFilterTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

matrix g_matLightView;
matrix g_matLightProj;
matrix	g_matProjInv;
matrix	g_matViewInv;
matrix	g_matWorldInv;
// x == 시작위치, y == 아틀라스값 , z == UV애니메이션값 , w == 안쓴다
vector				g_vUV_U;	
vector				g_vUV_V;

float fFocalDistance; // DOF
float fFocalRange; // DOF

float g_fTerrainMipMap; //희정

vector vOcclusionColor; // GODRAY
float fInvCamFar; // DEPTH
float fShadowBias = -0.00000125f;
float3 g_vCamPos;
float3 g_vPosition;
struct VS_IN_FIRST
{
	vector		vPosition		: POSITION;
	float2		vTexUV			: TEXCOORD0;
	float3		vNormal			: NORMAL;
	float3		vTangent		: TANGENT;
	float3		vBiNormal		: BINORMAL;
};

struct VS_OUT_FIRST
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	float3		vNormal : TEXCOORD2;
	float3		vTangent : TEXCOORD3;
	float3		vBiNormal : TEXCOORD4;
	vector		vViewPos : TEXCOORD5; // Occlusion
	vector		vLightProjPos : TEXCOORD6;
	float		fBlur : TEXCOORD7;
};

struct VS_IN_SECOND
{
	vector		vPosition		: POSITION;
	float2		vTexUV			: TEXCOORD0;
	float3		vNormal			: NORMAL;
	float3		vTangent		: TANGENT;
	float3		vBiNormal		: BINORMAL;
};

struct VS_OUT_SECOND
{
	vector		vPosition	: POSITION;
	float2		vTexUV		: TEXCOORD0;
	float3		vNormal		: TEXCOORD1;
	float3		vTangent	: TEXCOORD2;
	float3		vBiNormal	: TEXCOORD3;
};

struct VS_SHADOW_IN
{
	vector vPosition : POSITION;
};

struct VS_SHADOW_OUT
{
	vector vPosition : POSITION;
	vector vLightProjPos : TEXCOORD0;
};

struct VS_DEPTH_IN
{
	vector vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_DEPTH_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

// 버텍스 쉐이딩

VS_OUT_FIRST	VS_MAIN(VS_IN_FIRST In)
{
	VS_OUT_FIRST		Out = (VS_OUT_FIRST)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vViewPos = mul(vector(In.vPosition.xyz, 1.f), matWV);

	matrix	matLightWV, matLightWVP;
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
	Out.fBlur = saturate(abs(Out.vViewPos.z - fFocalDistance) / fFocalRange);
	return Out;
}

VS_OUT_FIRST	VS_TERRAIN(VS_IN_FIRST In)
{
	VS_OUT_FIRST		Out = (VS_OUT_FIRST)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vViewPos = mul(vector(In.vPosition.xyz, 1.f), matWV);

	matrix	matLightWV, matLightWVP;
	matLightWV = mul(g_matWorld, g_matLightView);
	matLightWVP = mul(matLightWV, g_matLightProj);
	Out.vLightProjPos = mul(vector(In.vPosition.xyz, 1.f), matLightWVP);
	Out.vTexUV = In.vTexUV * g_fTerrainMipMap;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.vNormal = normalize(worldNormal);
	float3 worldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	Out.vTangent = normalize(worldTangent);
	float3 worldBinormal = mul(In.vBiNormal, (float3x3)g_matWorld);
	Out.vBiNormal = normalize(worldBinormal);
	Out.fBlur = saturate(abs(Out.vViewPos.z - fFocalDistance) / fFocalRange);
	return Out;
}

VS_OUT_FIRST	VS_TERRAIN_RUN(VS_IN_FIRST In)
{
	VS_OUT_FIRST		Out = (VS_OUT_FIRST)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vViewPos = mul(vector(In.vPosition.xyz, 1.f), matWV);

	matrix	matLightWV, matLightWVP;
	matLightWV = mul(g_matWorld, g_matLightView);
	matLightWVP = mul(matLightWV, g_matLightProj);
	Out.vLightProjPos = mul(vector(In.vPosition.xyz, 1.f), matLightWVP);
	Out.vTexUV = In.vTexUV * g_fTerrainMipMap;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.vNormal = normalize(worldNormal);
	float3 worldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	Out.vTangent = normalize(worldTangent);
	float3 worldBinormal = mul(In.vBiNormal, (float3x3)g_matWorld);
	Out.vBiNormal = normalize(worldBinormal);
	Out.fBlur = saturate(abs(Out.vViewPos.z - fFocalDistance) / fFocalRange);
	return Out;
}

VS_OUT_SECOND VS_SUB(VS_IN_SECOND In)
{
	VS_OUT_SECOND Out = (VS_OUT_SECOND)0;
	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.vNormal = normalize(worldNormal);
	float3 worldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	Out.vTangent = normalize(worldTangent);
	float3 worldBinormal = mul(In.vBiNormal, (float3x3)g_matWorld);
	Out.vBiNormal = normalize(worldBinormal);
	return Out;
}

VS_SHADOW_OUT VS_SHADOW(VS_SHADOW_IN In)
{
	VS_SHADOW_OUT Out = (VS_SHADOW_OUT)0;
	matrix lightViewMatrix = g_matLightView;

	Out.vPosition = mul(In.vPosition, g_matWorld);
	Out.vPosition = mul(Out.vPosition, lightViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_matLightProj);

	Out.vLightProjPos = Out.vPosition;
	return Out;
}

VS_DEPTH_OUT	VS_DEPTH(VS_DEPTH_IN In)
{
	VS_DEPTH_OUT		Out = (VS_DEPTH_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	return Out;
}
float2 ChangeUV;
VS_OUT_FIRST	VS_UVCHANGE(VS_IN_FIRST In)
{
	VS_OUT_FIRST		Out = (VS_OUT_FIRST)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vViewPos = mul(vector(In.vPosition.xyz, 1.f), matWV);

	matrix	matLightWV, matLightWVP;
	matLightWV = mul(g_matWorld, g_matLightView);
	matLightWVP = mul(matLightWV, g_matLightProj);
	Out.vLightProjPos = mul(vector(In.vPosition.xyz, 1.f), matLightWVP);
	Out.vTexUV = In.vTexUV + ChangeUV;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.vNormal = normalize(worldNormal);
	float3 worldTangent = mul(In.vTangent, (float3x3)g_matWorld);
	Out.vTangent = normalize(worldTangent);
	float3 worldBinormal = mul(In.vBiNormal, (float3x3)g_matWorld);
	Out.vBiNormal = normalize(worldBinormal);
	Out.fBlur = saturate(abs(Out.vViewPos.z - fFocalDistance) / fFocalRange);
	return Out;
}

struct	PS_IN_FIRST
{
	float2			vTexUV		  : TEXCOORD0;
	vector			vProjPos	  : TEXCOORD1;
	float3			vNormal		  : TEXCOORD2;
	float3			vTangent	  : TEXCOORD3;
	float3			vBiNormal	  : TEXCOORD4;
	vector			vViewPos      : TEXCOORD5;
	vector			vLightProjPos : TEXCOORD6;
	float			fBlur		  : TEXCOORD7;
};

struct PS_OUT_FIRST
{
	vector		vColor			  : COLOR0;
	vector		vNormal			  : COLOR1;
	vector		vDepth		      : COLOR2;
	vector      vEmmisive		  : COLOR3;
};

struct	PS_IN_SECOND
{
	float2			vTexUV : TEXCOORD0;
	float3			vNormal : TEXCOORD1;
	float3			vTangent : TEXCOORD2;
	float3			vBiNormal : TEXCOORD3;
};

struct PS_OUT_SECOND
{
	vector		vNormal		 : COLOR0;
	vector		vOcclusion	 : COLOR1;
};

struct PS_SHADOW_IN
{
	vector vLightProjPos : TEXCOORD0;
};

struct PS_SHADOW_OUT
{
	vector vDepth : COLOR0;
};

struct	PS_DEPTH_IN
{
	float2			vTexUV : TEXCOORD0;
	vector			vProjPos : TEXCOORD1;
};

struct PS_DEPTH_OUT
{
	vector		vDepth : COLOR0;
};
// 픽셀 쉐이더

vector vChangeColor;
PS_OUT_FIRST PS_MAIN(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	//알베도
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor *= vChangeColor;
	Out.vColor.a = In.fBlur;

	//노말
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN); 
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	//깊이 1. 카메라에서의 Proj깊이 2. ViewZ 보관, 3. 카메라에서의 View 깊이 4. Light에서의 Proj깊이
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * fInvCamFar, In.vViewPos.z * fInvCamFar, In.vLightProjPos.z / In.vLightProjPos.w);

	float fCurrentDepth = In.vLightProjPos.z / In.vLightProjPos.w;

	float2 UV = In.vLightProjPos.xy / In.vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
	float fShadowDepth = tex2D(ShadowSampler, UV).r;

	if (fCurrentDepth > fShadowDepth - fShadowBias)
	{
		Out.vColor *= 0.5f;
	}
	//발광
	Out.vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	
	return Out;
}

float3 g_vRimColor;
float2 g_vRimMinMax;
PS_OUT_FIRST PS_RIMLIGHT(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	//알베도
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor *= vChangeColor;


	//노말
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	//깊이 1. 카메라에서의 Proj깊이 2. ViewZ 보관, 3. 카메라에서의 View 깊이 4. Light에서의 Proj깊이
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * fInvCamFar, In.vViewPos.z * fInvCamFar, In.vLightProjPos.z / In.vLightProjPos.w);

	float fCurrentDepth = In.vLightProjPos.z / In.vLightProjPos.w;

	float2 UV = In.vLightProjPos.xy / In.vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
	float fShadowDepth = tex2D(ShadowSampler, UV).r;

	if (fCurrentDepth > fShadowDepth - fShadowBias)
	{
		Out.vColor.rgb *= 0.5f;
	}
	//발광
	Out.vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);

	//Rim Light
	float3 vCamDir = normalize(g_vCamPos - g_vPosition);
	float fRim = smoothstep(g_vRimMinMax.x, g_vRimMinMax.y, 1 - max(0, dot(normalize(worldNormal), vCamDir))) * 2.f;
	float3 fFinalRimColor = fRim * g_vRimColor;
	Out.vColor.rgb += fFinalRimColor;
	Out.vColor.a = In.fBlur;
	return Out;
}

vector vMaskingColor;
PS_OUT_FIRST PS_MAIN_COLOR(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	//알베도
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor *= vMaskingColor;

	//노말
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	//깊이 1. 카메라에서의 Proj깊이 2. ViewZ 보관, 3. 카메라에서의 View 깊이 4. Light에서의 Proj깊이
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * fInvCamFar, In.vViewPos.z * fInvCamFar, In.vLightProjPos.z / In.vLightProjPos.w);

	float fCurrentDepth = In.vLightProjPos.z / In.vLightProjPos.w;

	float2 UV = In.vLightProjPos.xy / In.vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
	float fShadowDepth = tex2D(ShadowSampler, UV).r;

	if (fCurrentDepth > fShadowDepth - fShadowBias)
	{
		Out.vColor.rgb *= 0.5f;
	}
	//발광
	Out.vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	Out.vColor.a = In.fBlur;

	return Out;
}

PS_OUT_FIRST PS_SIWON_ALPHA_UV(PS_IN_FIRST In)
{
	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;
	// UV 애니메이션
	In.vTexUV.x *= g_vUV_U.y;
	In.vTexUV.x += g_vUV_U.x + g_vUV_U.z;
	if (In.vTexUV.x >= 1.f)
		In.vTexUV = 1.f;
	In.vTexUV.y *= g_vUV_V.y;
	In.vTexUV.y += g_vUV_V.x + g_vUV_V.z;
	//알베도
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor *= vChangeColor;
	
																 //노말
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	//깊이 1. 카메라에서의 Proj깊이 2. ViewZ 보관, 3. 카메라에서의 View 깊이 4. Light에서의 Proj깊이
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * fInvCamFar, In.vViewPos.z * fInvCamFar, In.vLightProjPos.z / In.vLightProjPos.w);

	float fCurrentDepth = In.vLightProjPos.z / In.vLightProjPos.w;

	float2 UV = In.vLightProjPos.xy / In.vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
	float fShadowDepth = tex2D(ShadowSampler, UV).r;

	if (fCurrentDepth > fShadowDepth - fShadowBias)
	{
		Out.vColor.rgb *= 0.5f;
	}
	//발광
	Out.vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	Out.vColor.a = In.fBlur;
	return Out;
}

PS_OUT_SECOND PS_SUB(PS_IN_SECOND In)
{

	PS_OUT_SECOND		Out = (PS_OUT_SECOND)0;
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	vector vAlbedoColor = tex2D(DiffuseSampler, In.vTexUV);
	vAlbedoColor += vOcclusionColor;
	Out.vOcclusion = vAlbedoColor;
	return Out;
}

PS_SHADOW_OUT PS_SHADOW(PS_SHADOW_IN In)
{
	PS_SHADOW_OUT Out = (PS_SHADOW_OUT)0;

	float fDepth = In.vLightProjPos.z / In.vLightProjPos.w;
	Out.vDepth = vector(fDepth, 0.f, 0.f, 1.f);
	return Out;
}

PS_DEPTH_OUT PS_DEPTH(PS_DEPTH_IN In)
{

	PS_DEPTH_OUT		Out = (PS_DEPTH_OUT)0;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.00025f,0.f, 1.f);
	return Out;
}
vector vUseMasking; // 쓰면 0001 안쓰면 1111
PS_OUT_FIRST PS_DECAL(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	float2 fUV = In.vProjPos.xy / In.vProjPos.w;
	fUV = fUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	vector Pre_Depth = tex2D(PreDepthSampler, fUV);
	float	fViewZ = Pre_Depth.y * 7000.f; 

	vector DecalPos; // 텍스쳐 좌표에서 투영 좌표로 변환

	DecalPos.x = (fUV.x * 2.f - 1.f) * fViewZ;
	DecalPos.y = (fUV.y * -2.f + 1.f) * fViewZ;
	DecalPos.z = Pre_Depth.x * fViewZ;
	DecalPos.w = fViewZ;

	DecalPos = mul(DecalPos, g_matProjInv);
	DecalPos = mul(DecalPos, g_matViewInv);
	DecalPos = mul(DecalPos, g_matWorldInv);
	clip(0.5f - abs(DecalPos.xyz));
	float2 vDecalUV = DecalPos.xz;


	//알베도
	Out.vColor = texCUBE(CubeSampler, float3(vDecalUV.x, 0.5f, vDecalUV.y));
	vector vMaskingColor = tex2D(MaskingSampler, float2(vDecalUV.x, vDecalUV.y));
	vMaskingColor += vUseMasking;
	Out.vColor = Out.vColor * vMaskingColor;
	return Out;
}
float fDiminishColor;
PS_OUT_FIRST PS_DECAL_DISSAPEAR(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	float2 fUV = In.vProjPos.xy / In.vProjPos.w;
	fUV = fUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
	vector Pre_Depth = tex2D(PreDepthSampler, fUV);
	float	fViewZ = Pre_Depth.y * 7000.f;

	vector DecalPos; // 텍스쳐 좌표에서 투영 좌표로 변환

	DecalPos.x = (fUV.x * 2.f - 1.f) * fViewZ;
	DecalPos.y = (fUV.y * -2.f + 1.f) * fViewZ;
	DecalPos.z = Pre_Depth.x * fViewZ;
	DecalPos.w = fViewZ;

	DecalPos = mul(DecalPos, g_matProjInv);
	DecalPos = mul(DecalPos, g_matViewInv);
	DecalPos = mul(DecalPos, g_matWorldInv);
	clip(0.5f - abs(DecalPos.xyz));
	float2 vDecalUV = DecalPos.xz;

	//알베도
	Out.vColor = texCUBE(CubeSampler, float3(vDecalUV.x, 0.5f, vDecalUV.y));
	vector vMaskingColor = tex2D(MaskingSampler, float2(vDecalUV.x, vDecalUV.y));
	vMaskingColor += vUseMasking;
	Out.vColor = Out.vColor * fDiminishColor * vMaskingColor;
	return Out;
}

float fDistortionWeight;
float fDistortionUV;
vector vDistortionColor;
PS_OUT_FIRST PS_DISTORTION(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	//알베도
	vector vDistortion = tex2D(DistortionSampler, In.vTexUV);
	float fWeight = vDistortion.r * fDistortionWeight;
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV + float2(fWeight, fWeight + fDistortionUV));
	Out.vColor *= vDistortionColor;
	//노말
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	//깊이 1. 카메라에서의 Proj깊이 2. ViewZ 보관, 3. 카메라에서의 View 깊이 4. Light에서의 Proj깊이
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * fInvCamFar, In.vViewPos.z * fInvCamFar, In.vLightProjPos.z / In.vLightProjPos.w);

	float fCurrentDepth = In.vLightProjPos.z / In.vLightProjPos.w;

	float2 UV = In.vLightProjPos.xy / In.vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
	float fShadowDepth = tex2D(ShadowSampler, UV).r;

	if (fCurrentDepth > fShadowDepth - fShadowBias)
	{
		Out.vColor.rgb *= 0.5f;
	}
	//발광
	Out.vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	//Out.vColor.a = In.fBlur;
	return Out;
}

float fTime;
PS_OUT_FIRST PS_DISSOLVE(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	//알베도
	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor *= vChangeColor;
	vector vDissolve = tex2D(DissolveSampler, In.vTexUV);
	vector ClipAmount = vDissolve - vector(fTime, fTime, fTime, fTime);
	clip(ClipAmount.r < 0.f ? -1.f : 1.f);

	if (ClipAmount.r < 0.04f && fTime > 0.f)
	{
		vector vEdgeColor = tex2D(DissolveEdgeSampler, float2(ClipAmount.r * (1 / 0.04f), 0.f));
		Out.vColor += vEdgeColor;
	}

	//노말
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	//깊이 1. 카메라에서의 Proj깊이 2. ViewZ 보관, 3. 카메라에서의 View 깊이 4. Light에서의 Proj깊이
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * fInvCamFar, In.vViewPos.z * fInvCamFar, In.vLightProjPos.z / In.vLightProjPos.w);

	float fCurrentDepth = In.vLightProjPos.z / In.vLightProjPos.w;

	float2 UV = In.vLightProjPos.xy / In.vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
	float fShadowDepth = tex2D(ShadowSampler, UV).r;

	if (fCurrentDepth > fShadowDepth - fShadowBias)
	{
		Out.vColor.rgb *= 0.5f;
	}
	//발광
	Out.vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	Out.vColor.a = In.fBlur;

	return Out;
}

vector g_UseMasking;
vector vUseDistortion;
struct VS_EFFECT_IN
{
	vector vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_EFFECT_OUT
{
	vector vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

VS_EFFECT_OUT VS_EFFECT(VS_EFFECT_IN In)
{
	VS_EFFECT_OUT Out = (VS_EFFECT_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	return Out;
}

struct PS_EFFECT_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_EFFECT_OUT
{
	vector vColor : COLOR0;
};

PS_EFFECT_OUT PS_EFFECT(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT Out = (PS_EFFECT_OUT)0;

	//마스킹을 할건지 안할건지
	vector vOriColor = tex2D(DiffuseSampler, In.vTexUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	vMaskingColor += g_UseMasking; //마스킹을 하면 0000넘기니 그대로 마스킹 안하면 1111넘기니 무조건 
	vector vDistortionColor = tex2D(DistortionSampler, In.vTexUV);
	
	vector vOriMasking = mul(vOriColor, vMaskingColor);
	
	saturate(vMaskingColor.x);
	saturate(vMaskingColor.y);
	saturate(vMaskingColor.z);
	saturate(vMaskingColor.w);

	//UV를 조정할건지


	Out.vColor = mul(vOriColor, vMaskingColor);
	return Out;
}

vector vDyeColor;
PS_OUT_FIRST PS_COLOR(PS_IN_FIRST In)
{

	PS_OUT_FIRST		Out = (PS_OUT_FIRST)0;

	//알베도
	Out.vColor = vDyeColor;


	//노말
	vector tangentNormal = tex2D(NormalSampler, In.vTexUV);
	tangentNormal = normalize(tangentNormal * 2.f - 1.f);
	float3x3 TBN = float3x3(normalize(In.vTangent), normalize(In.vBiNormal), normalize(In.vNormal)); //얘는 월드에서 접선으로 바꿔주는얘임
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal.xyz);  // 이러면 월드로 바뀜
	Out.vNormal = vector(worldNormal, 1.f);

	//깊이 1. 카메라에서의 Proj깊이 2. ViewZ 보관, 3. 카메라에서의 View 깊이 4. Light에서의 Proj깊이
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * fInvCamFar, In.vViewPos.z * fInvCamFar, In.vLightProjPos.z / In.vLightProjPos.w);

	/*float fCurrentDepth = In.vLightProjPos.z / In.vLightProjPos.w;

	float2 UV = In.vLightProjPos.xy / In.vLightProjPos.w;
	UV.y = -UV.y;
	UV = UV * 0.5f + 0.5f;
	vector fShadowDepth = tex2D(ShadowSampler, UV);

	if (fCurrentDepth > fShadowDepth.r + fShadowBias)
	{
		Out.vColor *= fShadowDepth * 0.5f;
	}*/
	//발광
	Out.vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	Out.vColor.a = In.fBlur;

	return Out;
}


vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);

struct VS_ALPHA_IN
{
	vector		vPosition : POSITION;		
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_ALPHA_OUT
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};


VS_ALPHA_OUT	VS_ALPHA(VS_ALPHA_IN In)
{
	VS_ALPHA_OUT		Out = (VS_ALPHA_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	vector vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vector vWorldDir = g_vLightDir * -1.f;

	float	fDot = saturate(dot(normalize(vWorldNormal), normalize(vWorldDir)));

	// float	fDot = max(dot(normalize(vWorldNormal), normalize(vWorldDir)), 0.f);

	Out.vShade = fDot;
	Out.vShade.a = 1.f;


	Out.vTexUV = In.vTexUV;

	return Out;
}

struct	PS_ALPHA_IN
{
	vector			vShade : COLOR0;
	float2			vTexUV : TEXCOORD0;

};

struct PS_ALPHA_OUT
{
	vector		vColor : COLOR0;
};

PS_ALPHA_OUT PS_ALPHA(PS_ALPHA_IN In)
{
	PS_ALPHA_OUT		Out = (PS_ALPHA_OUT)0;

	vector vColor = tex2D(DiffuseSampler, In.vTexUV);		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	clip(vColor.a < 0.1f ? -1.f : 1.f);
	Out.vColor = (vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + In.vShade * (g_vLightAmbient * g_vMtrlAmbient);

	return Out;
}


technique		Default_Device
{
	// 기능의 캡슐화
	pass	Normal //0
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	SUB //1
	{
		vertexshader = compile vs_3_0 VS_SUB();
		pixelshader = compile ps_3_0 PS_SUB();
	}

	pass	AlphaTest //2
	{
		alphatestenable = true;
		Alphafunc = greater;
		alpharef = 0xa0;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass		Alphablend_Black_Remove_cullnon //3
	{
		alphablendenable = true;
		srcblend = srccolor;
		destblend = invsrcalpha;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass		Alphablend //4
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
	}
	pass		Alphablend_Effect //5	시원 이펙트 UV 돌리는 패스 : 알파
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_SIWON_ALPHA_UV();
	}
	pass		Shadow //6
	{
	vertexshader = compile vs_3_0 VS_SHADOW();
	pixelshader = compile ps_3_0 PS_SHADOW();
	}

	pass Distortion //7
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;
		vertexshader = compile vs_3_0 VS_EFFECT();
		pixelshader = compile ps_3_0 PS_EFFECT();
	}
		pass		Alphablend_Effect //8	시원 이펙트 UV 돌리는 패스 : 논알파
	{
		alphablendenable = true;
		srcblend = SrcColor;
		destblend = DestColor;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_SIWON_ALPHA_UV();
	}
		pass		Depth // 9
	{
	vertexshader = compile vs_3_0 VS_DEPTH();
	pixelshader = compile ps_3_0 PS_DEPTH();
	}
		pass		Decal // 10
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;
		//zwriteEnable = false;
		zenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DECAL();
	}
		pass		Distortion // 11
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;
		cullmode = none;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DISTORTION();
	}
		pass		Decal_Inv // 12
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		cullmode = none;
		//zwriteEnable = false;
		zenable = false;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_DECAL_DISSAPEAR();
	}
		pass		Decal_Black_Remove // 13
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;
	//zwriteEnable = false;
	zenable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_DECAL();
	}
		pass		DISSOLVE // 14
	{
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_DISSOLVE();
	}
	pass		COLOR // 15 , 컬러 추가
	{
		alphablendenable = true;
	srcblend = srccolor;
	destblend = one;
		cullmode = none;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN_COLOR();
	}
	pass	Normal_Terrain //16 -- Terrain (밉맵 적용)  :희정
	{
		vertexshader = compile vs_3_0 VS_TERRAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}	
	pass	Normal_3D //17 -- 직교투영되는 3D 캐릭터들 용, 카르텔-깃발 전용 : 희정
	{
	cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	ChangeColor //18  색깔을 직접 넘겨줌
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_COLOR();
	}

	pass	RimLight //19 
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_RIMLIGHT();
	}
	pass	Run_Terrain //20
	{
		vertexshader = compile vs_3_0 VS_TERRAIN_RUN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass	ALPHA //21
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	//alphatestenable = true;
	//alphafunc = greater;
	//alpharef = 0xa0;

	vertexshader = compile vs_3_0 VS_ALPHA();
	pixelshader = compile ps_3_0 PS_ALPHA();
	}

	pass	UVCHANGE //22
	{

	vertexshader = compile vs_3_0 VS_UVCHANGE();
	pixelshader = compile ps_3_0 PS_MAIN();
	}
};