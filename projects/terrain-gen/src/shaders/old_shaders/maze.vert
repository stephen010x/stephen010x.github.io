precision mediump float;

attribute vec3 vert_pos;
attribute vec3 vert_norm;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;
uniform mat3 u_norm_mat;
uniform vec4 u_color;
uniform int u_mode;

uniform float u_tex_scale;

varying vec4 v_color;       // interpolated per vertex color
varying vec3 v_norm;        // interpolated per vertex normal
varying vec2 v_tex_coord;   // interpolated per vertex tex image coords
varying vec2 v_norm_coord;  // interpolated per vertex norm image coords
varying vec4 v_coords3;     // interpolated fragment 3d coords




void main() {
    // translate vertex for rasterization
    vec4 abs_pos = u_mod_mat * vec4(vert_pos, 1.0);
    gl_Position = u_proj_mat * abs_pos;

    // rotate normal according to normal matrix
    v_norm = normalize(u_norm_mat * vert_norm);

    /*// create shade colors for fragment shader
    // the vertex normal happens to be the vertex position
    // horray for the simplicify of spheres!
    float l_dot = dot(normalize(vert_pos), normalize(u_light_norm));
    float l_mul = (l_dot + 1.0) * (u_light_map[1] - u_light_map[0])/2.0 + u_light_map[0];
    // clamp to handle saturation
    color = clamp(u_color * l_mul, 0.0, 1.0);
    // make sure opacity is unaffected by shadow
    color[3] = 1.0;*/


    if (u_mode == 0) {
        v_tex_coord = vec2(abs_pos.x + abs_pos.y, abs_pos.z)*u_tex_scale;
    } else if (u_mode == 1) {
        v_tex_coord = vec2(abs_pos.y + abs_pos.z, abs_pos.x + abs_pos.z)*u_tex_scale;
    }
    v_norm_coord = v_tex_coord;

    v_color = u_color;

    // this lets the fragment shader know the 3d coordinates of each fragment
    v_coords3 = abs_pos;

    /*
    if (all(equal(norm, vec3(0.0, 0.0, 1.0)))) {
        color = u_color;
    } else {
        color = u_color / 1.5;
        color[3] = u_color[3];
    }*/

    //color = vec4(1.0, 0.5, 0.2, 1.0);
    //color = clamp(vec4(vert_pos.xyz, 1.0), 0.0, 1.0);
    //color = clamp(vec4(norm.xyz/2.0 + 0.5, 1.0), 0.0, 1.0);
}
