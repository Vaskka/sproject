#version 430 core

uniform sampler2D uTextureDiffuse1;
uniform sampler2D uTextureSpecular1;
uniform float uMaterialShinness;
uniform vec3 uViewPos;

in vec3 _PositionW;
in vec3 _NormalW;
in vec2 _TexCoords;
 
out vec4 _outFragColor;

void main()
{
	const vec3 LightDirectionW = vec3(0, 0, -1);
	const vec3 LightAmbient = vec3(0.05, 0.05, 0.05);
	const vec3 LightDiffuse = vec3(0.8, 0.8, 0.8);
	const vec3 LightSpecular = vec3(0.7, 0.7, 0.7);

    // ambient
    vec3 Result = LightAmbient * texture(uTextureDiffuse1, _TexCoords).rgb;
  	
    vec3 Norm = normalize(_NormalW);
    vec3 LightDir = normalize(-LightDirectionW);  
    float t = max(dot(Norm, LightDir), 0.0);
	if (t > 0)
	{
		// diffuse 
		Result += LightDiffuse * texture(uTextureDiffuse1, _TexCoords).rgb * t;

		// specular
		vec3 ViewDir = normalize(uViewPos - _PositionW);
		vec3 ReflectDir = reflect(-LightDir, Norm);
		float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), uMaterialShinness);
		Result += LightSpecular * Spec * texture(uTextureSpecular1, _TexCoords).rgb;
	}
 
    _outFragColor = vec4(Result, 1.0);
}