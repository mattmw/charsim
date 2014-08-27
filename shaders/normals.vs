#version 330
//#version 120
const int TRANSFORMATIONS = 8;

layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in ivec4 bone_id;
layout (location = 4) in vec4 bone_weight;
uniform mat4 WVP;
uniform mat4 bone_transformations[TRANSFORMATIONS];
uniform mat4 rot_transformation;

mat4 new_transformation = mat4(1.0);
vec4 newVertex;
vec4 newNormal;

out vec4 point_col;

out VertexNormal
{
    vec4 normal;
    vec4 color;
} vertex_normal;

void main()
{
	vec4 v = vec4(vposition, 1.0); // Transform to 4D homogenous vector.
	vec4 n = vec4(normal, 0.0);

    newVertex = (bone_transformations[bone_id[0]] * v) * bone_weight[0];
	newVertex = (bone_transformations[bone_id[1]] * v) * bone_weight[1] + newVertex;
	newVertex = (bone_transformations[bone_id[2]] * v) * bone_weight[2] + newVertex;
	newVertex = (bone_transformations[bone_id[3]] * v) * bone_weight[3] + newVertex;
		
	newNormal = (bone_transformations[bone_id[0]] * n) * bone_weight[0];
	newNormal = (bone_transformations[bone_id[1]] * n) * bone_weight[1] + newNormal;
	newNormal = (bone_transformations[bone_id[2]] * n) * bone_weight[2] + newNormal;
	newNormal = (bone_transformations[bone_id[3]] * n) * bone_weight[3] + newNormal;
	
//	vec4 p1 = vec4(0.0, 0.0, 0.0, 1.0);
	
	point_col = vec4(1.0, 1.0, 1.0, 1.0);
	
//	float vert_id = 0.1 * float(gl_VertexID);
	
//	p1.x = vert_id-0.7;
	
//	p1 = transformation * newVertex;

//		point_col = vec4(bone_weight[3], 0, 0, 1);

	gl_Position = newVertex;
//	gl_Position = p1;
    vertex_normal.normal = newNormal;
    vertex_normal.color = vec4(0.0, 0.0, 1.0, 1.0);
}

