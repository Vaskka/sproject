void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	vec4 layer1 = texture(iChannel0, uv); //background
	vec4 layer2 = texture(iChannel1, uv); //bubble
	vec4 outColor = mix(layer1, layer2, layer2.a);

	fragColor = outColor;
}