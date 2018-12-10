#define SOUND_DURATION 366
#define HEIGHT 0.02
#define PI 3.1415926

//-----------------------------------------------------------------------------------------------------------------------------------
//2D distance functions from iq's article: http://www.iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
float sdLine( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

float sdCircle( vec2 p, float r )
{
  return length(p) - r;
}

vec4 annularSector(vec2 uv, vec2 center, float minRad, float maxRad, int divideNum, vec3 color)
{
	vec2 v = normalize(uv - center);
	float theta = asin(v.y);
	theta = v.x > 0 ? theta : (-theta + PI);
	theta += iTime;

	float delta = 2.0*PI/divideNum;
	theta = mod(theta, delta);
	if (theta < delta * 0.2) return vec4(0.0);

	float d = length(uv - center);
	d = max(d - maxRad, minRad - d);

	float t = (d < 0 ? 1.0 : 0.1*minRad/d);
	return vec4(color, clamp(t, 0.0, 1.0));
}

vec4 renderPlaybackBar(vec2 uv)
{
	float playbackPos = iTime / SOUND_DURATION;

	vec3 baseLineCol = playbackPos > uv.x ? vec3(0.0, 0.8, 0.0) :  vec3(0.8, 0.0, 0.0);
	float alpha = 0.001 / sdLine(uv,vec2(0.0, HEIGHT),vec2(1.0, HEIGHT));
	vec4 lineCol = vec4(baseLineCol, alpha);

	float sd = sdCircle(uv-vec2(playbackPos, HEIGHT), 0.002);
	alpha = sd < 0 ? 1.0 : 0.002 / sd;
	vec4 circleCol = vec4(vec3(1.0), clamp(alpha, 0.0, 1.0));

	vec4 sectorCol = 2.0 * annularSector( uv, vec2(playbackPos, HEIGHT), 0.01+0.002*sin(iTime), 0.012+0.002*sin(iTime), 20, vec3(0.6) );
	sectorCol.a *= 0.2;

	vec4 col = lineCol;
	col = mix(col, circleCol, circleCol.a);
	col = mix(col, sectorCol, sectorCol.a);
	return col;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord / iResolution.x;
	fragColor = renderPlaybackBar(uv);
}