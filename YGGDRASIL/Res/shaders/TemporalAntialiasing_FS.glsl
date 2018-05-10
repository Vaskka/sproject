#version 430 core

uniform sampler2D uHistorySceneTexture;
uniform sampler2D uCurrentSceneTexture;
uniform sampler2D uMotionVectorTexture;
uniform float uUseTAA;

in vec2 _TexCoords;
out vec4 _outFragColor;

vec3 mixColor(vec3 vHistoryColor, vec3 vCurrentColor);
vec3 clampHistoryColor(vec3 vHistoryColor, vec3 vCurrentColor, sampler2D vCurrentSceneSampler, vec2 vCurrentTexCoord);

void main()
{
	vec2 CurrentTexCoord = _TexCoords;
	vec2 HistoryTexCoord = _TexCoords + texture(uMotionVectorTexture, _TexCoords).xy;
	vec3 HistoryColor = texture(uHistorySceneTexture, HistoryTexCoord).rgb;
	vec3 CurrentColor = texture(uCurrentSceneTexture, CurrentTexCoord).rgb;

	HistoryColor = clampHistoryColor(HistoryColor, CurrentColor, uCurrentSceneTexture, CurrentTexCoord);
	vec3 FinalColor = vec3(0.0);

	if (uUseTAA > 0.5)
		FinalColor = mixColor(HistoryColor, CurrentColor);
	else
		FinalColor = CurrentColor;

	_outFragColor = vec4(FinalColor, 1.0);
}

vec3 minVec3(vec3 vLhs, vec3 vRhs)
{
	vec3 Result = vec3(1.0, 0.0, 0.0);
	Result.x = min(vLhs.x, vRhs.x);
	Result.y = min(vLhs.y, vRhs.y);
	Result.z = min(vLhs.z, vRhs.z);
	return Result;
}

vec3 maxVec3(vec3 vLhs, vec3 vRhs)
{
	vec3 Result = vec3(1.0, 0.0, 0.0);
	Result.x = max(vLhs.x, vRhs.x);
	Result.y = max(vLhs.y, vRhs.y);
	Result.z = max(vLhs.z, vRhs.z);
	return Result;
}

vec3 clampVec3(vec3 vVal, vec3 vMinVal, vec3 vMaxVal)
{
	vec3 Result = vec3(1.0, 0.0, 0.0);
	Result.x = clamp(vVal.x, vMinVal.x, vMaxVal.x);
	Result.y = clamp(vVal.y, vMinVal.y, vMaxVal.y);
	Result.z = clamp(vVal.z, vMinVal.z, vMaxVal.z);
	return Result;
}

vec3 clampHistoryColor(vec3 vHistoryColor, vec3 vCurrentColor, sampler2D vCurrentSceneSampler, vec2 vCurrentTexCoord)
{
	vec3 NeighborhoodMin = vCurrentColor;
	vec3 NeighborhoodMax = vCurrentColor;
	vec2 TextureSize = vec2(textureSize(vCurrentSceneSampler, 0));
	vec2 TexelSize = vec2(1.0) / TextureSize;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			vec2 NeighborhoodTexCoord = vCurrentTexCoord + vec2(x, y) * TexelSize;
			vec3 NeighborhoodColor = texture(vCurrentSceneSampler, NeighborhoodTexCoord).xyz;
			NeighborhoodMin = minVec3(NeighborhoodMin, NeighborhoodColor);
			NeighborhoodMax = maxVec3(NeighborhoodMax, NeighborhoodColor);
		}
	}

	vec3 Result = clampVec3(vHistoryColor, NeighborhoodMin, NeighborhoodMax);
	return Result;
}

vec3 mixColor(vec3 vHistoryColor, vec3 vCurrentColor)
{
	return mix(vHistoryColor, vCurrentColor, 0.02f);
}