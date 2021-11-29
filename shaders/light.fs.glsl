#version 330

// From vertex shader
in vec2 fragmentUV;
in vec4 fragmentColor;
in vec2 fragmentPosition;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	float distance = length(fragmentUV);	// compute float distance

	color = vec4(fragmentColor.rgb * (fragmentColor.a * (1.0 - distance)), fragmentColor.a * (1.0 - distance));
}