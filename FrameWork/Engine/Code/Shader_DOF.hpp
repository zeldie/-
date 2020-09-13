texture g_HDRFinishTexture;

sampler HDRFinishSampler = sampler_state
{
	texture = g_HDRFinishTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_DownSampleTexture;

sampler DownSampleSampler = sampler_state
{
	texture = g_DownSampleTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_DOFHorizontalBlurTexture;

sampler DOFHorizontalBLurSampler = sampler_state
{
	texture = g_DOFHorizontalBlurTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_DOFBlurTexture;

sampler DOFBlurSampler = sampler_state
{
	texture = g_DOFBlurTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_HDRTexture;

sampler HDRSampler = sampler_state
{
	texture = g_HDRTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_LightShaftTexture;

sampler LightShaftSampler = sampler_state
{
	texture = g_LightShaftTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_RadialBlurTexture;

sampler RadialBlurSampler = sampler_state
{
	texture = g_RadialBlurTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_FinalImageTexture;

sampler FinalImageSampler = sampler_state
{
	texture = g_FinalImageTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

struct PS_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_DOWNSAMPLE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = tex2D(HDRFinishSampler, In.vTexUV);
	return Out;
}

//Horizontal Blur
float HBlurWeights[9];
float HBlurOffsets[9];
//Vertical Blur
float VBlurWeights[9];
float VBlurOffsets[9];
PS_OUT PS_HORIZONTALBLUR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = vector(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < 9; ++i)
	{
		vColor += (tex2D(DownSampleSampler, In.vTexUV + float2(HBlurOffsets[i], 0.f)) * HBlurWeights[i]);
	}
	//vColor *= 0.11f;
	Out.vColor = vector(vColor.rgb, 1.f);
	return Out;
}

PS_OUT PS_VERTICALBLUR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = vector(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < 9; ++i)
	{
		vColor += (tex2D(DOFHorizontalBLurSampler, In.vTexUV + float2(0.f, VBlurOffsets[i])) * VBlurWeights[i]);
	}
	//vColor *= 0.11f;
	Out.vColor = vector(vColor.rgb, 1.f);
	return Out;
}


PS_OUT PS_DOF(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//vector vBlurred = tex2D(DOFBlurSampler, In.vTexUV);
	//float fBlur = tex2D(AlbedoSampler, In.vTexUV).a;
	//vector vFullres = tex2D(HDRSampler, In.vTexUV);
	vector vLightShaft = tex2D(LightShaftSampler, In.vTexUV);
	vector vColor = tex2D(HDRSampler, In.vTexUV);
	Out.vColor = vLightShaft + vColor;
	//return Out;
	//vector vRadial = tex2D(RadialBlurSampler, In.vTexUV);
	////float3 vColor = lerp(vFullres, vBlurred, fBlur);
	//if (bRadial == true)
	//	vColor = vLightShaft + vColor *vRadial;
	//else
	//	vColor = vLightShaft + vColor;
	//Out.vColor = vColor;
	//if (bFilterMonochrome == false)
	//	return Out;
	//else
	//	Out.vColor = dot((float3)Out.vColor, float3(0.2125f, 0.7154f, 0.0721f));
	return Out;
}


vector vFadeColor;
PS_OUT PS_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = vFadeColor;
	Out.vColor = vColor;
	return Out;
}


bool bFilterMonochrome;
bool bRadial;
float2 g_vCenter = { 0.5f,0.5f };
#define SAMPLE_NUM 10
float g_fEffectAmount = 1.f;
float g_fRadius = 3.f;
PS_OUT PS_LASTPOSTPROCESS(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	/*vector vColor;
	float2 vDist = In.vTexUV - g_vCenter;

	for (int i = 0; i < SAMPLE_NUM; i++)
	{
		float fScale = 1 - g_fEffectAmount * ((float)i / (float)SAMPLE_NUM) * (saturate(length(vDist) / g_fRadius));
		vColor += tex2D(FinalImageSampler, vDist * fScale + g_vCenter);
	}
	vColor /= SAMPLE_NUM;
	Out.vColor = vColor;*/
	if (bRadial == true)
	{
		vector vColor = (vector)0;
		float2 vDist = In.vTexUV - g_vCenter;

		for (int i = 0; i < SAMPLE_NUM; i++)
		{
			float fScale = 1 - g_fEffectAmount * ((float)i / (float)SAMPLE_NUM) * (saturate(length(vDist) / g_fRadius));
			vColor += tex2D(FinalImageSampler, vDist * fScale + g_vCenter);
		}
		vColor /= SAMPLE_NUM;
		Out.vColor = vColor;
	}
	else
	{
		vector vFinalImage = tex2D(FinalImageSampler, In.vTexUV);
		Out.vColor = vFinalImage;
	}
	if (bFilterMonochrome == false)
		return Out;
	else
		Out.vColor = dot((float3)Out.vColor, float3(0.2125f, 0.7154f, 0.0721f));
	return Out;
}

technique Default_Device
{
	pass DownSample //0
	{
		//alphablendenable = true;
		//srcblend = srcalpha;
		//destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_DOWNSAMPLE();
	}

	pass HorizontalBlur //1
	{
		//alphablendenable = true;
		//srcblend = srcalpha;
		//destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_HORIZONTALBLUR();
	}

		pass VerticalBlur //2
	{
		//alphablendenable = true;
		//srcblend = srcalpha;
		//destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_VERTICALBLUR();
	}
		pass DOF //3
	{
		//alphablendenable = true;
		//srcblend = srcalpha;
		//destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_DOF();
	}

		pass Fade //4
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_FADE();
	}

		pass LASTPROCESS//5
	{

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_LASTPOSTPROCESS();
	}
};