#version 330

uniform sampler2D screen_texture;
uniform float time;
uniform float darken_screen_factor;
uniform float resolutionX;
uniform float resolutionY;

in vec2 texcoord;

layout(location = 0) out vec4 color;


struct glowCoordinates{
    float xCoordinates[100];
    float yCoordinates[100];
};

uniform glowCoordinates thingie;

#define PI 3.14159265359

float noiseHelperFunc(vec2 uv){
	return abs(fract(sin(uv.x*3. + uv.y*4.)*1.));    
}

float generateNoise(vec2 uv){
    
	vec2 gv = fract(uv);	
	vec2 id = floor(uv);
	
    
    gv = gv*gv*(3.-2.*gv);
    
    float bl = noiseHelperFunc(id);
    float br = noiseHelperFunc(id+vec2(1,0));
    float b = mix(bl,br,gv.x);
    
    float tl = noiseHelperFunc(id+vec2(0,1));
    float tr = noiseHelperFunc(id+vec2(1,1));
    float t = mix(tl,tr,gv.x);
    
    float c = mix(b,t,gv.y);
     
    
    return c;
}


float fbm(vec2 gv,float frequency,float amplitude){
	float c=0.;
	float n=0.;
	for(int i=0;i<3;i++){
		c+=generateNoise(gv*frequency*1.)*amplitude;
		frequency*=1.8;
		amplitude*=.38;
		n+=amplitude;
	}
	c/=n;
	return c;	
}

void main()
{
    vec4 in_color = texture(screen_texture, texcoord);
    color = in_color;

    // coordinate transformation for fog
    vec2 uv = gl_FragCoord.xy/vec2(resolutionX, resolutionY);
    uv-=.5;
    uv*=.47;
    uv.x*=resolutionX/resolutionY;
    vec2 iv = 5.*uv;
    
    // control speed of fog
    uv.x += time*0.025;
	uv.y += (time*0.7)*0.1;
    vec2 gv = fract(iv)-.5;
    vec2 id = floor(iv);
    
    // generate fog using fractional brownian motion.
    vec3 outPut;
    vec3 col =vec3(fbm(uv,5.,1.));
    vec3 col2 = col + vec3(fbm(uv, 3., 2.4));
    vec3 col3 = col2 + vec3(fbm(uv, 1., 3.4));
	outPut = vec3(col*0.1);
    // color = vec4(outPut, 0.1);
      

    // For the lightballs
    vec2 r =  2.0*vec2( gl_FragCoord.xy - 0.5*vec2(resolutionX, resolutionY))/resolutionY;

    for(int i = 0; i < 25; i++) {
        vec2 starCentre = vec2(thingie.xCoordinates[i], thingie.yCoordinates[i]);
        vec2 d = r - starCentre;
        if (length(d) < 0.01) {
            float v1 = sin(d.x +0.4*time);
            float v2 = sin(d.y +0.4*time);
            float v3 = sin(d.x+d.y +0.4*time);
            float v4 = sin(length(d) + 1.*time);
	        float v = v1+v2+v3+v4;
            v *= 1.;
            // color = vec4(vec3(sin(v + time*0.5 + 0.7), sin(v + time*0.5 + 1.2), sin(v + time*0.5 + 0.1)), 1.);
            color = vec4(vec3(sin(v * 0.5 * PI), sin(v + 0.1 * PI), sin(v + 1.0 * PI)), 1.);
            // color = vec4(vec3(v4), 1.);

        } 
    }

    for(int i = 25; i < 50; i++) {
        vec2 starCentre = vec2(thingie.xCoordinates[i], thingie.yCoordinates[i]);
        vec2 d = r - starCentre;
        if (length(d) < 0.01) {
            float v1 = sin(d.x +0.4*time);
            float v2 = sin(d.y +0.4*time);
            float v3 = sin(d.x+d.y +0.4*time);
            float v4 = sin(length(d) + 1.*time);
	        float v = v1+v2+v3+v4;
            v *= 1.;
            color = vec4(vec3(sin(v + 0.5*PI), 0., sin(v + 0.5*PI)), 1.);
            // color = vec4(vec3(sin(v * 0.5 * PI), sin(v + 0.1 * PI), sin(v + 1.0 * PI)), 1.);
            // color = vec4(vec3(v4), 1.);

        } 
    }

    for (int i = 50; i < 100; i++) {
        vec2 starCentre = vec2(thingie.xCoordinates[i], thingie.yCoordinates[i]);
        vec2 d = r - starCentre;
        if (length(d) < 0.01) {
            float v1 = sin(d.x +0.5*time);
            float v2 = sin(d.y +0.5*time);
            float v3 = sin(d.x+d.y +0.5*time);
            float v4 = sin(length(d) + 1.*time);
	        float v = v1+v2+v3+v4;
            v *= 1.;
            // color = vec4(vec3(sin(v + time*0.5 + 0.7), sin(v + time*0.5 + 1.2), sin(v + time*0.5 + 0.1)), 1.);
            color = vec4(vec3(sin(v * 0.5 * PI), sin(v + 0.1 * PI), 0.3), 1.);
            // color = vec4(vec3(v4), 1.);

        } 
    }        
}