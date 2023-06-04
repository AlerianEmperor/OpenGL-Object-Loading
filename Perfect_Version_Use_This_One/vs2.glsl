#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 mvp_matrix;
uniform mat4 normal_matrix;


struct Light
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

uniform Light light;


out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;

out vec3 varyingHalfVector;



void main()
{
	varyingVertPos = (mv_matrix * vec4(position, 1.0f)).xyz; //gl_Position.xyz; //(mvp_matrix * vec4(position, 1.0f)).xyz;

	gl_Position = mvp_matrix * vec4(position, 1.0);
	
	//light.position = (vec4(light.position, 1.0f) * mvp_matrix).xyz;

	vec3 light_pos = (vec4(light.position, 1.0f) * mv_matrix).xyz;

	varyingLightDir = normalize(light_pos - varyingVertPos);//light.position - varyingVertPos;

	varyingNormal = (normal_matrix * vec4(normal, 1.0f)).xyz;

	varyingHalfVector = (varyingLightDir - varyingVertPos).xyz;

}