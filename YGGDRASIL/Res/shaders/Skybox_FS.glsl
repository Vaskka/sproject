#version 430 core

uniform samplerCube uEnvmap;

in vec3 _TexCoord;

out vec4 _outFragColor;

const float GAMMA = 2.2;
const float EXPOSURE = 1.0;

void main()
{
	vec3 HDRColor = texture(uEnvmap, _TexCoord).rgb;

	vec3 MappedColor = vec3(1.0) - exp(-HDRColor * EXPOSURE);
	MappedColor = pow(MappedColor, vec3(1.0 / GAMMA));

	_outFragColor = vec4(MappedColor, 1.0);
}