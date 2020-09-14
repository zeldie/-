matrix				g_matWorld;
matrix				g_matView;
matrix				g_matProj;

texture				g_DiffuseTexture;

sampler				DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
//AddressU = Clamp;
//AddressV = Clamp;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	
};

texture				g_NormalTexture;

sampler				NormalSampler = sampler_state
{
	texture = g_NormalTexture;

};

texture				g_SpecularTexture;

sampler				SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
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

float Gamma;