#version 330

// Input attributes
in vec3 in_position;	// vertexPosition?
in vec4 vertexColor;
in vec2 in_texcoord;	// vertexUV?

// Passed to fragment shader
out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;


// Application data
uniform mat3 transform;
uniform mat3 projection;
// uniform mat4 P;

void main()
{
	fragmentPosition = vec2(in_position);	// is it right to change to vec2?
	fragmentColor = vertexColor;
	fragmentUV = in_texcoord;	// fragmentUV = vertexUV
	vec3 pos = projection * transform * vec3(in_position.xy, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}