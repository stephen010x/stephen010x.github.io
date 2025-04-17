#version 300 es
precision mediump float;

//attribute vec3 vert_pos;
//attribute vec3 vert_norm;
layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_norm;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;
uniform mat3 u_norm_mat;
uniform vec4 u_color;

//uniform float u_tex0_scale;

out vec4 v_color;
out vec2 v_tex_coord;   // interpolated per vertex norm image coords
out vec3 v_coords3;     // interpolated fragment 3d coords
out float v_depth;

out vec3 T;
out vec3 B;
out vec3 N;

out vec4 v_texmap;


uniform bool u_animate;
uniform float u_time;


void main() {
    float h_offset = 0.0;
    float period = 3.0;
    float sinmul = 0.25;
    vec3 newnorm = vert_norm;
    if (u_animate) {
        h_offset = sin((u_time+vert_pos.y)/period)*sinmul;
        newnorm = normalize(vec3(-cos((u_time+vert_pos.y)/period)*sinmul/period, 0.0, 1.0));
    }


    ////////////////////////////
    // vertex and normal translations
    
    // pass 3d coords to fragment shader
    vec4 coords3 = u_mod_mat * vec4(vert_pos.xy, vert_pos.z + h_offset, 1.0);
    v_coords3 = coords3.xyz / coords3.w;
    
    // translate vertex for rasterization
    vec4 pos = u_proj_mat * coords3;




    ////////////////////////////
    // set gl_Position

    // set depth to be logrithmic
    //gl_Position.z = 1.0/gl_Position.z;
    //gl_Position = vec4(pos.xy, log2(pos.z), pos.w);
    gl_Position = pos;




    ////////////////////////////
    // calculate TBN matrix

    // set normal
    // rotate normal according to model matrix
    N = normalize(u_norm_mat * newnorm);

    // select temporary fake tangent
    // avoid colinearity
    T = vec3(1.0, 0.0, 0.0);
    if (abs(N.x) > 0.999)
        T = vec3(0.0, 1.0, 0.0);

    // set bitangent vector
    B = normalize(cross(N, T));

    // set real tangent
    T = normalize(cross(B, N));




    ////////////////////////////
    // calculate normal coords and per-vertex color
    
    v_tex_coord = v_coords3.xy;// / u_tex0_scale;
    v_color = u_color;
    //v_depth = pos.z/pos.w;
    v_depth = pos.z;


    ////////////////////////////
    // calculate texture map

    float wiggle = sin((v_tex_coord.x + v_tex_coord.y)/10.0)*3.0;

    v_texmap = vec4(0.0);
    
    if (v_coords3.z < -25.0 + wiggle)
        if (N.z > 0.8)
            v_texmap[2] = 1.0;
        else
            v_texmap[3] = 1.0;
    else {
        if (N.z > 0.6 + wiggle/100.0)
            //if (v_coords3.z < -15.0 && N.z > 0.7 || v_coords3.z < -21.0)
            if (v_coords3.z < -15.0 + wiggle) //&& N.z > 0.65)
                v_texmap[2] = 1.0;
            else
                v_texmap[0] = 1.0;
        else
            v_texmap[1] = 1.0;
    }
}





