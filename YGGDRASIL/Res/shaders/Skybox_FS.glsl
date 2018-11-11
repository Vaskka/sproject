#version 460 core

uniform sampler2D uWhiteNoiseTex;
uniform vec3 uSunDir = normalize(vec3(0.0, 0.1, 1.0));
uniform float uTime = 0.0;

in vec3 _TexCoord;

out vec4 _outFragColor;

float fbm(vec2 p)
{
	const mat2 m2 = mat2(0.8, -0.6, 0.6, 0.8);
	float f = 0.0;
	f += 0.5000*texture(uWhiteNoiseTex, p / 256.0, -100.).x; p = m2*p*2.02;
	f += 0.2500*texture(uWhiteNoiseTex, p / 256.0, -100.).x; p = m2*p*2.03;
	f += 0.1250*texture(uWhiteNoiseTex, p / 256.0, -100.).x; p = m2*p*2.01;
	f += 0.0625*texture(uWhiteNoiseTex, p / 256.0, -100.).x;
	return f / 0.9375;
}

//sky color based on https://www.shadertoy.com/view/MlSSR1. 
vec3 skyColor(in vec3 ray)
{
	vec3 col = vec3(0.0);

	//sun
	float sundot = clamp(dot(ray, uSunDir), 0.0, 1.0);
	col += 0.05 * vec3(0.9, 0.3, 0.9) * pow(sundot, 1.0);
	col += 0.1 * vec3(1.0, 0.7, 0.7) * pow(sundot, 2.0);
	col += 0.9 * vec3(1.0) * pow(sundot, 256.0);

	//sky color
	vec3 blueSky = vec3(0.3, 0.55, 0.8);
	vec3 redSky = vec3(0.8, 0.8, 0.6);
	vec3 skyColor = mix(blueSky, redSky, 1.5 * pow(sundot, 8.0));
	col += skyColor * (1.0 - 0.8 * ray.y);

	//fog        
	//col = mix(col, 0.9 * vec3(0.9, 0.85, 0.85), pow(1.0 - clamp(ray.y + 0.1, 0.0, 1.0), 4.0));

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
	col = mix(col, cloudColor, 0.5 * smoothstep(0.5, 0.8, fbm(0.0005 * sc + fbm(0.0005 * sc + uTime * cloudFlux))));
	//cloud layer 2
	sc = cloudSpeed * 30.0 * uTime * ro.xz + ray.xz*(500.0 - ro.y) / ray.y;
	col = mix(col, cloudColor, 0.5 * smoothstep(0.5, 0.8, fbm(0.0002 * sc + fbm(0.0005 * sc + uTime * cloudFlux))));

	//contrast
	col = clamp(col, 0.0, 1.0);
	col = col * col * (3.0 - 2.0 * col);

	//saturation (amplify colour, subtract grayscale)
	float sat = 0.2;
	col = col * (1.0 + sat) - sat * dot(col, vec3(0.33));

	return col;
}

void main()
{
	vec3 col = skyColor(normalize(_TexCoord));
	_outFragColor = vec4(col, 1.0);
}