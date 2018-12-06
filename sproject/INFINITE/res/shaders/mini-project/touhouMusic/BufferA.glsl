void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec3 col = texture(iChannel0, fragCoord.xy / iResolution.xy).rgb;
	
	//gamma correction
	col = pow(col, vec3(1.0 / 2.2));

    fragColor = vec4(col, 1.0);
}