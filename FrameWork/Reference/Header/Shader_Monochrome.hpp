#include "Shader_Base.hpp"

float3 LuminanceConv = { 0.2125f, 0.7154f, 0.0721f };

struct	PS_IN
{
	float2			vTexUV : TEXCOORD0;
};


struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MONOCHROME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = dot((float3)tex2D(DiffuseSampler, In.vTexUV), LuminanceConv);
	
	return Out;
}

technique Default_Device
{
	pass Shadow
	{
		zwriteEnable = false;
		vertexShader = NULL;
		pixelshader = compile ps_3_0 PS_MONOCHROME();
	}


};