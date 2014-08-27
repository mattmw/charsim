#version 330
//#version 120

layout (location = 0) in vec3 vposition;
uniform mat4 WVP;

void main()
{
	vec4 v = vec4(vposition, 1); // Transform to 4D homogenous vector.
	gl_Position = WVP * v;
}
