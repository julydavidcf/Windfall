#version 330

// From vertex shader
in vec2 texCoord;
// in vec4 particleColor;
in float life;

// Application data
uniform sampler2D particleTextureBlue;
uniform sampler2D particleTextureRed;
uniform vec4 deathParticleColor;
uniform float particleType;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	// type 1.0 indicates death particles
	// type 2.0 indicates beam particles
	if (particleType == 1.) {
		color = (texture(particleTextureBlue, texCoord)) * life * 2.5;
	} else {
		color = (texture(particleTextureRed, texCoord)) * life * 2.5;
	}
}