#version 430 core

uniform sampler2D uSceneTexture;

in vec2 _TexCoords;

out vec4 _outFragColor;

const float GAMMA = 2.2;
const float EXPOSURE = 1.0;

void main()
{
	vec3 HDRColor = texture(uSceneTexture, _TexCoords).rgb;

	//tone mapping
	//vec3 MappedColor = vec3(1.0) - exp(-HDRColor * EXPOSURE);
	vec3 MappedColor = HDRColor;

	//gamma correct
	MappedColor = pow(MappedColor, vec3(1.0 / GAMMA));

	vec3 col = HDRColor;

	//contrast
	col = clamp(col, 0.0, 1.0);
	col = col * col * (3.0 - 2.0 * col);

	//saturation (amplify colour, subtract grayscale)
	float sat = 0.2;
	col = col * (1.0 + sat) - sat * dot(col, vec3(0.33));

	_outFragColor = vec4(col, 1.0);
}