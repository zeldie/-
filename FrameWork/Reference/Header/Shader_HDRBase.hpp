float Gamma;
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

texture			g_OutlineTexture;

sampler OutlineSampler = sampler_state
{
	texture = g_OutlineTexture;

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

float fLightAmount;
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

	//vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV); // pow(abs(tex2D(AlbedoSampler, In.vTexUV)), 1 / Gamma);
	vector		vAlbedo = pow(abs(tex2D(AlbedoSampler, In.vTexUV)), 2.2);
	vAlbedo *= float4(fLightAmount, fLightAmount, fLightAmount, fLightAmount);
	vector vOutline = tex2D(OutlineSampler, In.vTexUV) * 0.5f;
	vAlbedo = vAlbedo * vOutline;
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	//vShade *= vector(fLightAmount, fLightAmount, fLightAmount, 1.f);
	//vector		vEmmisive = pow(abs(tex2D(EmmisiveSampler, In.vTexUV)), 2.2f);
	vector		vEmmisive = tex2D(EmmisiveSampler, In.vTexUV);
	vEmmisive.rgb *= 5.f;
	//Out.vColor = vAlbedo * vShade + vEmmisive;
	Out.vColor = pow(abs(vAlbedo * vShade + vEmmisive), 0.4545f);
	//Out.vColor = pow(abs(Out.vColor), 1 / Gamma);
	return Out;
}

PS_OUT PS_MAIN_TOOL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV); // pow(abs(tex2D(AlbedoSampler, In.vTexUV)), 1 / Gamma);
	vAlbedo *= 2.f;
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
	pass Blend_Tool
	{
		alphablendenable = true;
	srcblend = srcalpha;
	destblend = invsrcalpha;

	zwriteEnable = false;
	vertexShader = NULL;
	pixelshader = compile ps_3_0 PS_MAIN_TOOL();
	}
};