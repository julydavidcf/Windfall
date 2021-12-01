#version 330

struct glowCoordinates{
    float xCoordinates[100];
    float yCoordinates[100];
};
uniform sampler2D screen_texture;
uniform float resolutionX;
uniform float resolutionY;
uniform glowCoordinates thingie;
uniform vec2 lightSourcePos;

in vec2 texcoord;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
    vec4 in_color = texture(screen_texture, texcoord);
    color = in_color * 0.2;
    //color = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 uv = gl_FragCoord.xy / resolutionY;
    vec2 lightSource = lightSourcePos / resolutionY;
    vec2 lightBall = uv - lightSource;

//    for (int i; i < lightBall.x; i++) {
//        color = in_color;
//    }

    float lightBallLuminance = max( 0.0, 1.0 - dot( lightBall, lightBall ) );
    vec3 col = vec3(0.9, 0.8, 0.4) * 0.4 * pow( lightBallLuminance, 900.0 );
    color += vec4(col * 1.2, 1.0);
    color += vec4(vec3(0.1, 0.8, 1.0) * 0.7 * pow( lightBallLuminance, 60 ), 1.0);
    color += in_color;

    float output_start = 0.1;
    float output_end = 1.0;
    float input_start = 0.8;
    float input_end = 1.0;   
    
//    if (lightBallLuminance < 0.95) {
//        float input = 0.95;
//        float output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
//        color = in_color * output;
//    }    
//    if (lightBallLuminance < 0.9) {
//        float input = 0.9;
//        float output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
//        color = in_color * output;
//    }
//    if (lightBallLuminance < 0.85) {
//        float input = 0.85;
//        float output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
//        color = in_color * output;
//    }
//
//    if (lightBallLuminance < 0.8) { // increase this value to reduce size of torchlight  
//        color = in_color * 0.1; // multiplied to reduce brightness of color 
//    }

    for (float i = 0.81; i < 1.0; i+=0.01) {
        float input = i;
        float prevInput = i - 0.01;
        float output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);               
        if (lightBallLuminance < i && lightBallLuminance > prevInput) {
            color = in_color * output;
        }
    }

    if (lightBallLuminance < 0.8) { // increase this value to reduce size of torchlight  
        color = in_color * 0.1; // multiplied to reduce brightness of color 
    }

    // need to think about the math behind the color gradient and the radius distance
}