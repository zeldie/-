texture g_HDRBaseTexture;

sampler HDRBaseSampler = sampler_state // 노말 텍스쳐에 필터링은 절대 해선 안된다.
{
	texture = g_HDRBaseTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_LuminanceTexture;

sampler LuminanceSampler = sampler_state
{
	texture = g_LuminanceTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

vector vGreyOffset[4]; //GreyDownSample에 사용
vector vDSOffset[9]; // DownSample에 사용

struct PS_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_GREYDOWNSAMPLE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float fAverage = 0.f;
	float fMaximum = -1e20;
	vector vcolor = 0.f;
	for (int i = 0; i < 4; ++i)
	{
	vcolor = tex2D(HDRBaseSampler, In.vTexUV + float2(vGreyOffset[i].x, vGreyOffset[i].y));
	float GreyValue = max(vcolor.r, max(vcolor.g, vcolor.b));
	fMaximum = max(fMaximum, GreyValue);
	fAverage += (0.25f * log(1e-5 + GreyValue));
	}
	fAverage = exp(fAverage);
	Out.vColor = vector(fAverage, fMaximum, 0.f, 1.f);
	return Out;
}

PS_OUT PS_DOWNSAMPLE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vcolor = 0.f;
	float fMaximum = -1e20;
	float fAverage = 0.f; 
	for (int i = 0; i < 9; ++i)
	{
		vcolor = tex2D(LuminanceSampler, In.vTexUV + float2(vDSOffset[i].x, vDSOffset[i].y));
		fAverage += vcolor.r;
		fMaximum = max(fMaximum, vcolor.g);
	}
	fAverage *= 0.11f;
	Out.vColor = vector(fAverage, fMaximum, 0.f, 1.f);
	return Out;
}

technique Default_Device
{
	pass GreySample
{

	//zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_GREYDOWNSAMPLE();
}

pass DownSample
{

	//zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_DOWNSAMPLE();
}
};