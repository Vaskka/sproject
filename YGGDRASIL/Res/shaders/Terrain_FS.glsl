#version 460 core

uniform sampler2D uWhiteNoiseTex;
uniform vec3 uCameraPosition;
uniform mat4 uViewMatrix;

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

vec3 render(in vec3 ro, in vec3 rd)
{
	vec2 march = raymarch(ro, rd);
	if (march.y < 0.0) discard;

	vec3 col = vec3(0.0);
	vec3 light = normalize(vec3(0.9, 0.1, 0.9));
	vec3 ambient = 5. * vec3(0.1, 0.15, 0.2);
	float sundot = clamp(dot(rd, light), 0.0, 1.0);
	vec3 pos = ro + march.x * rd;

	vec3 nor = calcNormal(pos, march.x);

	float lambert = clamp(dot(nor, light), 0., 1.);
	col = mapColour(pos, nor);
	col = mix(col, mapColour(pos, nor) * lambert, 0.8);

	// fog
	float fo = 1. - exp(-0.04*march.x);
	vec3 fco = 0.9*vec3(0.5, 0.7, 0.9) + 0.1*vec3(1.0, 0.8, 0.5)*pow(sundot, 4.0);
	col = mix(col, fco, fo);

	float sh = shadow(pos, light, 10.);
	col = 0.8*col + 0.2* col* sh;// + ambient * (1.0 - sh);

	return col;
}

void main()
{
	vec2 uv = 2.0 * _TexCoords - 1.0;
	vec3 ro = uCameraPosition;
	vec3 rd = normalize(vec3(uv, -1.0)) * mat3(uViewMatrix);

	_outFragColor = vec4(render(ro, rd), 1.0);
}