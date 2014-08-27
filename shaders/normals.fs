#version 330
//#version 120

in vec4 vertex_color;
out vec4 outputColor;

void main()
{
	float lerpValue = gl_FragCoord.y / 500.0f;
		
	//outputColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.2f, 0.2f, 0.2f, 1.0f), lerpValue);
	// outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	outputColor = vertex_color;
}
