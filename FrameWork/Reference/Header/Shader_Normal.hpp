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
// x == 시작위치, y == 아틀라스값 , z == UV애니메이션값 , w == 안쓴다
vector				g_vUV_U;
vector				g_vUV_V;


vector				g_vRGBA;
vector				g_UseMasking; // 마스킹을 사용할거면 setvector에서 0,0,0,0 넘겨주고 사용 안할거면 1,1,1,1 넘겨줘라

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

// 버텍스 쉐이딩

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

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
	
	return Out;
}

PS_OUT PS_SIWON_UV(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	// UV 애니메이션
	In.vTexUV.x *= g_vUV_U.y;
	In.vTexUV.x += g_vUV_U.x + g_vUV_U.z;
	In.vTexUV.y *= g_vUV_V.y;
	In.vTexUV.y += g_vUV_V.x + g_vUV_V.z;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입

	return Out;
}

PS_OUT PS_SIWON_UV_RGB(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	// UV 애니메이션
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

	vector vColor = tex2D(DiffuseSampler, In.vTexUV);		// 2D 텍스처로부터 UV값을 추출하는 함수 ,반환타입은 VECTOR 타입
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

//오리지널, 마스킹 둘다 UV 안움직임
PS_OUT PS_MASKING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vOriColor = tex2D(DiffuseSampler, In.vTexUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	Out.vColor = vOriColor * vMaskingColor;

	return Out;
}

//오리지널의 UV가 움직임, 마스킹 UV 안움직임
PS_OUT PS_UV_MASKING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vMaskingUV = In.vTexUV ;
	// UV 애니메이션
	vMaskingUV.x *= g_vUV_U.y;
	vMaskingUV.x += g_vUV_U.x + g_vUV_U.z;
	vMaskingUV.y *= g_vUV_V.y;
	vMaskingUV.y += g_vUV_V.x + g_vUV_V.z;

	vector vOriColor = tex2D(DiffuseSampler, vMaskingUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	Out.vColor = vOriColor * vMaskingColor;

	return Out;
}

//오리지널의 UV가 움직임, 마스킹 UV 안움직임
PS_OUT PS_UV_MASKING_DISAPPEAR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vMaskingUV = In.vTexUV;
	// UV 애니메이션
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
	// UV 애니메이션
	vMaskingUV.x *= g_vUV_U.y;
	vMaskingUV.x += g_vUV_U.x + g_vUV_U.z;
	vMaskingUV.y *= g_vUV_V.y;
	vMaskingUV.y += g_vUV_V.x + g_vUV_V.z;

	vector vOriColor = tex2D(DiffuseSampler, vMaskingUV);
	vector vMaskingColor = tex2D(MaskingSampler, In.vTexUV);
	Out.vColor = vOriColor * vMaskingColor * g_vRGBA;

	return Out;
}

//텍스쳐 둘의 합
PS_OUT PS_ADD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vFirstColor = tex2D(DiffuseSampler, In.vTexUV);
	vector vSecondColor = tex2D(MaskingSampler, In.vTexUV);
	vector vAddColor = vFirstColor + vSecondColor;
	Out.vColor = vAddColor;
	return Out;
}

//텍스쳐 둘의 합
PS_OUT PS_DISSAPEAR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = tex2D(DiffuseSampler, In.vTexUV);
	vColor *= g_vRGBA;
	Out.vColor = vColor;
	return Out;
}

//오리지널의 UV가 움직임, 마스킹 UV 안움직임, 디졸브
float fTime;
PS_OUT PS_UV_MASKING_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 vMaskingUV = In.vTexUV;
	// UV 애니메이션
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
	// 기능의 캡슐화
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
//4 기본 텍스쳐 이펙트 : 알파블렌드 
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
pass		 UI_COLOR //->희정만듬 (Color 수정)
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_UI_COLOR();
}

//6
pass		UI_UVCONTROL //->희정만듬 (UV + Color수정)
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

//9 // 마스킹 + UV 돌고 +알파블렌드
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

//10 ->데미지박스
pass		 UI_COLOR_DAMAGEBOX //->희정만듬 (Color 수정)
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	zenable = false;
	zwriteenable = false;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_UI_COLOR();
}

//11 ->tired전용
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

//12 ->ultimate전용
pass		AlphaTest_ultimate
{
	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xc0;

	cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_MAIN();
}
//13 텍스쳐 두개의 합
pass texture_Add
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;
vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_ADD();
}
//14 기본 텍스쳐 이펙트 : 알파블렌드  + RGB값 추가
pass		SiwonUVAlpha
{
	alphablendenable = true;
srcblend = srcalpha;
destblend = one;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_SIWON_UV_RGB();
}
//15 // 마스킹 + UV 돌고 +알파블렌드 + RGB
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
//16 기본인데 검은색 알파로 뚫기
pass BLACK_ALPHA_REMOVE
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}
//17 // 마스킹 + UV 돌고 +알파블렌드  , 9번패스에 감소값 있는거
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
//18 기본 텍스쳐 이펙트 : 알파블렌드 +검정색 컬러 뚫음 + RGB값 추가
pass		TEXTUREEFFECT_DUST_TS
{
	alphablendenable = true;
srcblend = srccolor;
destblend = one;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_SIWON_UV_RGB();
}


//19 // 마스킹 + UV 돌고 +알파블렌드
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

//20 서서히 사라짐
pass DIssapear
{
	alphablendenable = true;
srcblend = srccolor;
destblend = one;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_DISSAPEAR();
}

//21 나무 전용 - 알파블렌드 + 컬모드 넌
pass		AlphablendTree
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;
	cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}
//22 마스킹, 알파블렌드
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
pass ForKarmaOverHead // 희정 추가 : 검은색뚫기+ RGBA 수정
{
	alphablendenable = true;
	srcblend = srcalpha;
	destblend = one;
	cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_UI_COLOR();
}
//24
pass		ForAICondition //희정 추가 : 기본알파블렌드 + zenable false (AI들 머리위UI)
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
pass		 UI_alphatest_COLOR //->희정만듬 알파테스트(Color 수정)
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

//27 마스킹 + UV 돌고 +알파블렌드 + 디졸브
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