#version 330

struct glowCoordinates{
    float xCoordinates[40];
    float yCoordinates[40];
};
uniform sampler2D screen_texture;
uniform float resolutionX;
uniform float resolutionY;
uniform glowCoordinates thingie;
uniform vec2 lightSourcePos;
uniform float arrow;
uniform float collidesWithFirefly;
uniform float randLight;
in vec2 texcoord;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
    vec4 in_color = texture(screen_texture, texcoord);
    color = in_color * randLight;
    //color = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 uv = gl_FragCoord.xy / resolutionY;
   
    // for the archer arrow
    if (arrow == 1.0) {
        vec2 lightSource = lightSourcePos / resolutionY;
        vec2 lightBall = uv - lightSource;
//      for (int i; i < lightBall.x; i++) {
//        color = in_color;
//      }
        float lightBallLuminance = max( 0.0, 1.0 - dot( lightBall, lightBall ) );

        // vec3 col = vec3(0.9, 0.8, 0.4) * 0.4 * pow( lightBallLuminance, 900000.0 );
        // color += vec4(col * 1.2, 1.0);
//        float arrowSize = 200;
//        if (collidesWithFirefly == 1.0) {
//            arrowSize = 10;
//        }
//        color += vec4(vec3(0.6, 0.1, 0.4) * 0.7 * pow( lightBallLuminance, arrowSize), 1.0);
//        color *= 0.2;
        // color += in_color;  

        float arrowSize = 200;
        if (collidesWithFirefly == 1.0) {
            arrowSize = 10;
            float output_start = randLight;
            float output_end = 1.0;
            float input_start = 0.8;
            float input_end = 1.0;
            for (float i = 0.81; i < 1.0; i+=0.01) {
                float ip = i;
                float prevInput = i - 0.01;
                float op = output_start + ((output_end - output_start) / (input_end - input_start)) * (ip - input_start);               
                if (lightBallLuminance < i && lightBallLuminance > prevInput) {
                    color = in_color * op;
                }
            }
        }
        else {
            float output_start = randLight;
            float output_end = 1.0;
            float input_start = 0.95;
            float input_end = 1.0;
            for (float i = 0.96; i < 1.0; i+=0.01) {
                float ip = i;
                float prevInput = i - 0.01;
                float op = output_start + ((output_end - output_start) / (input_end - input_start)) * (ip - input_start);               
                if (lightBallLuminance < i && lightBallLuminance > prevInput) {
                    color = in_color * op;
                }
            }
        }
        

        // if (lightBallLuminance < 0.80) { // increase this value to reduce size of torchlight  
        //     color = in_color * 0.1; // multiplied to reduce brightness of color 
        // }
    }
   
   // for the fireflies
    for(int i = 0; i < 40; i++) {
        vec2 lightSource = vec2(thingie.xCoordinates[i], thingie.yCoordinates[i]);    
        lightSource = lightSource / resolutionY;
        vec2 lightBall = uv - lightSource;
        float lightBallLuminance = max( 0.0, 1.0 - dot( lightBall, lightBall ) );
        vec3 col = vec3(0.9, 0.8, 0.4) * 0.4 * pow( lightBallLuminance, 9000.0 );
        color += vec4(col * 1.2, 1.0);
        color += vec4(vec3(0.1, 0.8, 1.0) * 0.7 * pow( lightBallLuminance, 1000 ), 1.0);
        // color += in_color;
    }

    
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


    // need to think about the math behind the color gradient and the radius distance
}