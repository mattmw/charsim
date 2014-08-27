//#version 330
#version 120

varying vec4 pointCol;

varying vec3 vertexPos_worldspace;
varying vec3 eyeDir_cameraspace;
varying vec3 lightPos_worldspace;
varying vec3 lightDir_cameraspace;
varying vec3 normal_cameraspace;

varying vec2 UV;

vec3 color;

uniform sampler2D myTextureSampler;

void main()
{
    // Light emission properties
	vec3 lightCol = vec3(1,1,1);
	float lightPow = 70.0f;
	
	// Material properties
	//vec3 diffuseCol = vec3(0.7, 0.4, 0.1);
	vec3 diffuseCol = texture2D(myTextureSampler, UV).rgb;
	vec3 ambientCol = vec3(0.5,0.5,0.5) * diffuseCol;
    //vec3 ambientCol = vec3(0.1,0.1,0.1) * diffuseCol;
	vec3 specularCol = vec3(0.5,0.5,0.5);
	
	// Distance to the light
	float distance = length(lightPos_worldspace - vertexPos_worldspace );
	
	// Normal of the computed fragment, in camera space
	vec3 n = normalize(normal_cameraspace);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(lightDir_cameraspace);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
    // Eye vector (towards the camera)
	vec3 e = normalize(eyeDir_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 r = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( e,r ), 0,1 );
	
	color = diffuseCol;
	
	gl_FragColor = vec4(color, 1);
}
