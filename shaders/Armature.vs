#version 330
//#version 120
const int TRANSFORMATIONS = 8;

layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in ivec4 bone_id;
layout (location = 4) in vec4 bone_weight;
uniform mat4 WVP;
uniform mat4 W;
uniform mat4 V;
uniform mat4 bone_transformations[TRANSFORMATIONS];
uniform mat4 rot_transformation;

mat4 new_transformation = mat4(1.0);
vec4 skinned_vertex_pos_modelspace;
vec4 skinned_normal_modelspace;

// Shading-related output data
out vec3 vertex_pos_worldspace;
out vec3 eye_dir_cameraspace;
out vec3 light_pos_worldspace;
out vec3 light_dir_cameraspace;
out vec3 normal_cameraspace;
out float zDepth;

out vec4 point_col;

void main()
{
	vec4 vertex_pos_modelspace = vec4(vertex_pos, 1.0); // Transform to 4D homogenous vector.
	vec4 normal_modelspace = vec4(normal, 0.0);

    // Apply skinning to vertex and normal.
    skinned_vertex_pos_modelspace = (bone_transformations[bone_id[0]] * vertex_pos_modelspace) * bone_weight[0];
	skinned_vertex_pos_modelspace = (bone_transformations[bone_id[1]] * vertex_pos_modelspace) * bone_weight[1] + skinned_vertex_pos_modelspace;
	skinned_vertex_pos_modelspace = (bone_transformations[bone_id[2]] * vertex_pos_modelspace) * bone_weight[2] + skinned_vertex_pos_modelspace;
	skinned_vertex_pos_modelspace = (bone_transformations[bone_id[3]] * vertex_pos_modelspace) * bone_weight[3] + skinned_vertex_pos_modelspace;
	
	skinned_normal_modelspace = (bone_transformations[bone_id[0]] * normal_modelspace) * bone_weight[0];
	skinned_normal_modelspace = (bone_transformations[bone_id[1]] * normal_modelspace) * bone_weight[1] + skinned_normal_modelspace;
	skinned_normal_modelspace = (bone_transformations[bone_id[2]] * normal_modelspace) * bone_weight[2] + skinned_normal_modelspace;
	skinned_normal_modelspace = (bone_transformations[bone_id[3]] * normal_modelspace) * bone_weight[3] + skinned_normal_modelspace;
	
	vertex_pos_modelspace = skinned_vertex_pos_modelspace;
	normal_modelspace = skinned_normal_modelspace;
	
	point_col = vec4(1.0, 1.0, 1.0, 1.0);
	
    // Output of vertex in clip space
	gl_Position = WVP * vertex_pos_modelspace;
	
    // Shading data.
    vertex_pos_worldspace = (W * vertex_pos_modelspace).xyz;

    // Vector from vertex to camera (origin 0,0,0 in camera space)
	vec3 vertex_pos_cameraspace = (V * W * vertex_pos_modelspace).xyz;
	eye_dir_cameraspace = vec3(0,0,0) - vertex_pos_cameraspace;
	
	zDepth = vertex_pos_cameraspace.z;
	
    // This should be gathered from camera node.
    light_pos_worldspace = (W*vec4(5.0, 5.0, 13.5, 1.0)).xyz;
	
	// Vector from vertex to the light (hardcoded light pos for now)
	vec3 light_pos_cameraspace = (V * vec4(light_pos_worldspace,1)).xyz;
	light_dir_cameraspace = light_pos_cameraspace + vertex_pos_cameraspace;
	
	// Normal of the vertex in camer space
	normal_cameraspace = (V * W * normal_modelspace).xyz;
}

