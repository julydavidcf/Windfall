#version 330

// From vertex shader
in vec2 fragmentUV;
in vec4 fragmentColor;
in vec2 fragmentPosition;


// Output color
layout(location = 0) out  vec4 color;

struct glowCoordinates{
    float xCoordinates[100];
    float yCoordinates[100];
};

uniform glowCoordinates thing;
uniform float resY;
uniform float resX;
uniform float time;

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

    vec2 r =  2.0*vec2( gl_FragCoord.xy - 0.5*vec2(resX, resY))/resY;

	for(int i = 0; i < 3; i++) {
        vec2 starCentre = vec2(thing.xCoordinates[i], thing.yCoordinates[i]);            

            vec2 uv2 = gl_FragCoord.xy / resY;
            vec2 lightBall = uv2 - starCentre;
            float lightBallLuminance = max( 0.0, 1.0 - dot( lightBall, lightBall ) );
            vec3 col = vec3(0.9, 0.8, 0.4) * 0.4 * pow( lightBallLuminance, 900.0 );
            // if (gameLevel > 1) {
            color += vec4(col * 1.2, 1.0);
            color += vec4(vec3(0.9, 0.8, 0.4) * 0.7 * pow( lightBallLuminance, 1 ), 1.0);
            // }
    }
    
    for(int i = 25; i < 24; i++) {
        vec2 starCentre = vec2(thing.xCoordinates[i], thing.yCoordinates[i]);
        vec2 d = r - starCentre;
        // if (length(d) < 0.01) {
            float v1 = sin(d.x +0.4*time);
            float v2 = sin(d.y +0.4*time);
            float v3 = sin(d.x+d.y +0.4*time);
            float v4 = sin(length(d) + 1.*time);
	        float v = v1+v2+v3+v4;
            v *= 1.;

            vec2 uv2 = gl_FragCoord.xy / resY;
            vec2 vUvMoonDiff = uv2 - starCentre;
            float fMoonDot = max( 0.0, 1.0 - dot( vUvMoonDiff, vUvMoonDiff ) );
            vec3 col = vec3(0.1, 0.2, 0.5) * 0.7 * pow( fMoonDot, 900.0 );
            // if (gameLevel > 1) {
            color += vec4(col, 1.0);
            color += vec4(vec3(0.1, 0.2, 0.5) * 0.3 * pow( fMoonDot, 1. ), 1.0);
            // }

       //  } 
    }
}