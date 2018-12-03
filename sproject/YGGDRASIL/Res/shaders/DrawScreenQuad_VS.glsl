#version 460 core

layout(location = 0) in vec2 _inFragPosition;

void main()
{
	gl_Position = vec4(_inFragPosition, 0.0, 1.0);
}