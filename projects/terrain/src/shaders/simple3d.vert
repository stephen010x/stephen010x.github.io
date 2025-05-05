#version 300 es
precision mediump float;

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_norm;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;
uniform mat3 u_norm_mat;
uniform vec4 u_color;

out vec4 v_color;
out vec3 v_coords3;
out float v_depth;


void main() {
    // translate vertex for rasterization
    vec4 coords4 = u_mod_mat * vec4(vert_pos, 1.0);
    gl_Position = u_proj_mat * coords4;

    // rotate normal according to model matrix
    vec3 norm = normalize(u_norm_mat * vert_norm);

    v_color = u_color;
    v_coords3 = coords4.xyz;
    v_depth = gl_Position.z;
}
