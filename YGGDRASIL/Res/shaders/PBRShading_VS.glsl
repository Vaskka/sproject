#version 430 core

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

uniform mat4 uHistoryProjectionMatrix;
uniform mat4 uHistoryViewMatrix;
uniform mat4 uCurrentProjectionMatrix;
uniform float uUseTAA;

layout(location = 0) in vec3 _inVertexPosition;
layout(location = 1) in vec3 _inVertexNormal;
layout(location = 2) in vec2 _inVertexTexCoord;

out vec3 _PositionW;
out vec3 _NormalW;
out vec2 _TexCoord;

out vec4 _HistoryPositionP;
out vec4 _CurrentPositionP;

void main()
{
	_PositionW = vec4(uModelMatrix * vec4(_inVertexPosition, 1.0)).xyz;
	_NormalW = mat3(transpose(inverse(uModelMatrix))) * _inVertexNormal;
	_TexCoord = _inVertexTexCoord;

	_CurrentPositionP = uCurrentProjectionMatrix * uViewMatrix * vec4(_PositionW, 1.0);
	_HistoryPositionP = uHistoryProjectionMatrix * uHistoryViewMatrix * vec4(_PositionW, 1.0);

	if (uUseTAA > 0.5)
		gl_Position = uProjectionMatrix * uViewMatrix * vec4(_PositionW, 1.0);
	else
		gl_Position = _CurrentPositionP;
}