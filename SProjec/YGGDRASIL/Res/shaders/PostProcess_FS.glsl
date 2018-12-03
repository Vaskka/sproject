#version 460 core

uniform sampler2D	uSceneTexture;
uniform vec2		uWindowSize;

out vec4 _outFragColor;

const float GAMMA = 2.2;
const float EXPOSURE = 1.0;

void main()
{
	vec2 texCoord = gl_FragCoord.xy / uWindowSize;

	vec3 hdrColor = texture(uSceneTexture, texCoord).rgb;

	//tone mapping
	//vec3 mappedColor = vec3(1.0) - exp(-hdrColor * EXPOSURE);
	vec3 mappedColor = hdrColor;

	//gamma correct
	mappedColor = pow(mappedColor, vec3(1.0 / GAMMA));

	vec3 col = hdrColor;

	//contrast
	col = clamp(col, 0.0, 1.0);
	col = col * col * (3.0 - 2.0 * col);

	//saturation (amplify colour, subtract grayscale)
	float sat = 0.2;
	col = col * (1.0 + sat) - sat * dot(col, vec3(0.33));

	_outFragColor = vec4(col, 1.0);
}