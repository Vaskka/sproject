void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	vec3 col = texture(iChannel0, uv).rgb;
	col += texture(iChannel1, uv).rgb;
	col /= 2;
	
    fragColor = vec4(col, 1.0);
}