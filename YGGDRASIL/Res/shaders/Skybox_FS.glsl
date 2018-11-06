#version 430 core

in vec3 _TexCoord;

out vec4 _outFragColor;

//simple sky rendering based on https://www.shadertoy.com/view/MlSSR1. 
vec3 renderSky(in vec3 ray)
{
	vec3 col = vec3(0.0);
	vec3 light = normalize(vec3(0.9, 0.1, 0.9));
	float sundot = clamp(dot(ray, light), 0.0, 1.0);

	//sky color
	vec3 blueSky = vec3(0.3, 0.55, 0.8);
	vec3 redSky = vec3(0.8, 0.8, 0.6);
	vec3 skyCol = mix(blueSky, redSky, 1.5 * pow(sundot, 8.0));
	col = skyCol * (1.0 - 0.8 * ray.y);

	//sun
	col += 0.1 * vec3(0.9, 0.3, 0.9) * pow(sundot, 0.5);
	col += 0.2 * vec3(1.0, 0.7, 0.7) * pow(sundot, 1.0);
	col += 0.95 * vec3(1.0) * pow(sundot, 256.0);

	//horizon        
	col = mix(col, 0.9 * vec3(0.9, 0.75, 0.8), pow(1.0 - clamp(ray.y + 0.1, 0.0, 1.0), 8.0));

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
	vec3 col = renderSky(normalize(_TexCoord));
	_outFragColor = vec4(col, 1.0);
}