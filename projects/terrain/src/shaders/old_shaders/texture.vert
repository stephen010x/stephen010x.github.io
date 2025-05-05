precision mediump float;

attribute vec3 vert_pos;
attribute vec3 vert_norm;
attribute vec2 vert_tex;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;
uniform mat3 u_norm_mat;
//uniform vec4 u_color;

varying vec4 color;
varying vec3 norm;
varying vec2 tex_coord;


void main() {
    // translate vertex for rasterization
    gl_Position = u_proj_mat * u_mod_mat * vec4(vert_pos, 1.0);

    // rotate normal according to normal matrix
    vec3 norm = normalize(u_norm_mat * vert_norm);

    tex_coord = vert_tex;
}
