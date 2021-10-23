#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform int silenced;


// Output color
layout(location = 0) out  vec4 color;

void main()
{
	if(silenced == 1){
		vec4 vec = vec4(fcolor[0]/2,fcolor[1]/2,fcolor[2]/2,0.3);
		color = vec * texture(sampler0, vec2(texcoord.x, texcoord.y));
	}
	else {
		color = vec4(fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	}
}
