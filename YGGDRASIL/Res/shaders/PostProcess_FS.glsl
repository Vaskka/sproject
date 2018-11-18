#version 430 core

uniform sampler2D uSceneTexture;
uniform sampler2D uWhiteNoiseTex;
uniform vec3 uViewOrigin;
uniform vec3 uViewDirection;

in vec2 _TexCoords;

out vec4 _outFragColor;

const float GAMMA = 2.2;
const float EXPOSURE = 1.0;

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

#define terrainFbm(pos) fbm(pos, 4, 0.5)

vec2 map(in vec3 p) {
	float mountains = 19. * terrainFbm(p.xz*0.091);
	float trees = -.35 * terrainFbm(p.xz*10.);
	float rocks = -.002 * terrainFbm(p.xz*100.);
	float result = p.y + mountains + trees + rocks;

	return vec2(result, 1.0);
}

vec3 mapColour(in vec3 pos, in vec3 nor) {
	float darken = (1.0 - 0.5 * length(normalize(pos)));
	vec3 tint = vec3(.7, .7, .6);
	vec3 texture = texture(uWhiteNoiseTex, 0.006125*pos.xz, -100.).xyz;

	return  texture * tint;
}

vec2 raymarch(in vec3 ro, in vec3 rd) {
	vec2 h = vec2(0.001, 0.);
	float t = 0.;
	float tmax = 100.;

	for (int i = 0; i < 100; i++) {
		if (h.x < 0.0001 || t > tmax) break;
		h = map(ro + t * rd);
		t += 0.25 * h.x;
	}

	if (t > tmax) h.y = -1.;

	return vec2(t, h.y);
}

float shadow(in vec3 ro, in vec3 rd, in float maxt)
{
	float res = 1.0;
	float dt = 0.04;
	float t = .02;
	for (int i = 0; i < 20; i++)
	{
		float h = map(ro + rd * t).x;
		if (h < 0.001)
			return 0.0;
		res = min(res, maxt*h / t);
		t += h;
	}
	return res;
}

vec3 calcNormal(in vec3 pos, in float t)
{
	// show more detail the closer we are to the object
	vec3  eps = vec3(0.002 * t, 0.0, 0.0);
	vec3 nor;
	nor.x = map(pos + eps.xyy).x - map(pos - eps.xyy).x;
	nor.y = map(pos + eps.yxy).x - map(pos - eps.yxy).x;
	nor.z = map(pos + eps.yyx).x - map(pos - eps.yyx).x;
	return normalize(nor);
}

void main()
{
	vec3 HDRColor = texture(uSceneTexture, _TexCoords).rgb;

	//tone mapping
	//vec3 MappedColor = vec3(1.0) - exp(-HDRColor * EXPOSURE);
	vec3 MappedColor = HDRColor;

	//gamma correct
	MappedColor = pow(MappedColor, vec3(1.0 / GAMMA));

	vec3 col = vec3(0.0);
	vec3 ro = normalize(uViewOrigin);
	vec3 rd = normalize(uViewDirection);
	vec2 march = raymarch(ro, rd);

	vec3 light = normalize(vec3(0.9, 0.1, 0.9));
	vec3 ambient = 5. * vec3(0.1, 0.15, 0.2);
	float sundot = clamp(dot(rd, light), 0.0, 1.0);
	vec3 pos = ro + march.x * rd;

	// if we hit geometry
	if (march.y > 0.) {
		vec3 nor = calcNormal(pos, march.x);

		float lambert = clamp(dot(nor, light), 0., 1.);
		col = mapColour(pos, nor);
		col = mix(col, mapColour(pos, nor) * lambert, 0.8);

		//snow
		float snow = clamp(dot(normalize(nor), vec3(0., 1., 0.)), 0., 1.);
		snow = pow(snow, 5.);
		col = mix(col, vec3(1.)*snow, clamp(rd.y + 1., 0., 1.)*0.5);

		// fog
		float fo = 1. - exp(-0.04*march.x);
		vec3 fco = 0.9*vec3(0.5, 0.7, 0.9) + 0.1*vec3(1.0, 0.8, 0.5)*pow(sundot, 4.0);
		col = mix(col, fco, fo);

		float sh = shadow(pos, light, 10.);
		col = 0.8*col + 0.2* col* sh;// + ambient * (1.0 - sh);
	}
	else
	{
		col = HDRColor;
	}

	//contrast
	col = clamp(col, 0.0, 1.0);
	col = col * col * (3.0 - 2.0 * col);

	//saturation (amplify colour, subtract grayscale)
	float sat = 0.2;
	col = col * (1.0 + sat) - sat * dot(col, vec3(0.33));

	_outFragColor = vec4(col, 1.0);
}