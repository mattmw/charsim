#version 330
//#version 120

in vec4 point_col;

in vec3 vertex_pos_worldspace;
in vec3 eye_dir_cameraspace;
in vec3 light_pos_worldspace;
in vec3 light_dir_cameraspace;
in vec3 normal_cameraspace;

out vec3 color;

void main()
{
    // Light emission properties
	vec3 light_col = vec3(1,1,1);
	float light_pow = 100.0f;
	
	// Material properties
	vec3 diffuse_col = vec3(0.7, 0.4, 0.1);
	vec3 ambient_col = vec3(0.1,0.1,0.1) * diffuse_col;
	vec3 specular_col = vec3(0.5,0.5,0.5);
	
	// Distance to the light
	float distance = length(light_pos_worldspace - vertex_pos_worldspace );
	
	// Normal of the computed fragment, in camera space
	vec3 n = normalize(normal_cameraspace);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(light_dir_cameraspace);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cos_theta = clamp( dot( n,l ), 0,1 );
	
    // Eye vector (towards the camera)
	vec3 e = normalize(eye_dir_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 r = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cos_alpha = clamp( dot( e,r ), 0,1 );
	
	color = ambient_col +
	        diffuse_col * light_col * light_pow * cos_theta / (distance*distance) +
	        specular_col * light_col * light_pow * pow(cos_alpha,5) / (distance*distance);
}
