#version 430 core

uniform sampler2D uWhiteNoiseTex;
uniform vec3 uSunDir = normalize(vec3(.0, .1, 1.));

in vec3 _TexCoord;

out vec4 _outFragColor;

//based on https://www.shadertoy.com/view/MlSSR1. 
vec3 skyColorVer1(in vec3 ray)
{
	vec3 col = vec3(0.0);
	float sundot = clamp(dot(ray, uSunDir), 0.0, 1.0);

	//sky color
	vec3 blueSky = vec3(0.3, 0.55, 0.8);
	vec3 redSky = vec3(0.8, 0.8, 0.6);
	vec3 skyCol = mix(blueSky, redSky, 1.5 * pow(sundot, 8.0));
	col = skyCol * (1.0 - 0.8 * ray.y);

	//sun
	//col += 0.1 * vec3(0.9, 0.3, 0.9) * pow(sundot, 0.5);
	//col += 0.2 * vec3(1.0, 0.7, 0.7) * pow(sundot, 1.0);
	col += 9.0 * vec3(1.0, 0.6, 0.5) * pow(sundot, 256);

	//horizon        
	col = mix(col, 0.9 * vec3(0.9, 0.75, 0.8), pow(1.0 - clamp(ray.y + 0.1, 0.0, 1.0), 8.0));

	//Stars
	float s = texture(uWhiteNoiseTex, ray.xz * 1.25, -100.).x;
	s += texture(uWhiteNoiseTex, ray.xz* 4., -100.).x;
	s = pow(s, 17.0) * 0.00005 * max(ray.y, -0.2) * pow((1. - max(sundot, 0.)), 2.);
	if (s > .0)
	{
		vec3 backStars = vec3(s);
		col += backStars;
	}

	//contrast
	col = clamp(col, 0.0, 1.0);
	col = col * col * (3.0 - 2.0 * col);

	//saturation (amplify colour, subtract grayscale)
	float sat = 0.2;
	col = col * (1.0 + sat) - sat * dot(col, vec3(0.33));

	return col;
}

//based on https://www.shadertoy.com/view/lt2SR1
vec3 skyColorVer2(in vec3 ray)
{
	float yd = min(ray.y, 0.);
	ray.y = max(ray.y, 0.);

	vec3 col = vec3(0.);

	//Sky
	col += vec3(.4, .4 - exp(-ray.y*20.)*.3, .0) * exp(-ray.y*9.); // Red / Green 
	col += vec3(.3, .5, .8) * (1. - exp(-ray.y*8.)) * exp(-ray.y*.9); // Blue

	//Fog
	col = mix(col*1.2, vec3(.3), 1. - exp(yd*100.));

	//Sun
	float sundot = clamp(dot(ray, uSunDir), 0.0, 1.0);
	col += vec3(1.0, .8, .55) * pow(sundot, 15.) * .6;
	col += pow(max(dot(ray, uSunDir), 0.), 256.0) * 0.5;

	//Stars
	float s = texture(uWhiteNoiseTex, ray.xz * 1.25, -100.).x;
	s += texture(uWhiteNoiseTex, ray.xz* 4., -100.).x;
	s = pow(s, 17.0) * 0.00005 * max(ray.y, -0.2) * pow((1. - max(sundot, 0.)), 2.);
	if (s > .0)
	{
		vec3 backStars = vec3(s);
		col += backStars;
	}

	//Contrast
	col = clamp(col, 0.0, 1.0);
	col = col * col * (3.0 - 2.0 * col);

	//Saturation (amplify colour, subtract grayscale)
	float sat = 0.2;
	col = col * (1.0 + sat) - sat * dot(col, vec3(0.33));

	return col;
}

void main()
{
	vec3 col = skyColorVer2(normalize(_TexCoord));
	_outFragColor = vec4(col, 1.0);
}