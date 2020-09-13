texture			g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

};

texture			g_EmmisiveTexture;

sampler EmmisiveSampler = sampler_state
{
	texture = g_EmmisiveTexture;

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

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = pow(tex2D(AlbedoSampler, In.vTexUV), 2.2);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	Out.vColor = vAlbedo * vShade + vEmmisive;
	return Out;
}

technique Default_Device
{
	pass Blend
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

};