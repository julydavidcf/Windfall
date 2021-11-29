#version 330

// From vertex shader
in vec2 fragmentUV;
in vec4 fragmentColor;
in vec2 fragmentPosition;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	float distance = length(fragmentUV);	// compute float distance, to look at video again

	color = vec4(fragmentColor.rgb, fragmentColor.a * (pow(0.01, distance) - 0.01));
}