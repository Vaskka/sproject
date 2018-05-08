#version 430 core

uniform sampler2D uSceneTexture;

in vec2 _TexCoords;

layout(location = 0) out vec4 _outFragColor;

void main()
{
    _outFragColor = texture(uSceneTexture, _TexCoords);
}
