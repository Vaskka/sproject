#define SOUND_DURATION 366
#define HEIGHT 0.1
#define PI 3.1415926

vec4 circle(vec2 uv, vec2 pos, float rad, vec3 color) 
{
	float d = length(pos - uv) - rad;
	float t = (d < 0 ? 1.0 : rad / d);
	t = clamp(t, 0.0, 1.0);
	return vec4(color, t);
}

vec4 rangeCircle(vec2 uv, vec2 pos, float minRad, float maxRad, int divideNum, vec3 color)
{
	vec2 v = normalize(pos - uv);
	float theta = asin(v.y);
	theta = v.x > 0 ? theta : (-theta + PI);
	theta += iTime;

	float delta = 2.0*PI/divideNum;
	theta = mod(theta, delta);
	if (theta < delta * 0.2) return vec4(0.0);

	float d = length(pos - uv);
	d = max(d - maxRad, minRad - d);

	float t = (d < 0 ? 1.0 : 0.1 * minRad / d);
	t = clamp(t, 0.0, 1.0);

	return vec4(color, t);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord.xy / iResolution.y;
	float playbackPos = iTime / SOUND_DURATION;

	vec3 baseCol = playbackPos > uv.x ? vec3(0.4, 0.8, 0.0) :  vec3(0.8, 0.4, 0.0);
	vec4 lineCol = 1.0 * vec4(baseCol, 0.5 * (0.002/abs(uv.y-HEIGHT)));

	vec4 circleCol = 2.0 * circle(uv, vec2(playbackPos, HEIGHT), 0.006, vec3(0.0, 0.6, 1.0));
	circleCol.a *= 0.4;
	vec4 circleCol2 = 2.0 * rangeCircle(uv, vec2(playbackPos, HEIGHT), 0.022+0.002*sin(iTime), 0.024+0.002*sin(iTime), 5, vec3(0.8));
	circleCol2.a *= 0.2;

	vec4 col = lineCol;
	col = mix(col, circleCol, circleCol.a);
	col = mix(col, circleCol2, circleCol2.a);

	fragColor = col;
}