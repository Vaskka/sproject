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
	vec3 MappedColor = vec3(1.0) - exp(-HDRColor * EXPOSURE);

	//gamma correct
	MappedColor = pow(MappedColor, vec3(1.0 / GAMMA));

	_outFragColor = vec4(MappedColor, 1.0);
}