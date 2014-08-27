#version 330
//#version 120

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

uniform mat4 WVP;
uniform mat4 W;
uniform mat4 V;

// Shading-related output data
out vec3 vertexPos_worldspace;
out vec3 eyeDir_cameraspace;
out vec3 lightPos_worldspace;
out vec3 lightDir_cameraspace;
out vec3 normal_cameraspace;
out float zDepth;

out vec2 UV;

out vec4 pointCol;

void main()
{
	vec4 vertexPos_modelspace = vec4(vertexPosition, 1.0);
	vec4 normal_modelspace = vec4(vertexNormal, 1.0);
	
	pointCol = vec4(1.0, 1.0, 1.0, 1.0);
	
    // Output of vertex in clip space
	gl_Position = WVP * vertexPos_modelspace;
	
    // Shading data.
    vertexPos_worldspace = (W * vertexPos_modelspace).xyz;

    // Vector from vertex to camera (origin 0,0,0 in camera space)
	vec3 vertexPos_cameraspace = (V * W * vertexPos_modelspace).xyz;
	eyeDir_cameraspace = vec3(0,0,0) - vertexPos_cameraspace;
	
	zDepth = vertexPos_cameraspace.z;
	
    // This should be gathered from camera node.
    // Light transformations should be multiplied on the CPU, unless
    // I want to add a LightWorldMatrix uniform.
    //lightPos_worldspace = (LightWorldMat*vec4(5.0, 5.0, 13.5, 1.0)).xyz;
    lightPos_worldspace = (vec4(5.0, 5.0, 13.5, 1.0)).xyz;
	
	// Vector from vertex to the light (hardcoded light pos for now)
	vec3 lightPos_cameraspace = (V * vec4(lightPos_worldspace,1)).xyz;
	lightDir_cameraspace = lightPos_cameraspace + vertexPos_cameraspace;
	
	// Normal of the vertex in camer space
	normal_cameraspace = (V * W * normal_modelspace).xyz;
	
	UV.x = vertexUV.x;
	UV.y = 1.0 - vertexUV.y;
}

