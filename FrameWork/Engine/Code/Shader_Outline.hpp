texture			g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

float3x3 convX = { -1.f, 0.f, 1.f,
					-2.f, 0.f, 2.f,
					-1.f, 0.f, 1.f };

float3x3 convY = { 1.f, 2.f, 1.f,
					0.f, 0.f, 0.f,
					-1.f, -2.f, -1.f };
float vPixelOffsetX;
float vPixelOffsetY;

float Mask[9] = { -1.f, -1.f, -1.f, -1.f, 8.f, -1.f, -1.f, -1.f, -1.f };
float coord[3] = { -1.f, 0.f, 1.f };
float fDivider = 1.f;
float MAP_CX = 1280.f;
float MAP_CY = 720.f;

struct PS_IN
{
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_OUTLINEREADY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	/*float lx = 0;
	float ly = 0;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			float2 offset = float2(x, y) * float2(vPixelOffsetX, vPixelOffsetY);
			float3 tex = tex2D(NormalSampler, In.vTexUV + offset).rgb;
			float luminace = dot(tex, float3(0.3, 0.59, 0.11));

			lx += luminace * convX[y + 1][x + 1];
			ly += luminace * convY[y + 1][x + 1];
		}
	}

	float l = sqrt((lx*lx) + (ly*ly));*/

	vector vColor = vector(0.f, 0.f, 0.f, 0.f);
	vector vRet;
	float3 GrayScale = float3(0.3, 0.59, 0.11);

	for (int i = 0; i < 9 ; i++)
		vColor += Mask[i] * (tex2D(NormalSampler, In.vTexUV + float2(coord[i % 3] / MAP_CX, coord[i / 3] / MAP_CY)));
	float gray = 1 - (vColor.r * 0.3f + vColor.g * 0.59f + vColor.b*0.11f);
	//Out.vColor = vector(gray, gray, gray, 1.f) / fDivider;
	if (gray < 0.8f)
		Out.vColor = vector(gray, gray, gray, 1.f) / fDivider;
	else
		Out.vColor = vector(1.f, 1.f, 1.f, 1.f);
	return Out;
}

technique Default_Device
{
	pass OutlineReady
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_OUTLINEREADY();
	}

};