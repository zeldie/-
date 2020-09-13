#include "Shader_Base.hpp"

struct Mtrl //���� ǥ�� ����
{
	float4 vambient;
	float4 vdiffuse;
	float4 vspec;
	float  vspecPower;
};

struct DirLight //���⼺ ���� ����ü
{
	float4 vambient;
	float4 vdiffuse;
	float4 vspec;
	float3 vdirW;
};

matrix		g_WorldInv; // ���忪���
Mtrl		g_Mtrl; 
DirLight	g_Light;
float3		vCameraPos; //ī�޶� ��ġ
float2		vWaveMapOffset0; // ���� �븻�� ������0
float2		vWaveMapOffset1; // ���� �븻�� ������1
float		fRefractBias;//���� ����
float		fRefractPower; // ���� ����
float2		vRippleScale; // �ܹ��� ����
vector		vFog;
vector		vFogColor;

texture g_WaveNormalTexture0;
sampler WaveNormalSampler0 = sampler_state
{
	texture = g_WaveNormalTexture0;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

texture g_WaveNormalTexture1;
sampler WaveNormalSampler1 = sampler_state
{
	texture = g_WaveNormalTexture1;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

texture g_ReflectTexture;
sampler ReflectSampler = sampler_state
{
	texture = g_ReflectTexture;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture g_RefractTexture;
sampler RefractSampler = sampler_state
{
	texture = g_RefractTexture;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};


struct VS_IN
{
	float3 Position : POSITION;
	float2 Tex : TEXCOORD0;
};

struct VS_OUT
{
	float4 Position : POSITION;
	float3 TangentEye : TEXCOORD0;
	float3 TangentLightDir : TEXCOORD1;
	float2 Tex0 : TEXCOORD2;
	float2 Tex1 : TEXCOORD3;
	float4 ProjTex : TEXCOORD4;
	float EyeDist : TEXCOORD5;
	float Fog : TEXCOORD6;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// Build TBN-basis.  For flat water grid in the xz-plane in 
	// object space, the TBN-basis has a very simple form.
	//TBN ����� �������� �ϱ� ���� TBN ����� ������
	float3x3 TBN;
	TBN[0] = float3(1.0f, 0.0f, 0.0f); // Tangent goes along object space x-axis. //������ ��ü���� X������
	TBN[1] = float3(0.0f, 0.0f, -1.0f);// Binormal goes along object space -z-axis //�������� ��ü���� -Z������
	TBN[2] = float3(0.0f, 1.0f, 0.0f); // Normal goes along object space y-axis //������ ��ü���� y������

									   // Matrix transforms from object space to tangent space.
									   // ��ü�������� ������������ ����� ��ȯ�ض�.
	float3x3 toTangentSpace = transpose(TBN);

	// Transform eye position to local space.
	// ī�޶� �������� �������������� ��ȯ�ض�
	float3 localEyePosition = mul(float4(vCameraPos, 1.0f), g_WorldInv).xyz;
	Out.EyeDist = distance(localEyePosition, In.Position); //���� ī�޶� �����ǰ� ���� �������� �Ÿ����� ���Ѵ�.

																 // Transform to-eye vector to tangent space.
																 // ī�޶� ���͸� ź��Ʈ�������� ��ȯ�ض�
	float3 toEyeL = localEyePosition - In.Position;
	Out.TangentEye = mul(toEyeL, toTangentSpace);

	// Transform light direction to tangent space.
	//�� ���Ⱚ�� ź��Ʈ �������� ��ȯ�ض�
	float3 localLightDirection = mul(float4(g_Light.vdirW, 0.0f), g_WorldInv).xyz;
	Out.TangentLightDir = mul(localLightDirection, toTangentSpace);

	// Transform to homogeneous clip space.
	//������������� ��ķ� ���� ��ġ�� ���Ѵ�.
	Out.Position = mul(float4(In.Position, 1.0f), matWVP);

	// Scroll texture coordinates.
	//�ؽ����� ��ǥ�� �����¸�ŭ ��ũ���ض�
	Out.Tex0 = In.Tex + vWaveMapOffset0;
	Out.Tex1 = In.Tex + vWaveMapOffset1;

	// Generate projective texture coordinates from camera's perspective.
	// ī�޶��� �������� �����ؽ��� ��ǥ�� �����Ѵ�.
	Out.ProjTex = Out.Position;

	Out.Fog = vFog.x + Out.Position.z*vFog.y; //��������

	if (Out.Fog < 0.f)
		Out.Fog = 0.f;
	else if (Out.Fog > 1.f)
		Out.Fog = 1.f;

	// Done--return the output.
	//����ü�� ��ȯ�ض�~
	return Out;
}


float4 PS_MAIN(VS_OUT Input) : COLOR
{
	//ź��Ʈ������ �ִ� ī�޶�, ���� ������ ����ȭ�Ѵ�.
	float3 toEyeT = normalize(Input.TangentEye);
	float3 lightDirT = normalize(Input.TangentLightDir);
	
	// Light vector is opposite the direction of the light.
	float3 lightVecT = -lightDirT;
	
	// Sample normal map.
	//�븻���� ���ø��� ���� �����Ѵ�.
	float3 normalT0 = tex2D(WaveNormalSampler0, Input.Tex0).rgb;
	float3 normalT1 = tex2D(WaveNormalSampler1, Input.Tex1).rgb;
	
	// Expand from [0, 1] compressed interval to true [-1, 1] interval.
	//[0,1] �ؽ��İ��� [-1, 1] ���������� �ű��.
	normalT0 = 2.0f*normalT0 - 1.0f;
	normalT1 = 2.0f*normalT1 - 1.0f;
	
	// Average the two vectors.
	//�� ������ ��հ��� ���Ѵ�(����ȭ ����)
	float3 normalT = normalize(0.5f*(normalT0 + normalT1));
	
	// Compute the reflection vector.
	//�ݻ纤�͸� ����Ѵ�.(�ݻ纤�͸� ���Ҷ��� �������� �� ���͸� ����ؾ� ������ ������ -���� ��⶧���� �ٽ� -���� �ش�)
	float3 r = reflect(-lightVecT, normalT);
	
	// Determine how much (if any) specular light makes it into the eye.
	//�ݻ������� �󸶳� ������ �����ض�! (����ŧ�����⿡ ���� �޶���)
	float t = pow(max(dot(r, toEyeT), 0.0f), g_Mtrl.vspecPower);
	
	// Determine the diffuse light intensity that strikes the vertex.
	// ������ �ε�ĥ Ȯ�걤�� ���⸦ �����Ѵ�.
	float s = max(dot(lightVecT, normalT), 0.0f);
	
	// If the diffuse light intensity is low, kill the specular lighting term.
	// It doesn't look right to add specular light when the surface receives 
	// little diffuse light.
	//���� Ȯ�걤�� ���Ⱑ 0���� �������� ����ŧ���� �ݻ���� �����Ƿ� ����ŧ���� 0���� �����.
	if (s <= 0.0f)
	t = 0.0f;
	
	// Project the texture coordinates and scale/offset to [0,1].
	// �ؽ��� ��ǥ�� ������/�������� [0,1]�� �����Ѵ�.
	Input.ProjTex.xy /= Input.ProjTex.w;
	Input.ProjTex.x = 0.5f*Input.ProjTex.x + 0.5f;
	Input.ProjTex.y = -0.5f*Input.ProjTex.y + 0.5f;
	
	// To avoid clamping artifacts near the bottom screen edge, we 
	// scale the perturbation magnitude of the v-coordinate so that
	// when v is near the bottom edge of the texture (i.e., v near 1.0),
	// it doesn't cause much distortion.  The following power function
	// scales v very little until it gets near 1.0.
	// (Plot this function to see how it looks.)
	float vPerturbMod = -pow(abs(Input.ProjTex.y), 10.0f) + 1.0f;
	
	// Sample reflect/refract maps and perturb texture coordinates.
	// �ݻ�/������ �� �����ؽ��� ���ø�
	float2 perturbVec = normalT.xz*vRippleScale;
	
	perturbVec.y *= vPerturbMod;
	float3 reflectCol = tex2D(ReflectSampler, Input.ProjTex.xy + perturbVec).rgb;
	float3 refractCol = tex2D(RefractSampler, Input.ProjTex.xy + perturbVec).rgb;
	
	// Refract based on view angle.
	// �þ߰��� �������� ����
	float refractWt = saturate(fRefractBias + pow(max(dot(toEyeT, normalT), 0.0f), fRefractPower));
	
	// Weighted average between the reflected color and refracted color, modulated
	// with the material.
	// �ݻ�� ����� ������ ���� ������ ����ġ ��հ��� ��ȯ
	float3 ambientMtrl = g_Mtrl.vambient*lerp(reflectCol, refractCol, refractWt);
	float3 diffuseMtrl = g_Mtrl.vdiffuse*lerp(reflectCol, refractCol, refractWt);
	
	// Compute the ambient, diffuse and specular terms separatly.
	//�ֺ���, Ȯ�걤, ���ݻ籤 ���
	float3 spec = t*(g_Mtrl.vspec*g_Light.vspec).rgb;
	float3 diffuse = saturate(diffuseMtrl*g_Light.vdiffuse.rgb);
	float3 ambient = ambientMtrl*g_Light.vambient;
	
	float3 final = diffuse + spec;
	
	//���װ� ����Ǳ��� ���� Į��
	float4 Color = float4(final, g_Mtrl.vdiffuse.a);
	//���װ� ����� ���� Į��
	float4 finalColor = Input.Fog*Color + (1 - Input.Fog)*vFogColor;
	
	// Output the color and the alpha.
	return finalColor;
}

technique Default_Device
{
	pass P0
	{
		vertexShader = compile vs_3_0 VS_MAIN();
		pixelShader = compile ps_3_0 PS_MAIN();
	}
};