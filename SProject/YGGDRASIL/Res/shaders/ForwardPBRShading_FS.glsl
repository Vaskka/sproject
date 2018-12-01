#version 430 core

uniform sampler2D uTextureAlbedo1;
uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;

uniform vec3 uEyePositionW;
uniform vec3 uLightDirectionW = vec3(0, 0, -1);
uniform vec3 uLightColor = vec3(5.0, 5.0, 5.0);

in vec3 _PositionW;
in vec3 _NormalW;
in vec2 _TexCoord;

layout(location = 0) out vec4 _outFragColor;

const float PI = 3.1415926;
const float GAMMA = 2.2;

float distributionGGX(vec3 N, vec3 H, float vRoughness);
float geometrySchlickGGX(float NdotV, float vRoughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float vRoughness);
vec3 fresnelSchlick(float vCosTheta, vec3 vF0);
vec3 fresnelSchlickRoughness(float vCosTheta, vec3 vF0, float vRoughness);
vec3 shadingWithUnrealModelForParallelLight(vec3 vAlbedo, float vMetallic, float vRoughness, vec3 vNormalW, vec3 vLightColor, vec3 vViewDirW, vec3 vLightDirW);

void main()
{
	vec3 Albedo = pow(texture(uTextureAlbedo1, _TexCoord).xyz, vec3(GAMMA));
	float Metallic = 0.1;
	float Roughness = 0.2;

	vec3 NormalW = normalize(_NormalW);
	vec3 LightDirW = normalize(-uLightDirectionW);
	vec3 ViewDirW = normalize(uEyePositionW - _PositionW);

	vec3 LoWithParallelLight = shadingWithUnrealModelForParallelLight(Albedo, Metallic, Roughness, NormalW, uLightColor, ViewDirW, LightDirW);
	vec3 Lo = LoWithParallelLight + vec3(0.1);

	_outFragColor = vec4(Lo, 1.0);
}

vec3 shadingWithUnrealModelForParallelLight(vec3 vAlbedo, float vMetallic, float vRoughness, vec3 vNormalW, vec3 vLightColor, vec3 vViewDirW, vec3 vLightDirW)
{
	const vec3 Radiance = vLightColor;
	const vec3 HalfDir = normalize(vViewDirW + vLightDirW);
	const vec3 F0 = mix(vec3(0.04), vAlbedo, vMetallic);

	float NDF = distributionGGX(vNormalW, HalfDir, vRoughness);
	float G = geometrySmith(vNormalW, vViewDirW, vLightDirW, vRoughness);
	vec3  F = fresnelSchlick(max(dot(HalfDir, vViewDirW), 0.0), F0);

	vec3 Nominator = NDF * G * F;
	float Denominator = 4.0 * max(dot(vNormalW, vViewDirW), 0.0) * max(dot(vNormalW, vLightDirW), 0.0) + 0.001;
	vec3 Specular = Nominator / vec3(Denominator);

	vec3 Ks = F;
	vec3 Kd = (1.0 - vMetallic) * (vec3(1.0) - Ks);
	float NdotL = max(dot(vNormalW, vLightDirW), 0.0);
	vec3 Lo = (Kd * vAlbedo / vec3(PI) + Specular) * Radiance * vec3(NdotL);
	return Lo;
}

float distributionGGX(vec3 N, vec3 H, float vRoughness)
{
	float A = vRoughness * vRoughness;
	float A2 = A * A;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float Nom = A2;
	float Denom = (NdotH2 * (A2 - 1.0) + 1.0);
	Denom = PI * Denom * Denom;

	return Nom / Denom;
}

float geometrySchlickGGX(float NdotV, float vRoughness)
{
	float R = (vRoughness + 1.0);
	float K = (R * R) / 8.0;

	float Nom = NdotV;
	float Denom = NdotV * (1.0 - K) + K;

	return Nom / Denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float vRoughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = geometrySchlickGGX(NdotV, vRoughness);
	float ggx1 = geometrySchlickGGX(NdotL, vRoughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float vCosTheta, vec3 vF0)
{
	return vF0 + (1.0 - vF0) * pow(1.0 - vCosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float vCosTheta, vec3 vF0, float vRoughness)
{
	return vF0 + (max(vec3(1.0 - vRoughness), vF0) - vF0) * pow(1.0 - vCosTheta, 5.0);
}