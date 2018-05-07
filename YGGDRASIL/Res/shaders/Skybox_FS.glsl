#version 430 core

uniform samplerCube uEnvmap;

in vec3 _TexCoord;

out vec4 _outFragColor;

void main()
{
	_outFragColor = texture(uEnvmap, _TexCoord);
}