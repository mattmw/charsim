//#version 330
#version 120

//out vec4 outputColor;

void main()
{
	float lerpValue = gl_FragCoord.y / 500.0f;
		
	//outputColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.2f, 0.2f, 0.2f, 1.0f), lerpValue);
	gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
