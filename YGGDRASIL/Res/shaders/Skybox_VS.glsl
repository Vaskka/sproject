#version 430 core

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

layout (location = 0) in vec3 _inVertexPosition;

out vec3 _TexCoord;

void main()
{
	vec4 Pos = uProjectionMatrix * mat4(mat3(uViewMatrix)) * vec4(_inVertexPosition, 1.0);
	gl_Position = Pos.xyww;
	_TexCoord = _inVertexPosition;
}