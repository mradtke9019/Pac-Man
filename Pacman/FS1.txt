#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;



uniform float time;
uniform mat4 MVP;
uniform mat4 Identity;
uniform float rand;

const float ambientCoeff   = 0.5;
const float specularCoeff  = 0.1;
const float specularExp    = 2.0;

uniform vec3 LightColor;
uniform vec3 ObjectColor;
uniform vec3 LightPosition;
uniform vec3 LightDirection;
in vec3 FragPos;

void main()                                                             
{
    vec3 l = normalize(LightDirection);
	vec3 n = normalize(fNormal);
	vec3 e = normalize(-fPosition.xyz);
	vec3 h = normalize (e+l);

	vec3 dir = vec3(-.5,1,.5); //direction of color diffusion
	float diffuse = pow(dot(fNormal,dir),12.0);
	vec3 diffuseColor = diffuse * vec3(0.5,0.5,0.5);
	
	
	vec3 ambientColor = ambientCoeff * ObjectColor;
	vec3 specularColor = specularCoeff*pow(max(0.0,dot(n,h)),specularExp) * LightColor;
	
	
	gl_FragColor = vec4(ambientColor + specularColor +diffuseColor,1.0);
}