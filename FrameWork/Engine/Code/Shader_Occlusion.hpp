texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_RandomTexture;

sampler RandomSampler = sampler_state
{
	texture = g_RandomTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_OcclusionBaseTexture;

sampler OcclusionBaseSampler = sampler_state
{
	texture = g_OcclusionBaseTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture			g_OcclusionHorizontalTexture;

sampler OcclusionHorizontalSampler = sampler_state
{
	texture = g_OcclusionHorizontalTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

const float3 fKernal[16] =
{
	float3(0.355512, -0.709318, -0.102371),
	float3(0.534186, 0.71511, -0.115167),
	float3(-0.87866, 0.157139, -0.115167),
	float3(0.140679, -0.475516, -0.0639818),
	float3(-0.0796121, 0.158842, -0.677075),
	float3(-0.0759516, -0.101676, -0.483625),
	float3(0.12493, -0.0223423, -0.483625),
	float3(-0.0720074, 0.243395, -0.967251),
	float3(-0.207641, 0.414286, 0.187755),
	float3(-0.277332, -0.371262, 0.187755),
	float3(0.63864, -0.114214, 0.262857),
	float3(-0.184051, 0.622119, 0.262857),
	float3(0.110007, -0.219486, 0.435574),
	float3(0.235085, 0.314707, 0.696918),
	float3(-0.290012, 0.0518654, 0.522688),
	float3(0.0975089, -0.329594, 0.609803)
};

float fRadius;

//Horizontal Blur
float HBlurWeight[9];
float HBlurOffset[9];
//Vertical Blur
float VBlurWeight[9];
float VBlurOffset[9];

struct PS_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_OCCLUSION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	float3 vRandom = tex2D(RandomSampler, In.vTexUV * 10.f).xyz;
	vRandom = vRandom * 2.f - 1.f;

	float fPixelDepth = tex2D(DepthSampler, In.vTexUV).b;
	float fDepth = fPixelDepth * 10000.f;
	float3 vKernelScale = float3(fRadius / fDepth, fRadius / fDepth, fRadius * 0.00001f	);

	float fOcclusion = 0.f;
	for (int j = 1; j < 3; ++j)
	{
		float3 random = tex2D(RandomSampler, In.vTexUV * (7.f + (float)j)).xyz;
		random = random * 2.f - 1.f;

		for (int i = 0; i < 16; ++i)
		{
			float3 vRotatedKernel = reflect(fKernal[i], random) * vKernelScale;
			float fSampleDepth = tex2D(DepthSampler, vRotatedKernel.xy + In.vTexUV).b;
			float fDelta = max(fSampleDepth - fPixelDepth + vRotatedKernel.z, 0.f);
			float fRange = abs(fDelta) / (vKernelScale.z * fRadius);
			fOcclusion += lerp(fDelta * fRadius, fRadius, saturate(fRange));
		}
	}

	Out.vColor = fOcclusion / (2.f * 8.f);
	Out.vColor = lerp(0.1f, 0.6f, saturate(Out.vColor.x));
	return Out;
}


PS_OUT PS_OCCLUSION_HORIZONTAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = vector(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < 9; ++i)
	{
		vColor += (tex2D(OcclusionBaseSampler, In.vTexUV + float2(HBlurOffset[i], 0.f)) * HBlurWeight[i]);
	}
	Out.vColor = vector(vColor.rgb, 1.f);

	return Out;
}

PS_OUT PS_OCCLUSION_VERTICAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	vector vColor = vector(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < 9; ++i)
	{
		vColor += (tex2D(OcclusionHorizontalSampler, In.vTexUV + float2(0.f, VBlurOffset[i])) * VBlurWeight[i]);
	}
	Out.vColor = vector(vColor.rgb, 1.f);
	
	return Out;
}

technique Default_Device
{
	pass Occlusion
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_OCCLUSION();
	}

	pass Occlusion_Horizontal
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_OCCLUSION_HORIZONTAL();
	}

	pass Occlusion_Vertical
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_OCCLUSION_VERTICAL();
	}

};