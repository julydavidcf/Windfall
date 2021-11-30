#version 330

// From Vertex Shader
in vec3 vcolor;

// Application data
uniform vec3 fcolor
uniform sampler2D particleTextureBlue;

// Output color
layout(location = 0) out vec4 color;

void main()
{
	color = vec4(fcolor * vcolor, 1.0);
	color = (texture(particleTextureBlue, texCoord)) * life * 2.5;
}