#version 460 core

uniform sampler2D uWhiteNoiseTex;
uniform float uTime = 0.0;

in vec3 _TexCoord;
out vec4 _outFragColor;

#define DAY_TIME_PERIOD 240.0
#define PI				3.1415926

float noise(vec2 pos) { return texture(uWhiteNoiseTex, pos / 256.0).x; }

float fbm(vec2 pos, int octaves, float persistence)
{
	float total = 0.0, frequency = 1.0, amplitude = 1.0, maxValue = 0.0;
	for (int i = 0; i < octaves; ++i)
	{
		total += noise(pos * frequency) * amplitude;
		maxValue += amplitude;
		amplitude *= persistence;
		frequency *= 2.0;
	}
	return total / maxValue;
}

#define cloudFbm(pos) fbm(pos, 4, 0.5)

vec3 calculateSunDirection(float globalTime)
{
	float dayTime = mod(globalTime, DAY_TIME_PERIOD) / DAY_TIME_PERIOD;
	float azimuthal = dayTime * PI * 2;
	float polar = PI * (0.5 - 0.3 * sin(2 * PI * dayTime));
	vec3 sunDir = normalize(vec3(sin(polar) * sin(azimuthal), cos(polar), -sin(polar) * cos(azimuthal)));
	return sunDir;
}

//sky color based on https://www.shadertoy.com/view/MlSSR1. 
vec3 renderSky(vec3 ray)
{
	vec3 col = vec3(0.0);

	//sun
	vec3 sunDir = calculateSunDirection(uTime);
	float sundot = clamp(dot(ray, sunDir), 0.0, 1.0);
	col += 0.05 * vec3(0.9, 0.3, 0.9) * pow(sundot, 1.0);
	col += 0.1 * vec3(1.0, 0.7, 0.7) * pow(sundot, 2.0);
	col += 0.9 * vec3(1.0) * pow(sundot, 256.0);

	//sky color
	vec3 blueSky = vec3(0.3, 0.55, 0.8);
	vec3 redSky = vec3(0.8, 0.8, 0.6);
	vec3 skyColor = mix(blueSky, redSky, 1.5 * pow(sundot, 8.0));
	col += skyColor * (1.0 - 0.8 * ray.y);

	//fog        
	//col = mix(col, 0.9 * vec3(1.0, 0.95, 1.0), pow(1.0 - clamp(ray.y + 0.1, 0.0, 1.0), 4.0));

	//stars
	float s = texture(uWhiteNoiseTex, ray.xz * 1.25, -100.0).x;
	s += texture(uWhiteNoiseTex, ray.xz * 4.0, -100.0).x;
	s = pow(s, 17.0) * 0.00005 * max(ray.y, -0.2) * pow((1.0 - max(sundot, 0.0)), 2.0);
	if (s > 0.0) { vec3 backStars = vec3(s); col += backStars; }

	//clouds 
	float cloudSpeed = 0.01;
	float cloudFlux = 0.5;
	//cloud layer 1
	vec3 cloudColor = mix(vec3(1.0, 0.95, 1.0), 0.35 * redSky, pow(sundot, 2.0));
	vec3 ro = 10.0 * ray;
	vec2 sc = cloudSpeed * 50.0 * uTime * ro.xz + ray.xz * (1000.0 - ro.y) / ray.y;
	col = mix(col, cloudColor, 0.5 * smoothstep(0.5, 0.8, cloudFbm(0.0005 * sc + cloudFbm(0.0005 * sc + uTime * cloudFlux))));
	//cloud layer 2
	sc = cloudSpeed * 30.0 * uTime * ro.xz + ray.xz*(500.0 - ro.y) / ray.y;
	col = mix(col, cloudColor, 0.5 * smoothstep(0.5, 0.8, cloudFbm(0.0002 * sc + cloudFbm(0.0005 * sc + uTime * cloudFlux))));

	return col;
}

void main()
{
	vec3 col = renderSky(normalize(_TexCoord));
	_outFragColor = vec4(col, 1.0);
}