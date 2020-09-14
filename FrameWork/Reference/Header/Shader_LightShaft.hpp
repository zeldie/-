texture g_OcclusionTexture;

sampler OcclusionSampler = sampler_state
{
	texture = g_OcclusionTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

//X : Density, Y : Decay, Z : Weight, W : Exposure
vector vLightShaftValue;
const int NUM_SAMPLES = 100;
float fLightPositionX;
float fLightPositionY;
struct PS_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_LIGHTSHAFT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	float2 deltaTexCoord = (In.vTexUV- float2(fLightPositionX, fLightPositionY));
	deltaTexCoord *= vLightShaftValue.x / NUM_SAMPLES;
	vector vcolor = tex2D(OcclusionSampler, In.vTexUV);
	float illuminationDecay = 1.f;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		In.vTexUV -= deltaTexCoord;
		vector vSample = tex2D(OcclusionSampler, In.vTexUV);
		vSample *= illuminationDecay * vLightShaftValue.z;
		vcolor += vSample;
		illuminationDecay *= vLightShaftValue.y;
	}

	Out.vColor = saturate(vcolor * vLightShaftValue.w);
	return Out;
}


technique Default_Device
{
	pass LightShaft
	{
		alphablendenable = true;
		srcblend = one;
		destblend = one;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_LIGHTSHAFT();
	}
};