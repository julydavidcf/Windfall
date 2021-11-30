#version 330

uniform sampler2D screen_texture;
uniform float resolutionX;
uniform float resolutionY;
in vec2 texcoord;


// From vertex shader
// in vec2 fragmentUV;
// in vec4 fragmentColor;
// in vec2 fragmentPosition;

struct glowCoordinates{
    float xCoordinates[100];
    float yCoordinates[100];
};

uniform glowCoordinates thingie;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	// check if distance is within radius 50 of (0,0) to render a small circle

//	vec2 point = vec2(0.0f, 0.0f);
//	float dist = distance(point, fragmentUV.xy);
//	if (dist < 50)
//	{		
//		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//	}
	
//	float distance = length(fragmentUV);	// compute float distance
//
//	//color = vec4(fragmentColor.rgb * (fragmentColor.a * (1.0 - distance)), fragmentColor.a * (1.0 - distance));
//	color = vec4(255.0, 255.0, 255.0, 255.0);

    
    vec4 in_color = texture(screen_texture, texcoord);
    color = in_color;

    // For the lightballs
    vec2 r =  2.0*vec2( gl_FragCoord.xy - 0.5 * vec2(resolutionX, resolutionY))/resolutionY;

    for(int i = 0; i < 3; i++) {
        vec2 starCentre = vec2(thingie.xCoordinates[i], thingie.yCoordinates[i]);
        vec2 uv2 = gl_FragCoord.xy / resolutionY;
        vec2 lightBall = uv2 - starCentre;
        float lightBallLuminance = max( 0.0, 1.0 - dot( lightBall, lightBall ) );
        vec3 col = vec3(0.9, 0.8, 0.4) * 0.4 * pow( lightBallLuminance, 900.0 );
        color += vec4(col * 1.2, 1.0);        
        color += vec4(vec3(0.9, 0.8, 0.4) * 0.7 * pow( lightBallLuminance, 1 ), 1.0);
   }
}