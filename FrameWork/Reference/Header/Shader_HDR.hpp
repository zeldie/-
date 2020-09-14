
texture g_HDRBaseTexture;

sampler HDRBaseSampler = sampler_state 
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

texture g_BrightPassTexture;

sampler BrightPassSampler = sampler_state
{
	texture = g_BrightPassTexture;
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

texture g_HorizontalBlurTexture;

sampler HorizontalBlurSampler = sampler_state
{
	texture = g_HorizontalBlurTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_VerticalBlurTexture;

sampler VerticalBlurSampler = sampler_state
{
	texture = g_VerticalBlurTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture				g_EmmisiveTexture;

sampler				EmmisiveSampler = sampler_state
{
	texture = g_EmmisiveTexture;
minfilter = linear;
magfilter = linear;
mipfilter = linear;

};

texture			g_BlurTexture;

sampler BlurSampler = sampler_state
{
	texture = g_BlurTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;


};

texture			g_ShadowTexture;

sampler ShadowSampler = sampler_state
{
	texture = g_ShadowTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;


};

texture			g_RadialBlurTexture;

sampler RadialBlurSampler = sampler_state
{
	texture = g_RadialBlurTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;


};
//Luminance
vector vGreyOffset[4]; //GreyDownSample에 사용
vector vDSOffset[9]; // DownSample에 사용
//BrightPass
float  fBrightPassThreshold;
vector vDSOffsetBright[16];
//Horizontal Blur
float HBloomWeights[9];
float HBloomOffsets[9];
//Vertical Blur
float VBloomWeights[9];
float VBloomOffsets[9];

//final
float       fExposure;
float       fGaussianScalar;
float       g_rcp_bloom_tex_w;
float       g_rcp_bloom_tex_h;

//흑백
bool bFilterMonochrome;

float a = 0.15f;

float b = 0.5f;

float c = 0.1f;

float d = 0.2f;

float e = 0.02f;

float f = 0.3f;


float Gamma;
float3 Uncharted2Tonemap(float3 x) 
{
	return (((x * (a * x + c * b)) + d * d) / (x * (a * x + b) + d * f)) -e / f;
}

float Luminance = 0.08f;

static const float fMiddleGray = 0.18f;

static const float fWhiteCutoff = 0.8f;

//float fA;
//float fB;
//float fC;
//float fD;
//float fE;
//float fF;
//float3 ToneMapping(float3 fX)
//{
//	return ((fX * (fA * fX + fC * fB)) + fD * fD) / (fX * (fA * fX + fB) + fD * fF)) - fE / fF;
//}
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

PS_OUT PS_BRIGHTPASS(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	vector vAverage = vector(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < 4; ++i)
	{
		vAverage += tex2D(HDRBaseSampler, In.vTexUV + float2(vDSOffsetBright[i].x, vDSOffsetBright[i].y));
	}
	vAverage *= 0.25f;
	float luminance = max(vAverage.r, max(vAverage.g, vAverage.b));
	if (luminance < fBrightPassThreshold)
		vAverage = vector(0.f, 0.f, 0.f, 1.f);
	Out.vColor = vAverage + pow(abs(tex2D(BlurSampler, In.vTexUV)), Gamma);
	return Out;
}

PS_OUT PS_DOWNSAMPLE16(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	vector vAverage = vector(0.f, 0.f, 0.f, 0.f);
	for (int i= 0; i < 16; ++i)
	{
		vAverage += tex2D(BrightPassSampler, In.vTexUV + float2(vDSOffsetBright[i].x, vDSOffsetBright[i].y));
	}
	vAverage *= (1.f / 16.f);
	Out.vColor = vAverage;
	return Out;
}

PS_OUT PS_HORIZONTALBLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	vector vColor = vector(0.f, 0.f, 0.f, 0.f);
	for (int i= 0; i < 9; ++i)
	{
		vColor += (tex2D(DownSampleSampler, In.vTexUV + float2(HBloomOffsets[i], 0.f)) * HBloomWeights[i]);
	}
	Out.vColor = vector(vColor.rgb, 1.f);
	return Out;
}

PS_OUT PS_VERTICALBLUR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	vector vColor = vector(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < 9; ++i)
	{
		vColor += (tex2D(HorizontalBlurSampler, In.vTexUV + float2(0.f, VBloomOffsets[i])) * VBloomWeights[i]);
	}
	Out.vColor = vector(vColor.rgb, 1.f);
	return Out;
}

PS_OUT PS_FIANL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	// Read the HDR value that was computed as part of the original scene
	vector vC = pow(abs(tex2D(HDRBaseSampler, In.vTexUV)), Gamma) * fMiddleGray / (Luminance + 0.001f);
	//vector vC = pow(abs(tex2D(HDRBaseSampler, In.vTexUV) * fMiddleGray / (Luminance + 0.001f)), Gamma);

	// Read the luminance value, target the centre of the texture
	// which will map to the only pixel in it!
	vector vL = tex2D(LuminanceSampler, float2(0.5f, 0.5f));


	// Compute the blur value using a bilinear filter
	// It is worth noting that if the hardware supports linear filtering of a
	// floating point render target that this step can probably be skipped.
	//float xWeight = frac(In.vTexUV.x / g_rcp_bloom_tex_w) - 0.5;
	//float xDir = xWeight;
	//xWeight = abs(xWeight);
	//xDir /= xWeight;
	//xDir *= g_rcp_bloom_tex_w;

	//float yWeight = frac(In.vTexUV.y / g_rcp_bloom_tex_h) - 0.5;
	//float yDir = yWeight;
	//yWeight = abs(yWeight);
	//yDir /= yWeight;
	//yDir *= g_rcp_bloom_tex_h;

	//// sample the blur texture for the 4 relevant pixels, weighted accordingly
	vector vB = tex2D(VerticalBlurSampler, In.vTexUV);
	//vector vB = ((1.f - xWeight) * (1.f - yWeight))    * tex2D(VerticalBlurSampler, In.vTexUV);
	//vB += (xWeight * (1.f - yWeight))             * tex2D(VerticalBlurSampler, In.vTexUV + float2(xDir, 0.0f));
	//vB += (yWeight * (1.f - xWeight))             * tex2D(VerticalBlurSampler, In.vTexUV + float2(0.0f, yDir));
	//vB += (xWeight * yWeight)                      * tex2D(VerticalBlurSampler, In.vTexUV + float2(xDir, yDir));

	vector vBlur = pow(abs(tex2D(BlurSampler, In.vTexUV)), Gamma);
	// Compute the actual colour:
	vector vFinal = vC + vB + vBlur * 2.f;
	//vC = vC + 0.25f * vB;
	//vector vFinal = pow(abs(vC + 0.25f * vB), 1 / 2.2f)

	// Reinhard's tone mapping equation (See Eqn#3 from 
	// "Photographic Tone Reproduction for Digital Images" for more details) is:
	//
	//      (      (   Lp    ))
	// Lp * (1.0f +(---------))
	//      (      ((Lm * Lm)))
	// -------------------------
	//         1.0f + Lp
	//
	// Lp is the luminance at the given point, this is computed using Eqn#2 from the above paper:
	//
	//        exposure
	//   Lp = -------- * HDRPixelIntensity
	//          l.r
	//
	// The exposure ("key" in the above paper) can be used to adjust the overall "balance" of 
	// the image. "l.r" is the average luminance across the scene, computed via the luminance
	// downsampling process. 'HDRPixelIntensity' is the measured brightness of the current pixel
	// being processed.

	//float Lp = (fExposure / vL.r) * max(vFinal.r, max(vFinal.g, vFinal.b));

	// A slight difference is that we have a bloom component in the final image - this is *added* to the 
	// final result, therefore potentially increasing the maximum luminance across the whole image. 
	// For a bright area of the display, this factor should be the integral of the bloom distribution 
	// multipled by the maximum value. The integral of the gaussian distribution between [-1,+1] should 
	// be AT MOST 1.0; but the sample code adds a scalar to the front of this, making it a good enough
	// approximation to the *real* integral.

	//float LmSqr = (vL.g + fGaussianScalar * vL.g) * (vL.g + fGaussianScalar * vL.g);

	////////////////////////레이나드////////////////////////////
	//float toneScalar = (Lp * (1.0f + (Lp / (LmSqr)))) / (1.0f + Lp);
	//vC = vFinal * toneScalar;
	//vC = (vFinal + tex2D(EmmisiveSampler, In.vTexUV)) * toneScalar;
	////////////////////////레이나드////////////////////////////
	// Tonemap the final outputted pixel:

	//////////////////언차티드////////////////////////////
	//float3 Diffuse = Uncharted2Tonemap(2.f * vFinal.rgb);
	//float3 WhiteScale = 1.f / Uncharted2Tonemap(11.2f);
	//vFinal = vector(Diffuse, 1.f) * vector(WhiteScale, 1.f);
	//vFinal = pow(abs(vFinal), 0.45f);
	//////////////////언차티드////////////////////////////
	
	////////////////////////EA///////////////////////////////
	/*float3 x = max(0.f, vFinal - 0.004f);
	vFinal.rgb = pow((x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f), 0.454f);*/
	////////////////////////EA///////////////////////////////

	//////////////////SAMPLE///////////////////
	vFinal.rgb *= (1.f + (vFinal.rgb / (fWhiteCutoff * fWhiteCutoff)));
	vFinal.rgb /= (1.f + vFinal.rgb);

	//vFinal.rgb = pow(abs(vFinal.rgb), 0.454f);
	//////////////////SAMPLE///////////////////
	// Return the fully composed colour
	vFinal.a = 1.0f;
	
	Out.vColor = vFinal;
	//if (bFilterMonochrome == false)
	//	return Out;
	//else
	//	Out.vColor = dot((float3)Out.vColor, float3(0.2125f, 0.7154f, 0.0721f));



	return Out;
}

//float2 g_texmapscale;
//float4 offset_lookup(sampler map, float4 loc, float2 offset)
//{
//	return tex2D(map, float4(loc.xy + offset * g_texmapscale * loc.w, loc.z, loc.w));
//}
//float EPSILON = 0.00001f;
//PS_OUT PS_SHADOW_FILTER(PS_IN In)
//{
//	PS_OUT Out = (PS_OUT)0;
//	float fShadow = 0.f;
//	for (int x = -1; x <= 1; ++x)
//	{
//		for (int y = -1; y <= 1; ++y)
//		{
//			float pcfDepth = tex2D(ShadowSampler, In.vTexUV.xy + vec2(x, y) * g_texmapscale.x).r;
//			fShadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//		}
//	}
//	shadow /= 9.0;
//	//float Factor = 0.f;
//	//float z = 0.5f * In.vTexUV.y + 0.5f;
//	//for (int y = -1; y <= 1; y++)
//	//{
//	//	for (int x = -1; x <= 1; x++)
//	//	{
//	//		float2 Offsets = float2(x * g_texmapscale.x, y * g_texmapscale.y);
//	//		float3 UVC = float3(In.vTexUV + Offsets, z + EPSILON);
//	//		Factor += tex2D(ShadowSampler, UVC.xy);
//	//	}
//	//}
//	//Out.vColor.r = (0.5f + (Factor / 18.f));
//	/*float sum = 0;
//	float x, y;
//
//	for (y = -1.5; y <= 1.5; y += 1.0)
//		for (x = -1.5; x <= 1.5; x += 1.0)
//			sum += offset_lookup(ShadowSampler, In.vTexUV, float2(x, y));
//
//	Out.vColor.r = sum / 16.0;*/
//	return Out;
//}

float2 g_vCenter = { 0.5f,0.5f };
#define SAMPLE_NUM 10
float g_fEffectAmount = 1.f;
float g_fRadius = 3.f;
vector PS_RADIALBLUR(PS_IN In) : COLOR0
{

	vector vColor = (vector)0;
	float2 vDist = In.vTexUV - g_vCenter;

	for (int i = 0; i < SAMPLE_NUM; i++)
	{
		float fScale = 1 - g_fEffectAmount * ((float)i / (float)SAMPLE_NUM) * (saturate(length(vDist) / g_fRadius));
		vColor += tex2D(RadialBlurSampler, vDist * fScale + g_vCenter);
	}
	vColor /= SAMPLE_NUM;
	return vColor;
}

technique Default_Device
{
	pass GreySample //0
	{

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_GREYDOWNSAMPLE();
	}

	pass DownSample //1
	{

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_DOWNSAMPLE();
	}

	pass BrightPass //2
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_BRIGHTPASS();
	}

	pass DownSample16 //3
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_DOWNSAMPLE16();
	}

	pass HorizontalBlur //4
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_HORIZONTALBLUR();
	}

	pass VerticlaBlur //5
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_VERTICALBLUR();
	}

	pass VerticlaBlur //6
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_FIANL();
	}
	pass RadialBLur //7
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_RADIALBLUR();
	}
};