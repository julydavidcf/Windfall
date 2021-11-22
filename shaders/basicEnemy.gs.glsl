#version 330 core
layout (triangles) in;
// layout (points, max_vertices = 6) out;
layout (triangle_strip, max_vertices = 6) out;

in vec3 color[];
in vec2 pos[];

out vec3 vcolor;
out vec2 vpos;

uniform float time;
uniform float offset;
uniform int shouldDeform;
uniform int deformType2;

// Application data
// uniform mat3 transform;
// uniform mat3 projection;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   // return normalize(cross(a, b));
   // return  -1 * (a + b);
    vec3 result = b - a;
    // result.y -= 0.3;
    return result;
    // result.y *= sin(1.08);
}

vec3 GetNormal2()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   // return normalize(cross(a, b));
   // return  -1 * (a + b);
    vec3 result = a - b;
    // result.y -= 0.3;
    return result;
    // result.y *= sin(1.08);
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2;
    vec3 direction =  normal * ((sin( 0.001 * time) + 0.1) / 2.0) * magnitude;
    // direction.y *= 1.4;
    return position + vec4(direction, 0.0);
} 

vec4 explode2 (vec4 position, vec3 normal) 
{
    float magnitude = 20.0;
    // if (offset <= 0.9) {
        vec3 direction = normal * offset * magnitude;
        return position + vec4(direction, 0.0);
    // } else {
    //     vec3 direction = normal * offset * 0.1;
    //     return position - vec4(direction, 0.0);
    // }
    
}

void main() {

    vec3 normal = GetNormal();

    if (shouldDeform == 1) {
        gl_Position = explode(gl_in[0].gl_Position, normal);
    } else {
        gl_Position = gl_in[0].gl_Position;
    }
    // gl_Position.y -= 0.3;
    vpos = pos[0];
    vcolor = color[0];
    EmitVertex();

     if (shouldDeform == 1) {
        gl_Position = explode(gl_in[1].gl_Position, normal);
    } else {
        gl_Position = gl_in[1].gl_Position;
    }
    // gl_Position = explode(gl_in[1].gl_Position, normal);
    // gl_Position = gl_in[1].gl_Position;
    // gl_Position.y -= 0.3;
    vpos = pos[1];
    vcolor = color[1];
    EmitVertex();

     if (shouldDeform == 1) {
        if (deformType2 == 1) {
            vec3 normal2 = GetNormal2();
            gl_Position = explode(gl_in[2].gl_Position, normal + normal2);
        } else {
            gl_Position = explode(gl_in[2].gl_Position, normal);
        }
    } else {
        gl_Position = gl_in[2].gl_Position;
    }
    // gl_Position = explode(gl_in[2].gl_Position, normal);
    // gl_Position = gl_in[2].gl_Position;
    // gl_Position.y -= 0.3;
    vpos = pos[2];
    vcolor = color[2];
    EmitVertex();

    // gl_Position = gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position / 3.;
    // gl_PointSize = 5.;
    // vpos = pos[0];
    // vcolor = color[1];
    // EmitVertex();

    // gl_Position = gl_in[0].gl_Position;
    // gl_PointSize = 5.;
    // gl_Position += ((sin(time) + 1.0) / 2.0);
    // vpos = pos[0];
    // vcolor = color[0];
    // EmitVertex();

    EndPrimitive();
}