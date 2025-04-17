#version 300 es
precision mediump float;

//attribute vec3 vert_pos;
layout(location = 0) in vec3 vert_pos;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;

out float depth;
//varying vec4 position;


void main() {
    gl_Position = u_proj_mat * u_mod_mat * vec4(vert_pos, 1.0);
    //position = gl_Position;
    //depth = gl_Position.z/gl_Position.w;
    depth = gl_Position.z;
}
