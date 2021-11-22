#version 330

// Input attributes
in vec3 in_position;
in vec3 in_color;

out vec3 color;
out vec2 pos;

// Application data
uniform mat3 transform;
uniform mat3 projection;

void main()
{
	pos = in_position.xy; // local coordinated before transform
	color = in_color;
	vec3 pos = projection * transform * vec3(pos.xy, 1.0); // why not simply *in_position.xyz ?
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}