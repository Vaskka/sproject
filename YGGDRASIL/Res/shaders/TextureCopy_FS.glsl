#version 430 core

uniform sampler2D uSceneTexture;

in vec2 _TexCoords;

layout(location = 0) out vec4 _outFragColor;

void main()
{
	vec2 TexCoords1 = vec2(gl_FragCoord.x / 1920.0f, gl_FragCoord.y / 1080.0f);
    _outFragColor = texture(uSceneTexture, TexCoords1);
}
