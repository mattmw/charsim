//#version 330
#version 120
#extension GL_EXT_gpu_shader4 : enable
const int TRANSFORMATIONS = 8;

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexUV;
attribute ivec4 boneId;
attribute vec4 boneWeight;
attribute vec3 morphPosition0;
attribute vec3 morphNormal0;
attribute vec3 morphPosition1;
attribute vec3 morphNormal1;

uniform mat4 WVP;
uniform mat4 W;
uniform mat4 V;
uniform mat4 boneTransformations[TRANSFORMATIONS];
uniform float morphWeight0;
uniform float morphWeight1;

vec4 skinnedVertexPos_modelspace;
vec4 skinnedNormal_modelspace;

// Shading-related output data
varying vec3 vertexPos_worldspace;
varying vec3 eyeDir_cameraspace;
varying vec3 lightPos_worldspace;
varying vec3 lightDir_cameraspace;
varying vec3 normal_cameraspace;
varying float zDepth;

varying vec2 UV;

varying vec4 pointCol;

void main()
{
    // Blend base vertex and normal with morph target vertex and normal.
    vec3 morphPosition = vertexPosition;
    // morphPosition = (morphPosition + morphWeight0 * (morphPosition0 - morphPosition));
    // morphPosition = (morphPosition + morphWeight1 * (morphPosition1 - morphPosition));
    morphPosition += morphWeight0 * morphPosition0;
    morphPosition += morphWeight1 * morphPosition1;
        
    vec3 morphNormal = vertexNormal;
//    morphNormal = (morphNormal + morphWeight0 * (morphNormal0 - morphNormal));
//    morphNormal = (morphNormal + morphWeight1 * (morphNormal1 - morphNormal));
    morphNormal += morphWeight0 * morphNormal0;
    morphNormal += morphWeight1 * morphNormal1;
    morphNormal = normalize(morphNormal);

	vec4 vertexPos_modelspace = vec4(morphPosition, 1.0); // Transform to 4D homogenous vector.
	vec4 normal_modelspace = vec4(morphNormal, 0.0);

    // Apply skinning to vertex and normal.
    skinnedVertexPos_modelspace = (boneTransformations[boneId[0]] * vertexPos_modelspace) * boneWeight[0];
	skinnedVertexPos_modelspace = (boneTransformations[boneId[1]] * vertexPos_modelspace) * boneWeight[1] + skinnedVertexPos_modelspace;
	skinnedVertexPos_modelspace = (boneTransformations[boneId[2]] * vertexPos_modelspace) * boneWeight[2] + skinnedVertexPos_modelspace;
	skinnedVertexPos_modelspace = (boneTransformations[boneId[3]] * vertexPos_modelspace) * boneWeight[3] + skinnedVertexPos_modelspace;
	
	skinnedNormal_modelspace = (boneTransformations[boneId[0]] * normal_modelspace) * boneWeight[0];
	skinnedNormal_modelspace = (boneTransformations[boneId[1]] * normal_modelspace) * boneWeight[1] + skinnedNormal_modelspace;
	skinnedNormal_modelspace = (boneTransformations[boneId[2]] * normal_modelspace) * boneWeight[2] + skinnedNormal_modelspace;
	skinnedNormal_modelspace = (boneTransformations[boneId[3]] * normal_modelspace) * boneWeight[3] + skinnedNormal_modelspace;
	
	vertexPos_modelspace = skinnedVertexPos_modelspace;
	normal_modelspace = skinnedNormal_modelspace;
	
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

